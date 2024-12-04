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

    int page_index; // virtual address divided by page_size
    int frame_index; // serial
    int valid;
    int dirty;

} page;

typedef struct page_table {

    page **entries;
    int SP;
    int size;

} page_table;

memory *create_memory(int count, int size) {

    memory *new = (memory *)malloc(sizeof(memory));
    new->count = count;
    new->size = size;
    new->data = (int *)malloc(count * size);
    return new;

}

page *create_page(int page_index, int frame_index, int valid, int dirty) {

    page *new = (page *)malloc(sizeof(page));
    new->page_index = page_index;
    new->frame_index = frame_index;
    new->valid = valid;
    new->dirty = dirty;
    return new;

}

page_table *create_page_table(int size) {

    page_table *new = (page_table *)malloc(sizeof(page_table));
    new->entries = (page **)malloc(size * sizeof(page *));
    new->SP = 0;
    new->size = size;

    for(int i = 0; i < size; i++)
        new->entries[i] = NULL;

    return new;

}

int *page_table_add(page_table *pt, page *tp) {

    if(pt == NULL) {

        printf("ERROR: NO PAGE TABLE\n");
        _exit(1);

    }

    if(pt->SP == pt->size) {

        printf("ERROR: PAGE INDEX OUT OF RANGE\n");
        _exit(1);

    }

    int writeAt = pt->SP;
    pt->entries[writeAt] = tp;
    pt->SP++;
    return writeAt;

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

//converting from virtual memory to physical memory
//we use the page number in the virtual memory and set it as a
//tag in our page table. The tag is linked to the page number of
//the physical address. We then take the physical page number and


// void convert_virtual_to_physical(int *virtual_addresses, int num_addresses, page_table *pt) {
//     for (int i = 0; i < num_addresses; i++) {
//         int va = virtual_addresses[i];  // Get virtual address

//         // Step 1: Extract page number and offset from virtual address
//         int page_number = va / C;  // Page number = Virtual Address / Page Size
//         int page_offset = va % C;  // Page offset = Virtual Address % Page Size

//         // Step 2: Check if the page is in the page table (i.e., valid)
//         if (pt->entries[page_number].valid == 0) {
//             // Page fault: This page is not in memory, so we assign a new frame
//             if (pt->next_free_frame >= N) {
//                 printf("Error: No free frames available.\n");
//                 return;
//             }
            
//             // Assign the page to the next available frame
//             pt->entries[page_number].frame_number = pt->next_free_frame;
//             pt->entries[page_number].valid = 1;  // Mark the page as valid
//             pt->next_free_frame++;  // Move to the next free frame
//         }

//         // Step 3: Calculate the physical address
//         int frame_number = pt->entries[page_number].frame_number;
//         int physical_address = (frame_number * C) + page_offset;

//         // Step 4: Print or store the physical address
//         printf("Virtual Address: %d -> Physical Address: %d\n", va, physical_address);
//     }
// }


void convert_virtual_to_physical(){
    // Take virtual address, check first 8 bits = frame number
    // We take the virtual address mod frame size = page offset
    // Frame number X frame size + offset = physical address
    //llr

    //in the page table the page number will be uninitialized and
    //as we start converting virtual addresses to physical addresses
    //we will update the page number in the page table
    //so the first page number we get e.g. 10 will be linked to the
    //first frame number, 0. The second page number we get e.g. 15
    //will be linked to the second frame number, 1.
}

int main() {

    memory *vm = create_memory(PAGES_COUNT, PAGES_SIZE);
    populate_virtual_memory(vm);
    printf("test: %d\n", vm->data[999]);

    page_table *pt = create_page_table(1000);

    for(int i = 0, p = 0; i < vm->size; i += PAGES_SIZE, p++)
        page_table_add(pt, create_page(p,0,0,0));

}