#ifndef DYNAMIC_MEM_ALLOC_H
#define DYNAMIC_MEM_ALLOC_H

/**
 * @brief An OK state, indicating a success
 *
 */
#define DYNAMIC_MEM_ERROR_OK    0

/**
 * @brief A Not OK state, indicating a failure
 *
 */
#define DYNAMIC_MEM_ERROR_NOK   1

/**
 * @brief Appends a given memory to the free list as a heap
 *
 * @param mem The address of the memory to be added, this must be aligned to the size of the pointer sizeof(void*)
 * @param size The size of the given memory in bytes, this is rounded down to the nearest size of the pointer sizeof(void*)
 * @return The state of appending:\n
 *         DYNAMIC_MEM_ERROR_OK: the memory block was added successfully.\n
 *         DYNAMIC_MEM_ERROR_NOK: the memory block was not added, because its size is smaller than the overhead of 1 allocation.\n
 */
u8 DynamicMem_AppendBlock(void* mem, u16 size);

/**
 * @brief Allocate a memory block from the free list (heap)
 *
 * @param newBlockSize The required size of the allocation in bytes, this is rounded up to the nearest size of the pointer sizeof(void*)
 * @param out The address of the pointer/buffer that'll hold the address of the allocated memory block
 * @return The size of the allocated memory in bytes
 */
u16 DynamicMem_Alloc(u16 newBlockSize, void** out);

/**
 * @brief Free/Deallocate a memory block by its given address
 *
 * @param ptr A pointer in the range of a memory block previously allocated by DynamicMem_Alloc,
 * @return The state of the free-ing:\n
 *         DYNAMIC_MEM_ERROR_OK: the memory block was free-ed successfully.\n
 *         DYNAMIC_MEM_ERROR_NOK: the memory block was not free-ed, because the given address didn't correspond to any allocated block.\n
 */
u8 DynamicMem_Free(void* ptr);

#endif /* DYNAMIC_MEM_ALLOC_H */
