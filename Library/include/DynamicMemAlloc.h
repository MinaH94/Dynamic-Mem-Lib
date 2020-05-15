#ifndef DYNAMIC_MEM_ALLOC_H
#define DYNAMIC_MEM_ALLOC_H

#define DYNAMIC_MEM_ERROR_OK    0
#define DYNAMIC_MEM_ERROR_NOK   1

u8 DynamicMem_AppendBlock(void* mem, u16 size);

u16 DynamicMem_Alloc(u16 newBlockSize, void** out);

u8 DynamicMem_Free(void* ptr);

#endif /* DYNAMIC_MEM_ALLOC_H */
