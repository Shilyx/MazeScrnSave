#ifndef _MAZECELL_H
#define _MAZECELL_H

#include <Windows.h>

enum Direction
{
    DIR_LEFT,
    DIR_TOP,
    DIR_RIGHT,
    DIR_BOTTOM,
    DIR_TOTAL,
};

class CMaze;

class CMazeCell
{
    friend CMaze;

public:
    CMazeCell(void);
    ~CMazeCell(void);

public:
    void GoGenerate(bool *pGetNewCell = NULL);
    bool CanGoGenerate(Direction dir);
    void GoResolve(bool *pGetNewCell = NULL);
    bool CanGoResolve(Direction dir);

public:
    void SetParent(CMaze *pMaze, int x, int y);       // 构造后必须调用
    void Reset();                        // 重置
    void ReDraw();
    void FillSolidRect(HDC hDc, RECT *pRect, COLORREF color);
    Direction GetRandomDirection() const;
    Direction GetNextDirection(Direction dir) const;
    CMazeCell *GetCell(Direction dir);
    bool GetWall(Direction dir);
    void SetWall(Direction dir, bool bWall);

private:
    CMaze *m_pMaze;
    int m_x;
    int m_y;
    RECT m_rectCell;

    bool m_bRightWall;
    bool m_bBottomWall;
    bool m_bIsBegin;
    bool m_bIsEnd;
    bool m_bVisited;
    bool m_bDied;
    bool m_bGenerated;
    CMazeCell *m_pPrevCell;

    int m_nDirectionAddtion;
};

#endif /* _MAZECELL_H */