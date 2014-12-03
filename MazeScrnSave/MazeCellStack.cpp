#include "MazeCellStack.h"
#include <stdlib.h>

CMazeCellStack::CMazeCellStack()
{
    top = NULL;
}

// CMazeCellStack::CMazeCellStack(const CMazeCellStack& Stack)
// {
//     if (!Stack.IsEmpty())
//     {
//         top = new StackNode;
//         top->Cell = Stack.top->Cell;
//         StackNode *tmp = top, *tmp2, *p = Stack.top->Next;
//         while (p!=NULL)
//         {
//             tmp2 = new StackNode;
//             tmp2->Cell = p->Cell;
//             tmp->Next = tmp2;
//             tmp = tmp2;
//             p = p->Next;
//         }
//         tmp->Next = NULL;
//     }
// }

CMazeCellStack::~CMazeCellStack()
{
    Init();
}

void CMazeCellStack::Push(CMazeCell* Cell)
{
    StackNode* NewNode = new StackNode;
    NewNode->Cell = Cell;
    NewNode->Next = top;
    top = NewNode;
}

void CMazeCellStack::Pop()
{
    if (!IsEmpty())
    {
        StackNode* tmp = top;
        top = top->Next;
        delete tmp;
    }
}

CMazeCell* CMazeCellStack::Top() const
{
    if (top != NULL)
    {
        return top->Cell;
    }

    return NULL;
}

bool CMazeCellStack::IsEmpty()
{
    return top == NULL;
}

bool CMazeCellStack::IsFull()
{
    return false;
}

void CMazeCellStack::Init()
{
    while (!IsEmpty())
    {
        Pop();
    }
}
