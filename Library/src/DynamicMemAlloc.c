/* libs */
#include "STD_TYPES.h"
/* own files */
#include "List.h"
#include "DynamicMemAlloc.h"
#include "DynamicMemAlloc_cfg.h"


/* type for a memory block */
typedef void* Block_t;

/* type for each allocation node haeder */
typedef struct
{
   List_Node_t listNode; /* node pointers (previous and after) */
   u16 nodeBlocksCount;  /* blocks count of this node (excluding overhead) */
} MemNode_t;


/* the size of each memory unit/block (in bytes) */
#define BLOCK_SIZE_BYTES             ((u16)sizeof(void*))

/* a secret word/value added at the beginning and end of the allocated blocks to later check for heap corruption */
#define SECRET_WORD                  ((Block_t)0x101EC5DE101EC5DE)

/* the size of the node overhead (in blocks) */
#define ALLOC_OVERHEAD_SIZE_BLOCKS   ((u16)sizeof(MemNode_t) / BLOCK_SIZE_BYTES)

 /* the amount of blocks reserved for secret words */
#define SECRET_BLOCKS_COUNT          ((u16)2)


/* internal heap, used by both lists (list of free nodes and list of allocated nodes) */
static Block_t internalMemPool[DYNAMIC_MEMORY_POOL_SIZE_BYTES / BLOCK_SIZE_BYTES];

/* the list of free nodes, each node is dynamic in size depending on size of each node */
static List_t freeList;

/* the list of allcoated (not free) nodes, each node is dynamic in size depending on allocation size */
static List_t allocedList;


/* private/internal functions */
/* ----------------------------------*/
/* this function initializes both lists and adds the internal heap to the free list */
static void DynamicMem_InitListsIfNotInitialized(void);

/* this function attempts to defragement the free list after free() call */
static void DynamicMem_DefragmentFreeListIfPossible(void);

/* callback for the sort function of the list, it sorts nodes ascendingly by their addresses */
static u8 DynamicMem_SortListAscendingByNodeAddrCallback(const List_Node_t* nextNode, const List_Node_t* fittingNode);
/* ----------------------------------*/


/* public APIs/functions */
u8 DynamicMem_AppendBlock(void* mem, u16 size)
{
   u8 state; /* the state of the appending operation */

   /* if the amount of blocks in this heap if more than overhead blocks + secret blocks (sufficient for at least 1 data block) */
   if ((size / BLOCK_SIZE_BYTES) > (ALLOC_OVERHEAD_SIZE_BLOCKS + SECRET_BLOCKS_COUNT))
   {
      /* attempt to initialize the library if it wasn't already */
      DynamicMem_InitListsIfNotInitialized();

      /* the size of this node = size of it in blocks - overhead size in blocks */
      ((MemNode_t*)mem)->nodeBlocksCount = size / BLOCK_SIZE_BYTES - ALLOC_OVERHEAD_SIZE_BLOCKS;

      /* append the new memory block */
      List_Append(&freeList, (List_Node_t*)mem);

      /* sort then defragment the free list if possible (combine contiguous memory blocks) */
      DynamicMem_DefragmentFreeListIfPossible();

      /* set the function return state to OK */
      state = DYNAMIC_MEM_ERROR_OK;
   }
   else /* if this heap has no room for at least 1 data block */
   {
      /* set the function return state to OK */
      state = DYNAMIC_MEM_ERROR_NOK;
   }

   /* return the state of appending the given memoty block */
   return state;
}

u16 DynamicMem_Alloc(u16 newBlockSize, void** out)
{
   MemNode_t* foundNode; /* a free node whose blocks count is >= required blocks + 2 blocks for secret word */
   MemNode_t* newNode;   /* used to split the found node if can hold more blocks after allocation */
   u16 foundNodeBlocks;  /* the amount of pure blocks in the found node: all node blocks - overhead blocks - 2 (blocks of secret word) */

   /* attempt to initialize the library if it wasn't already */
   DynamicMem_InitListsIfNotInitialized();

   /* reset the allocated blocks indicator */
   foundNodeBlocks = 0;

   /* adjust the required block size:
    * required size in blocks
    * +1 if the required size in bytes is not a multiple of the block size
    * +2 for the wrapping secret words */
   newBlockSize = newBlockSize / BLOCK_SIZE_BYTES
      + !!(newBlockSize % BLOCK_SIZE_BYTES)
      + 2;

   /* point at first node in the free list */
   foundNode = (MemNode_t*)freeList.headNode;

   /* search for a node with size >= required size */
   /* loop until we've passed by all free nodes, or until a fitting node is found */
   for (u16 i = freeList.nodesCount; (i > 0) && (foundNodeBlocks == 0); i--)
   {
      /* if the amount of pure blocks in this node is sufficient for the new block */
      if (foundNode->nodeBlocksCount >= newBlockSize)
      {
         /* update the indicator of the allocated blocks count, remove the count of secret words blocks */
         foundNodeBlocks = foundNode->nodeBlocksCount - 2;
      }
      else /* if this node size is NOT sufficient in size */
      {
         /* point at next node */
         foundNode = (MemNode_t*)(foundNode->listNode.nextNode);
      }
   }

   if (foundNodeBlocks) /* if any fitting node was found */
   {
      /* if there'll be extra space left in this block after reserving */
      /* (pure blocks in found node + overhead) - (new block size + overhead) > (overhead + 2)
       * (pure blocks in found node) - (new block size) > (overhead + 2) */
      if ((foundNode->nodeBlocksCount - newBlockSize) > (ALLOC_OVERHEAD_SIZE_BLOCKS + 2))
      {
         /* split the block */
         /* the new node is exactly after the newly alloced block */
         newNode = (MemNode_t*)((Block_t*)foundNode + ALLOC_OVERHEAD_SIZE_BLOCKS + newBlockSize);

         /* the amount of the blocks in the new node is
          * = (size of found node) - (size of new node) - (overhead size)
          * = (size of pure blocks in found node + overhead blocks)
          *   - (size of pure blocks in new node + overhead blocks)
          *   - (overhead blocks)
          * = (size of pure blocks in the found node)
          *   - (size of blocks in new node)
          *   - overhead blocks */
         newNode->nodeBlocksCount = foundNode->nodeBlocksCount - ALLOC_OVERHEAD_SIZE_BLOCKS - newBlockSize;

         /* add the new free node to the list, we append/prepend the new free node below/above this one (newly allocated)
          * because this one will be removed from the free list so the new node will be in-place of it.
          * this keeps the free list sorted */
         List_InsertBefore(&freeList, &newNode->listNode, &foundNode->listNode);

         /* we shouldn't attempt to defragment the free list here
          * because splitting a node won't fragment it */
      }
      else /* if this node will be reserved with no extra space for another nodes */
      {
         /* add the extra left blocks to the required amount of blocks,
          * this prevents those blocks from being dead,
          * otherwise this node (after being allocated) won't point at them
          * and when free-ing it they won't be reclaimed back */
         newBlockSize += foundNode->nodeBlocksCount - newBlockSize;
      }

      /* set the size of the found blocks to the amount of pure blocks (total allocation size - overhead)*/
      foundNode->nodeBlocksCount = newBlockSize;

      /* remove the found block from the free list since it's now alloced (not free)
       * and add it to the alloced list */
      List_Remove(&freeList, &foundNode->listNode);
      List_Append(&allocedList, &foundNode->listNode);

      /* put the secret word at the beginning and end of the pure blocks (blocks without the overhead) */
      *((Block_t*)foundNode + ALLOC_OVERHEAD_SIZE_BLOCKS) = SECRET_WORD;
      *((Block_t*)foundNode + ALLOC_OVERHEAD_SIZE_BLOCKS + newBlockSize - 1) = SECRET_WORD;

      /* set the output to the block exactly after 1st secret word */
      *out = (void*)((Block_t*)foundNode + ALLOC_OVERHEAD_SIZE_BLOCKS + 1);
   }
   else /* if no fitting node was found */
   {

   }

   /* return the amount of the allocated blocks (in bytes) - overhead blocks count - 2 (secret word blocks) */
   return (newBlockSize - 2) * BLOCK_SIZE_BYTES;
}

u8 DynamicMem_Free(void* ptr)
{
   u8 state; /* the state of the appending operation */

   MemNode_t* searchNode; /* points at each node in the allcoated list, used to locate the corresponding node */
   u8 isNodeFound;        /* whether we found the required node in the allocated list*/
   u16 blocksCount;       /* the amount of pure blocks in this node */
   Block_t* blocks;       /* pointer to the beginning of the pure blocks */

   /* start searching from the head node of the allocated list */
   searchNode = (MemNode_t*)allocedList.headNode;

   /* initially we didn't find any node, hence reset the indicator */
   isNodeFound = 0;

   /* search for the address of the node in the allocated list */
   for (u16 i = allocedList.nodesCount; (i > 0) && !isNodeFound; i--)
   {
      /* if the given address is within the range of blocks of this node
       * (ptr >= address of the block after 1st secret block) && (ptr < address of last secret block) */
      if ( ((Block_t*)ptr >= ((Block_t*)searchNode + ALLOC_OVERHEAD_SIZE_BLOCKS + 1)) &&
           ((Block_t*)ptr < ((Block_t*)searchNode + ALLOC_OVERHEAD_SIZE_BLOCKS + searchNode->nodeBlocksCount - 1)) )
      {
         /* set the indicator */
         isNodeFound = 1;
      }
      else /* if we didn't find the node in the allocated list yet */
      {
         /* get the next node */
         searchNode = (MemNode_t*)searchNode->listNode.nextNode;
      }
   }

   if (isNodeFound) /* if any node is found */
   {
      /* get the address of the pure blocks (address of 1st secret word), node address + overhead */
      blocks = (Block_t*)searchNode + ALLOC_OVERHEAD_SIZE_BLOCKS;

      /* get the pure blocks count (counting the 2 blocks of the secret word) */
      blocksCount = searchNode->nodeBlocksCount;

      /* if the secret wrapping words are found at beginning and end of the pure blocks */
      if ((blocks[0] == SECRET_WORD) && (blocks[blocksCount - 1] == SECRET_WORD))
      {
         /* clear the secret words */
         blocks[0] = 0;
         blocks[blocksCount - 1] = 0;

         /* remove the found node from the alloced list and add it to the free list */
         List_Remove(&allocedList, &searchNode->listNode);
         List_Append(&freeList, &searchNode->listNode);

         /* sort then defragment the free list if possible (combine contiguous memory blocks) */
         DynamicMem_DefragmentFreeListIfPossible();

         /* set the function return state to OK */
         state = DYNAMIC_MEM_ERROR_OK;
      }
      else /* if the secret wrapping words are not found in the pure blocks */
      {
         /* set the function return state to NOK */
         state = DYNAMIC_MEM_ERROR_NOK;
      }
   }
   else /* if the given address wasn't corresponding to any node in the allocated list */
   {
      /* set the function return state to NOK */
      state = DYNAMIC_MEM_ERROR_NOK;
   }

   /* return the state of free-ing the node containing the given address */
   return state;
}

// TODO: implement this API
static u16 DynamicMem_Realloc(u16 newBlockSize, void** in_out)
{
	(void)newBlockSize;
	(void)in_out;

   return 0;
}

/* private/internal functions */
static void DynamicMem_InitListsIfNotInitialized(void)
{
   static u8 isLibraryInitialized = 0;

   if (!isLibraryInitialized) /* if library was not initialized */
   {
      /* set the initialization flag to indicate that it's now initialized */
      isLibraryInitialized = 1;

      /* initialize the free and allocated lists */
      List_Init(&freeList);
      List_Init(&allocedList);

      /* add the big memory block (heap) to the list as one big node */
      List_Append(&freeList, (List_Node_t*)internalMemPool);

      /* the size of this big node = size of it in blocks - overhead size in blocks */
      ((MemNode_t*)freeList.headNode)->nodeBlocksCount = (
         (u16)DYNAMIC_MEMORY_POOL_SIZE_BYTES / BLOCK_SIZE_BYTES -
         ALLOC_OVERHEAD_SIZE_BLOCKS);
   }
   else /* if library was already initialized */
   {

   }
}

static void DynamicMem_DefragmentFreeListIfPossible(void)
{
   /* current node indexer */
   MemNode_t* currentNode;

   /* sort all the nodes ascendingly be their address */
   List_Sort(&freeList, DynamicMem_SortListAscendingByNodeAddrCallback);

   /* start searching from the head node of the free list */
   currentNode = (MemNode_t*)freeList.headNode;

   /* loop until we reach the tail node
    * this condition is satisfied if:
    * 1- we reached the tail node
    * 2- we have only one node left (since it's a circular list) */
   while ((List_Node_t*)currentNode != freeList.tailNode)
   {
      /* if the next node (by pointer maths) is the next node in the list (2 nodes contiguous in memory) */
      if (((Block_t*)currentNode + ALLOC_OVERHEAD_SIZE_BLOCKS + currentNode->nodeBlocksCount) == (Block_t*)currentNode->listNode.nextNode)
      {
         /* set the size of this node to:
          *   (pure blocks count of current node) + (next node size)
          * = (pure blokcs count of current node) + (overhead + count of pure blocks in next node)  */
         currentNode->nodeBlocksCount += ALLOC_OVERHEAD_SIZE_BLOCKS + ((MemNode_t*)currentNode->listNode.nextNode)->nodeBlocksCount;

         /* then remove the next node since we now took its space */
         List_Remove(&freeList, currentNode->listNode.nextNode);
      }
      else /* if next node (by pointer maths) is NOT the next node in the list (2 nodes noncontiguous in memory) */
      {
         currentNode = (MemNode_t*)currentNode->listNode.nextNode;
      }
   }
}

static u8 DynamicMem_SortListAscendingByNodeAddrCallback(const List_Node_t* nextNode, const List_Node_t* fittingNode)
{
   return (nextNode < fittingNode);
}
