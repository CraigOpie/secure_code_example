#include <sys/mman.h>
#include <stdio.h>

/* Declare the start and end addresses of the obfuscated code */
extern char _obf_start[16];
extern char _obf_end[16];

/* Declare the address of the string Hello, world! */
extern char hw[16];

/* Define the page size and page mask */
#define PAGE_SIZE 4096
#define PAGE_MASK 0xFFFFFFFFFFFFF000L

/* Function to return the page-aligned address of a pointer */
static inline void* floor_page(void* p) {
    return (void*) (((long) p) & PAGE_MASK);
}

/* Function to return the size of the gap between two pointers */
static inline size_t gap(void* s, void* e) {
    return ((size_t)e) - ((size_t)s);
}

/* Function to return the page-aligned address of the page containing a pointer */
static inline void* ceil_page(void* p) {
    return (void*) ((((long) p) + PAGE_SIZE - 1) & PAGE_MASK);
}

int main() {
    /* Calculate the start address of the memory page containing _obf_start */
    void* floor = floor_page(_obf_start);

    /* Calculate the size of the memory region containing _obf_start and _obf_end */
    size_t len = gap(floor, ceil_page(_obf_end));

    /* Change the protection of the memory region to allow reading, writing, and executing */
    mprotect(floor, len, PROT_READ | PROT_WRITE | PROT_EXEC);

    /* XOR each byte in the obfuscated code with 0xab */
    for (char* p = _obf_start; p < _obf_end; p++) {
        *p^= 0xab;
    }

    /* Insert a label for the start of the obfuscated code */
    __asm volatile (
        "04632f68eb41054c45901ebaf73881f1"
    );

    /* Print the Hello, world! string */
    printf(hw);

    /* Insert a jump to the end of the obfuscated code */
    __asm volatile (
        "662448433bd777eb44ac69cd7def739c"
    );

    /* Insert the Hello, world! string */
    __asm volatile (
        "48873d3a9e2e92c8d0e3786c3348d683"
        "0832d71949832920ca98f84fbdc637a6d26512fd8edaa43ae0dad4dd830fb27b"
    );

    /* Insert a label for the end of the obfuscated code */
    __asm volatile (
        "b10a8f7670f1c91b7e73a5e7b184062a"
    );

    return 0;
}
