#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PAGES_COUNT 256
#define PAGES_SIZE 256
#define FRAMES_COUNT 256
#define FRAME_SIZE 256
#define TLB_ENTRIES 16

typedef struct memory {

    int count;
    int size;
    int *data;

} memory;

typedef struct page {

    unsigned int page_index; // virtual address divided by page_size
    unsigned int frame_index; // serial
    int valid;

} page;

typedef struct page_table {

    page **entries;
    unsigned int SP;
    unsigned int size;

} page_table;

memory *create_memory(int count, int size) {

    memory *new = (memory *)malloc(sizeof(memory));
    new->count = count;
    new->size = size;
    new->data = (int *)malloc(count * size);
    return new;
}

page *create_page(unsigned int page_index, unsigned int frame_index, int valid) {

    page *new = (page *)malloc(sizeof(page));
    new->page_index = page_index;
    new->frame_index = frame_index;
    new->valid = valid;
    return new;
}

page_table *create_page_table(unsigned int size) {

    page_table *new = (page_table *)malloc(sizeof(page_table));
    new->entries = (page **)malloc(size * sizeof(page *));
    new->SP = 0;
    new->size = size;

    for(unsigned int i = 0; i < size; i++)
        new->entries[i] = NULL;

    return new;
}

page_table *page_table_add(page_table *pt, page *tp) {

    if(pt == NULL) {

        printf("ERROR: NO PAGE TABLE\n");
        _exit(1);
    }

    if(pt->SP == pt->size) {

        printf("ERROR: PAGE INDEX OUT OF RANGE\n");
        _exit(1);
    }

    pt->entries[pt->SP++] = tp;
    return pt;

}

unsigned int check_duplicate(page_table *pt, unsigned int pi) {

    unsigned int ptPi = 0;
    int ptSP = pt->SP;

    for(int i = 0; i < ptSP; i++) {

        ptPi = pt->entries[i]->page_index;

        if(pi == ptPi) return i;

    }

    return ptSP;

}

unsigned int conditional_PT_add(page_table *pt, unsigned int pi) {

    unsigned int ptSP = check_duplicate(pt, pi);

    if(pt->entries[ptSP] == NULL) {
        
        page *np = create_page(pi, ptSP, 0);
        page_table_add(pt, np);

    }
    
    return ptSP;
    
}

void *populate_virtual_memory(memory *vm) {

    // INPUT FILE
    FILE *fptr = fopen("addresses.txt", "r");

    if (fptr == NULL)
        return NULL;
    
    char *lineptr = malloc(sizeof(char) * 200);
    char *address_input = (char *)malloc(5 * sizeof(char));
    int count = 0;

    while (fgets(address_input, 200, fptr) > 0) {

        char *copy = (char *)malloc(sizeof(char) * (strlen(address_input) + 1));
        strcpy(copy, address_input);
        vm->data[count] = atoi(address_input);
        count++;

    }

    fclose(fptr);
    return NULL;

}

unsigned int convert_virtual_to_physical(unsigned int virtual_address, int frame_number){
    // physical address = frame number · page size + offset
    unsigned int physical_address = frame_number * PAGES_SIZE + (virtual_address % PAGES_SIZE);

    return physical_address;
}

int main() {
    
    memory *vm = create_memory(PAGES_COUNT, PAGES_SIZE);
    populate_virtual_memory(vm);
    // printf("test: %d\n", vm->data[999]);

    
    // printf("vm->data: %d", vm->data[0]);
    // for(int i = 1; i < 100; i++)
    //     printf(", %d", vm->data[i]);


    page_table *pt = create_page_table(PAGES_COUNT);
    page_table *TLB = create_page_table(TLB_ENTRIES);

    int TLB_exit = 0;

    // PRINT DESIRED OUTPUT
    for (int i = 0; i < 1000; i++) {

        unsigned int virtual_address = vm->data[i] & 0xFFFF;
        unsigned int page_number = virtual_address/PAGES_SIZE;
        unsigned int frame_number;
        
        //  CHECK TLB
        // if page number is in TLB -> we get our frame number
        // else:
        // add the page to the TLB and
        // conditional_PT_add(pt, page_number);

        int inTLB = 0;
        // Look through TLB
        for (int i = 0; i < TLB_ENTRIES; i++) {

            page *currentpage = TLB->entries[i];

            if(currentpage == NULL)
                continue;

            if (currentpage->page_index == page_number) {

                inTLB = 1;
                frame_number = currentpage->frame_index;
                break;

            }

        }

        if (inTLB != 1) { // ADD ALL THE FUCKING CODE TO ADD TO PAGE TABLE
        
            TLB_exit++;
            frame_number = conditional_PT_add(pt, page_number);
            page_table_add(TLB, pt->entries[frame_number]);
            
             if(TLB->SP == TLB_ENTRIES)
                 TLB->SP = 0;
        
        }
        
        // This will run independent of weather TBL was used or not
        unsigned int physical_address = convert_virtual_to_physical(virtual_address, frame_number);
         printf("Virtual address: %u Physical address: %u\n", virtual_address, physical_address);

    }

    printf("\n Exit counter: %d\n", TLB_exit);

    printf("TLB: %u", TLB->entries[0]->frame_index);
    for(int i = 1; i < TLB_ENTRIES; i++)
        printf(", %u", TLB->entries[i]->frame_index);

    // PRINT PAGE TABLE
    // printf("\nPAGE TABLE\n");
    // for(int i = 0; i < pt->SP; i++)
    //     printf("| page_index:  %3u  |  frame_index: %3u |\n", pt->entries[i]->page_index, pt->entries[i]->frame_index);

    
    
}