#ifndef LIST_H
#define LIST_H

#define LIST_ERR_OK  0
#define LIST_ERR_NOK 1

typedef struct List_Node_t
{
   struct List_Node_t* previousNode;
   struct List_Node_t* nextNode;
} List_Node_t;

typedef struct List_t
{
   List_Node_t* headNode;
   List_Node_t* tailNode;
   u16 nodesCount;
} List_t;

void List_Init(List_t* list);

void List_Append(List_t* list, List_Node_t* node);
void List_Prepend(List_t* list, List_Node_t* node);

void List_InsertBefore(List_t* list, List_Node_t* node, List_Node_t* before);
void List_InsertAfter(List_t* list, List_Node_t* node, List_Node_t* after);

void List_Remove(List_t* list, List_Node_t* node);

void List_Sort(List_t* list, u8 (*conditionCallback)(const List_Node_t* nextNode, const List_Node_t* fittingNode));

#endif /* LIST_H */
