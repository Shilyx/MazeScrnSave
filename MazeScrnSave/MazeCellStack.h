#ifndef _MAZECELLSTACK_H
#define _MAZECELLSTACK_H

class CMazeCell;

struct StackNode
{
    CMazeCell* Cell;
    StackNode* Next;
};

class CMazeCellStack  
{
public:
    CMazeCellStack();
//     CMazeCellStack(const CMazeCellStack&);
    ~CMazeCellStack();

    void Push(CMazeCell*);						// Ñ¹Õ»
    void Pop();									// ³öÕ»
    CMazeCell* Top() const;						// Õ»¶¥
    bool IsEmpty();								// ¿Õ£¿
    bool IsFull();								// Âú£¿
    void Init();								// Ïú»ÙÖØ½¨

private:
    StackNode* top;								// Õ»¶¥Ö¸Õë		
};


#endif /* _MAZECELLSTACK_H */