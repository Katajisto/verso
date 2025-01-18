#include <stdio.h>
#include "verso.h"

int main() {
    // Create an arena with 64-byte blocks
    Verso* v = VersoNew(64);
    if (v == NULL) {
        printf("Failed to create arena\n");
        return 1;
    }

    // Allocate some memory of different sizes
    char* str1 = VersoAlloc(v, 20);  // Should fit in first block
    char* str2 = VersoAlloc(v, 30);  // Should fit in first block
    char* str3 = VersoAlloc(v, 40);  // Should create new block
    
    // Write to memory to ensure it's usable
    sprintf(str1, "Hello");
    sprintf(str2, "World");
    sprintf(str3, "Test");
    
    // Print to verify
    printf("%s %s %s\n", str1, str2, str3);
    
    VersoReset(v);
    
    // Allocate some memory of different sizes
    char* dstr1 = VersoAlloc(v, 20);  // Should fit in first block
    char* dstr2 = VersoAlloc(v, 30);  // Should fit in first block
    char* dstr3 = VersoAlloc(v, 40);  // Should create new block
    
    // Write to memory to ensure it's usable
    sprintf(str1, "Hello");
    sprintf(str2, "World");
    sprintf(str3, "Test");
    
    // Print to verify
    printf("%s %s %s\n", str1, str2, str3);
    
    // Print total allocation
    printf("Total allocated: %zu\n", v->totalAlloc);
    
    // Cleanup
    VersoFree(v);

    VersoCheck();
    
    return 0;
}
