#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256 // Number of pages and frames
#define C 256 // Page and frame size

typedef struct memory
{
    int n;
    int c;
    int *a;
} memory;

typedef struct {
    int page_number;   // The page number (explicitly stored for clarity)
    int frame_number;  // The physical frame number
    int valid;         // Validity of the entry (1 if valid, 0 if invalid)
    int dirty;         // Dirty bit (1 if modified, 0 if clean)
} page_table_entry;

typedef struct {
    page_table_entry entries[N];  // Array of page table entries
    int next_free_frame;         // Keeps track of the next free frame to assign
} page_table;


// VIRTUAL MEMORY
memory *create_virtual_memory() {
    memory *vm = (memory *)malloc(sizeof(memory));
    vm->n = N;
    vm->c = C;
    vm->a = (int *)malloc((vm->n * vm->c));
    return vm;
}

void *populate_virtual_memory(memory *vm) {
    // INPUT FILE
    FILE *fptr = fopen("addresses.txt", "r");
    if (fptr == NULL)
    {
        return NULL;
    }
    
    char *lineptr = malloc(sizeof(char) * 200);
    char *address_input = (char *)malloc(5 * sizeof(char));

    int count = 0;
    while (fgets(address_input, 200, fptr) > 0)
    {
        char *copy = (char *)malloc(sizeof(char) * (strlen(address_input) + 1));
        strcpy(copy, address_input);
        vm->a[count] = atoi(address_input);
        count++;
    }
    fclose(fptr);
    return NULL;
}


// PHYSICAL MEMORY
memory *create_physical_memory() {
    memory *pm = (memory *)malloc(sizeof(memory));
    pm->n = N;
    pm->c = C;
    pm->a = (int *)malloc((pm->n * pm->c));
    return pm;
}


// CREATE PAGE TABLE
page_table *create_page_table() {
    page_table *pt = (page_table *)malloc(sizeof(page_table));
    
    // Initialize all entries to be invalid
    for (int i = 0; i < N; i++) {
        pt->entries[i].page_number = -1;   // Not yet assigned
        pt->entries[i].frame_number = -1;  // No frames assigned
        pt->entries[i].valid = 0;          // Invalid initially
        pt->entries[i].dirty = 0;          // Not dirty initially
    }
    
    pt->next_free_frame = 0; // Start assigning frames from 0
    
    return pt;
}

//converting from virtual memory to physical memory
//we use the page number in the virtual memory and set it as a
//tag in our page table. The tag is linked to the page number of
//the physical address. We then take the physical page number and


void convert_virtual_to_physical(int *virtual_addresses, int num_addresses, page_table *pt) {
    for (int i = 0; i < num_addresses; i++) {
        int va = virtual_addresses[i];  // Get virtual address

        // Step 1: Extract page number and offset from virtual address
        int page_number = va / C;  // Page number = Virtual Address / Page Size
        int page_offset = va % C;  // Page offset = Virtual Address % Page Size

        // Step 2: Check if the page is in the page table (i.e., valid)
        if (pt->entries[page_number].valid == 0) {
            // Page fault: This page is not in memory, so we assign a new frame
            if (pt->next_free_frame >= N) {
                printf("Error: No free frames available.\n");
                return;
            }
            
            // Assign the page to the next available frame
            pt->entries[page_number].frame_number = pt->next_free_frame;
            pt->entries[page_number].valid = 1;  // Mark the page as valid
            pt->next_free_frame++;  // Move to the next free frame
        }

        // Step 3: Calculate the physical address
        int frame_number = pt->entries[page_number].frame_number;
        int physical_address = (frame_number * C) + page_offset;

        // Step 4: Print or store the physical address
        printf("Virtual Address: %d -> Physical Address: %d\n", va, physical_address);
    }
}


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
    memory *vm = create_virtual_memory();
    populate_virtual_memory(vm);
    printf("test: %d\n", vm->a[999]);
}