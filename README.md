# Dynamic Mem Lib
A library for dynamic memory allocation that uses a circular linked-list.

## Credits
All credits to Phillip Johnston for his amazing explanation at: [Implementing Malloc: First-fit Free List](https://embeddedartistry.com/blog/2017/02/15/implementing-malloc-first-fit-free-list/)

## Target
This is written to be a target-independant library, hence it could be used anywhere! \
But it was mainly written for STM32, also the test application is written for STM32.

## How to use
1. Open the file [DynamicMemAlloc_cfg.h](https://github.com/MinaH94/Dynamic-Mem-Lib/blob/master/Library/include/DynamicMemAlloc_cfg.h) and set the macro `DYNAMIC_MEMORY_POOL_SIZE_BYTES` to the required size of the heap (in bytes).
2. You can optionally append a memory block of your own by calling [DynamicMem_AppendBlock()](https://github.com/MinaH94/Dynamic-Mem-Lib/blob/0ace240dbf1a236d8b6f6dabf8813a08304f9026/Library/include/DynamicMemAlloc.h#L25).
3. Call [DynamicMem_Alloc()](https://github.com/MinaH94/Dynamic-Mem-Lib/blob/0ace240dbf1a236d8b6f6dabf8813a08304f9026/Library/include/DynamicMemAlloc.h#L34) and [DynamicMem_Free()](https://github.com/MinaH94/Dynamic-Mem-Lib/blob/0ace240dbf1a236d8b6f6dabf8813a08304f9026/Library/include/DynamicMemAlloc.h#L44)

Sample code:
```c
#include "STD_TYPES.h"
#include "DynamicMemAlloc.h"
#include <stdio.h>

/* these pointers will hold the addresses of the allocated memory blocks */
void* ptr;
void* ptr1;

/* an optional memory heap/pool */
//void* mempool[16];

void main(void)
{
   /* (optional) add the our memory pool to the list of free memory */
   //DynamicMem_AppendBlock(mempool, sizeof(mempool));

   /* try to allocate 4 bytes */
   sz = DynamicMem_Alloc(4, &ptr);
   printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr, sz);

   /* try to allocate another 4 bytes */
   sz = DynamicMem_Alloc(4, &ptr1);
   printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr1, sz);

   /* free the 2nd block by an address in the middle */
   state = DynamicMem_Free((u8*)ptr1 + 2);
   printf("Free-ing addr @ 0x%08X, result = %s\n", (u8*)ptr1 + 2, (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");

   /* free the 1st block by its address */
   state = DynamicMem_Free(ptr);
   printf("Free-ing addr @ 0x%08X, result = %s\n", ptr, (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");
}
```

The size of `DYNAMIC_MEMORY_POOL_SIZE_BYTES ` should be a multiple of `sizeof(void*)`, else it'll be rounded down, for further reading about this requirement/restriction see this: [The Lost Art of Structure Packing: 3. Alignment requirements](http://www.catb.org/esr/structure-packing/)

## TODO
- Implement `realloc()`: get node address of given addr, if next node by pointer math is in free list, and sizes of both nodes combined (this node and next one by pointer math) has blocks count >= required, then accept request.

- Make the APIs of the linked-list return boolean value
