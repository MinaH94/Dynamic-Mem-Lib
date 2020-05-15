#include "STD_TYPES.h"
#include "List.h"
#include "DynamicMemAlloc.h"
#include <diag/Trace.h>


u32 mempool[16];

void* ptr;
void* ptr1;
void* ptr2;
void* ptr3;
void* ptr4;
void* ptr5;
void* ptr6;
void* ptr7;

u16 sz;
u8 state;


void TEST_dynamicAllocation(void)
{
   DynamicMem_AppendBlock(mempool, sizeof(mempool));

   sz = DynamicMem_Alloc(4, &ptr);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr, sz);

   sz = DynamicMem_Alloc(4, &ptr1);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr1, sz);

   sz = DynamicMem_Alloc(4, &ptr2);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr2, sz);

   sz = DynamicMem_Alloc(4, &ptr3);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr3, sz);

   sz = DynamicMem_Alloc(4, &ptr4);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr4, sz);

   sz = DynamicMem_Alloc(4, &ptr5);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr5, sz);

   sz = DynamicMem_Alloc(4, &ptr6);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr6, sz);

   sz = DynamicMem_Alloc(4, &ptr7);
   trace_printf("Allocated addr @ 0x%08X, with size = %d bytes\n", ptr7, sz);


   state = DynamicMem_Free(ptr6);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 ptr6,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");

   state = DynamicMem_Free(ptr4);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 ptr4,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");

   state = DynamicMem_Free(ptr2);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 ptr2,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");

   state = DynamicMem_Free(ptr);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 ptr,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");


   state = DynamicMem_Free(ptr1);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 ptr1,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");

   state = DynamicMem_Free(ptr5);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 ptr5,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");

   state = DynamicMem_Free(ptr3);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 ptr3,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");

   state = DynamicMem_Free((u8*)ptr7 + 2);
   trace_printf("Free-ing addr @ 0x%08X, result = %s\n",
					 (u8*)ptr7 + 2,
					 (state == DYNAMIC_MEM_ERROR_OK) ? "DYNAMIC_MEM_ERROR_OK" : "DYNAMIC_MEM_ERROR_NOK");
}
