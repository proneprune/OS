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

//converting from virtual memory to physical memory
//we use the page number in the virtual memory and set it as a
//tag in our page table. The tag is linked to the page number of
//the physical address. We then take the physical page number and


void convert_virtual_to_physical(){
    // Take virtual address, check first 8 bits = frame number
    // We take the virtual address mod frame size = page offset
    // Frame number X frame size + offset = physical address
    llr
}



int main() {
    memory *vm = create_virtual_memory();
    populate_virtual_memory(vm);
    printf("test: %d\n", vm->a[999]);
}