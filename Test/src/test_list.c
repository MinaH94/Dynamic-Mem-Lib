#include "STD_TYPES.h"
#include "List.h"
#include <diag/Trace.h>


typedef struct
{
   List_Node_t listNode;
   void* data;
} TestNode_t;

List_t list;
TestNode_t n[6];

static u8 sortListAscendingByNodeAddrCallback(const List_Node_t* nextNode, const List_Node_t* fittingNode);
static void displayList(List_t* list);


void TEST_list(void)
{
   n[0].data = (void*)"node 1";
   n[1].data = (void*)"node 2";
   n[2].data = (void*)"node 3";
   n[3].data = (void*)"node 4";
   n[4].data = (void*)"node 5";
   n[5].data = (void*)"node 6";

   List_Init(&list);

   for (int i = 0; i < 6; i++)
   {
      List_Append(&list, &n[i].listNode);
   }

   displayList(&list);

   List_Remove(&list, &n[2].listNode); // "node 3"

   displayList(&list);

   for (int i = 0; i < 6; i++)
   {
      List_Remove(&list, &n[i].listNode);
   }



   for (int i = 0; i < 6; i++)
   {
      List_Prepend(&list, &n[i].listNode);
   }

   displayList(&list);

   List_Remove(&list, &n[2].listNode); // "node 3"

   displayList(&list);

   for (int i = 0; i < 6; i++)
   {
      List_Remove(&list, &n[i].listNode);
   }



   for (int i = 0; i < 6; i++)
   {
      List_InsertAfter(&list, &n[i].listNode, list.tailNode);
   }

   displayList(&list);

   List_Remove(&list, &n[2].listNode); // "node 3"

   displayList(&list);

   for (int i = 0; i < 6; i++)
   {
      List_Remove(&list, &n[i].listNode);
   }



   for (int i = 0; i < 6; i++)
   {
      List_InsertAfter(&list, &n[i].listNode, list.headNode);
   }

   displayList(&list);

   List_Remove(&list, &n[2].listNode); // "node 3"

   displayList(&list);

   for (int i = 0; i < 6; i++)
   {
      List_Remove(&list, &n[i].listNode);
   }


   for (int i = 0; i < 6; i++)
   {
      List_InsertBefore(&list, &n[i].listNode, list.headNode);
   }

   displayList(&list);

   List_Remove(&list, &n[2].listNode); // "node 3"

   displayList(&list);

   for (int i = 0; i < 6; i++)
   {
      List_Remove(&list, &n[i].listNode);
   }



   for (int i = 0; i < 6; i++)
   {
      List_InsertBefore(&list, &n[i].listNode, list.tailNode);
   }

   displayList(&list);

   List_Remove(&list, &n[2].listNode); // "node 3"

   displayList(&list);

   for (int i = 0; i < 6; i++)
   {
      List_Remove(&list, &n[i].listNode);
   }

   List_Append(&list, &n[5].listNode);

   List_Append(&list, &n[3].listNode); //
   List_Append(&list, &n[4].listNode);
   List_Append(&list, &n[1].listNode); //
   List_Append(&list, &n[2].listNode); //

   List_Append(&list, &n[0].listNode); //

   displayList(&list);

   List_Sort(&list, sortListAscendingByNodeAddrCallback);

   displayList(&list);
}


static u8 sortListAscendingByNodeAddrCallback(const List_Node_t* nextNode, const List_Node_t* fittingNode)
{
   return (nextNode < fittingNode);
}

static void displayList(List_t* list)
{
   TestNode_t* test;
   List_Node_t* lst = list->headNode;

   for (u8 i = 0; i < list->nodesCount; i++)
   {
      test = (TestNode_t*)lst;
      trace_printf("%s\n", (char*)test->data);

      lst = lst->nextNode;
   }

   trace_printf("\n");
}
