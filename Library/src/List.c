/* libs */
#include "STD_TYPES.h"
/* own */
#include "List.h"


#define MEM_ALLOC_OK  0
#define MEM_ALLOC_NOK 1

static void List_AddFirstNode(List_t* list, List_Node_t* node);
static void List_InsertBetween(List_t* list, List_Node_t* node, List_Node_t* beforeNode, List_Node_t* afterNode);

void List_Init(List_t* list)
{
   /* reset everything */
   list->headNode = 0;
   list->tailNode = 0;
   list->nodesCount = 0;
}

void List_Append(List_t* list, List_Node_t* node)
{
   if (list->tailNode) /* if tail node already allocated */
   {
      /* insert the node like this [list->headNode, ..., list->tailNode, node] */
      List_InsertBetween(list, node, list->tailNode, list->headNode);

      /* update the list, the tail is the new node */
      list->tailNode = node;
   }
   else /* if tail node is empty (1st time appending) */
   {
      List_AddFirstNode(list, node);
   }
}

void List_Prepend(List_t* list, List_Node_t* node)
{
   if (list->headNode) /* if head node already allocated */
   {
      /* insert the node like this [node, list->headNode, ..., list->tailNode] */
      List_InsertBetween(list, node, list->tailNode, list->headNode);

      /* update the list, the head is the new node */
      list->headNode = node;
   }
   else /* if head node is empty (1st time prepending) */
   {
      List_AddFirstNode(list, node);
   }
}

void List_InsertBefore(List_t* list, List_Node_t* node, List_Node_t* before)
{
   if (list->headNode && list->tailNode) /* if list contains some nodes (not 1st time inserting) */
   {
      /* insert the node like this [before->previousNode, node, before] */
      List_InsertBetween(list, node, before->previousNode, before);

      if (before == list->headNode) /* if inserting before head node */
      {
         /* update the list, head node is now the new node */
         list->headNode = node;
      }
      else /* if inserting before any other node (other than head) */
      {

      }
   }
   else /* if 1st time inserting */
   {
      List_AddFirstNode(list, node);
   }
}

void List_InsertAfter(List_t* list, List_Node_t* node, List_Node_t* after)
{
   if (list->headNode && list->tailNode) /* if list contains some nodes (not 1st time inserting) */
   {
      /* insert the node like this [after, node, after->nextNode] */
      List_InsertBetween(list, node, after, after->nextNode);

      if (after == list->tailNode) /* if inserting after tail node */
      {
         /* update the list, tail node is now the new node */
         list->tailNode = node;
      }
      else /* if inserting after any other node (other than tail) */
      {

      }
   }
   else /* if 1st time inserting */
   {
      List_AddFirstNode(list, node);
   }
}

void List_Remove(List_t* list, List_Node_t* node)
{
   // TODO: search for node in list for safety and don't blatantly assume it's there

   /* this determines whether we found a valid node or not (unsafe solution currently) */
   u8 isExist = 0;

   if (node == list->headNode) /* if we'll remove the head */
   {
      /* update the list, the head node is now what was after it */
      list->headNode = list->headNode->nextNode;

      /* update the circular connectors:
       * the previous node of the new head is the tail,
       * also the next node of the tail is the new head */
      list->headNode->previousNode = list->tailNode;
      list->tailNode->nextNode = list->headNode;

      /* declare that we found a valid node */
      isExist = 1;
   }
   else if (node == list->tailNode) /* if we'll remove the tail */
   {
      /* update the list, the tail node is now what was before it */
      list->tailNode = list->tailNode->previousNode;

      /* update the circular connectors:
       * the previous node of the head is the new tail,
       * also the next node of the new tail is the head */
      list->headNode->previousNode = list->tailNode;
      list->tailNode->nextNode = list->headNode;

      /* declare that we found a valid node */
      isExist = 1;
   }
   else if (node->previousNode && node->nextNode) /* if this node is not deleted/removed before */
   {
      /* connect the surrounding nodes together:
       * before connecting: [previous, node, next]
       * after connecting:  [previous, next] */
      node->previousNode->nextNode = node->nextNode;
      node->nextNode->previousNode = node->previousNode;

      /* declare that we found a valid node (here it's unsafe and blatant) */
      isExist = 1;
   }

   if (isExist) /* if we found a valid node */
   {
      /* reset the node connectors */
      node->previousNode = 0;
      node->nextNode = 0;

      /* decrement the nodes count*/
      list->nodesCount--;

      if (list->nodesCount == 0) /* if we reached zero nodes */
      {
         /* reset the list head and node */
         list->headNode = 0;
         list->tailNode = 0;
      }
      else /* if we didn't reach zero nodes */
      {

      }
   }
   else /* if we didn't find a valid node */
   {

   }
}

void List_Sort(List_t* list, u8 (*conditionCallback)(const List_Node_t* nextNode, const List_Node_t* fittingNode))
{
   /* the most fitting node that satisfies the swapping condition (per iteration) */
   List_Node_t* conditionedNode;

   /* foreach node in the list */
   for (List_Node_t* currentNode = list->headNode; currentNode != list->tailNode; currentNode = currentNode->nextNode)
   {
      /* assume the most fitting node for the condition is the current one */
      conditionedNode = currentNode;

      /* foreach node in the list after the current one (until we reach the head again) */
      for (List_Node_t* nextNode = currentNode->nextNode; nextNode != list->headNode; nextNode = nextNode->nextNode)
      {
         if (conditionCallback(nextNode, conditionedNode)) /* if nextNode is more fitting for the condition than conditionedNode */
         {
            /* assign the newly-found fitting node to the conditionedNode */
            conditionedNode = nextNode;
         }
         else /* if conditionedNode is the most fitting node for the condition */
         {

         }
      }

      if (conditionedNode != currentNode) /* if a more fitting node was found */
      {
         /* disconnect the most fitting node from the list */
         List_Remove(list, conditionedNode);

         /* then add it again before the current node */
         List_InsertBefore(list, conditionedNode, currentNode);

         /* and point at the newly-placed fitting node since it's now in-place of (old) currentNode */
         currentNode = conditionedNode;
      }
      else /* if the current node was the most fitting one for the condition */
      {

      }
   }
}


static void List_AddFirstNode(List_t* list, List_Node_t* node)
{
   /* both the head and the tail point at the new node */
   list->headNode = node;
   list->tailNode = node;

   /* also the node points at itself (circular) */
   node->previousNode = node;
   node->nextNode = node;

   /* set nodes count to 1 */
   list->nodesCount = 1;
}

static void List_InsertBetween(List_t* list, List_Node_t* node, List_Node_t* beforeNode, List_Node_t* afterNode)
{
   /* insert the node like this:
    * before: [before, after]
    * after:  [before, node, after] */

    /* attach the connectors of the surrounding nodes (after and before) to the new node */
   beforeNode->nextNode = node;
   afterNode->previousNode = node;

   /* attach the connectors of the new node to the surrounding ones */
   node->nextNode = afterNode;
   node->previousNode = beforeNode;

   /* increment the nodes count */
   list->nodesCount++;
}
