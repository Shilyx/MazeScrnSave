#include "MazeCell.h"
#include "Maze.h"

CMazeCell::CMazeCell(void)
{
    Reset();
}

CMazeCell::~CMazeCell(void)
{
}

void CMazeCell::SetParent(CMaze *pMaze, int x, int y)
{
    m_pMaze = pMaze;
    m_x = x;
    m_y = y;

    m_rectCell.left = m_pMaze->GetOffsetX() + m_x * m_pMaze->GetCellWidth();
    m_rectCell.top = m_pMaze->GetOffsetY() + m_y * m_pMaze->GetCellHeight();
    m_rectCell.right = m_rectCell.left + m_pMaze->GetCellWidth();
    m_rectCell.bottom = m_rectCell.top + m_pMaze->GetCellHeight();
}

void CMazeCell::Reset()
{
    m_bRightWall = true;
    m_bBottomWall = true;
    m_bGenerated = false;
    m_bVisited = false;
    m_bDied = false;
    m_bIsBegin = false;
    m_bIsEnd = false;
    m_pPrevCell = NULL;
    m_nDirectionAddtion = 1;

#if 1
    if (rand() % 2 == 0)
    {
        m_nDirectionAddtion = -1;
    }
#else
    if ((m_x + m_y) % 2 == 0)
    {
        m_nDirectionAddtion = -1;
    }
#endif
}

void CMazeCell::ReDraw()
{
    HDC hDc = m_pMaze->GetDc();
    RECT rect = m_rectCell;
    int nInflateValueX = 0;
    int nInflateValueY = 0;

    // 擦除
    if (!m_bVisited && !m_bDied)
    {
        OffsetRect(&rect, 1, 1);
        FillSolidRect(hDc, &rect, m_pMaze->GetColorManager().GetColor(clBackGround));
    }

    // 画墙
    rect = m_rectCell;

    if (m_bRightWall)
    {
        POINT ptSrc = {(1 + m_x) * m_pMaze->GetCellWidth() + m_pMaze->GetOffsetX(), (1 + m_y) * m_pMaze->GetCellHeight() + m_pMaze->GetOffsetY()};
        POINT ptDst = {ptSrc.x, m_y * m_pMaze->GetCellHeight() + m_pMaze->GetOffsetY()};

        MoveToEx(hDc, ptSrc.x, ptSrc.y, NULL);
        LineTo(hDc, ptDst.x, ptDst.y);
    }

    if (m_bBottomWall)
    {
        POINT ptSrc = {(1 + m_x) * m_pMaze->GetCellWidth() + m_pMaze->GetOffsetX(), (1 + m_y) * m_pMaze->GetCellHeight() + m_pMaze->GetOffsetY()};
        POINT ptDst = {m_x * m_pMaze->GetCellWidth() + m_pMaze->GetOffsetX(), ptSrc.y};

        MoveToEx(hDc, ptSrc.x, ptSrc.y, NULL);
        LineTo(hDc, ptDst.x, ptDst.y);
    }

    // 绘制访问和死亡
    if (m_pPrevCell != NULL)
    {
        rect = m_rectCell;
        nInflateValueX = m_pMaze->GetCellWidth() / -5;
        nInflateValueY = m_pMaze->GetCellHeight() / -5;

        if (nInflateValueX == 0)
        {
            nInflateValueX = -1;
        }

        if (nInflateValueY == 0)
        {
            nInflateValueY = -1;
        }

        InflateRect(&rect, nInflateValueX, nInflateValueY);

        if (m_bDied)
        {
            RECT rectConnection = rect;
            RECT rectPrev = m_pPrevCell->m_rectCell;

            InflateRect(&rectPrev, nInflateValueX, nInflateValueY);
            UnionRect(&rectConnection, &rect, &rectPrev);
            SubtractRect(&rectConnection, &rectConnection, &rectPrev);

            FillSolidRect(hDc, &rectConnection, m_pMaze->GetColorManager().GetColor(clBadPath));

            if (m_pPrevCell != NULL && (m_pPrevCell->m_bIsBegin || m_pPrevCell->m_bIsEnd))
            {
                m_pPrevCell->ReDraw();
            }
        }
        else if (m_bVisited)
        {
            RECT rectConnection = rect;
            RECT rectPrev = m_pPrevCell->m_rectCell;

            InflateRect(&rectPrev, nInflateValueX, nInflateValueY);
            UnionRect(&rectConnection, &rect, &rectPrev);

            FillSolidRect(hDc, &rectConnection, m_pMaze->GetColorManager().GetColor(clVisited));

            if (m_pPrevCell != NULL && (m_pPrevCell->m_bIsBegin || m_pPrevCell->m_bIsEnd))
            {
                m_pPrevCell->ReDraw();
            }
        }
    }

    // 绘制开始和结束
    if (m_bIsBegin || m_bIsEnd)
    {
        rect = m_rectCell;
        nInflateValueX = m_pMaze->GetCellWidth() / -10;
        nInflateValueY = m_pMaze->GetCellHeight() / -10;

        if (nInflateValueX == 0)
        {
            nInflateValueX = -1;
        }

        if (nInflateValueY == 0)
        {
            nInflateValueY = -1;
        }

        InflateRect(&rect, nInflateValueX, nInflateValueY);

        if (m_bIsBegin)
        {
            FillSolidRect(hDc, &rect, m_pMaze->GetColorManager().GetColor(clPtStart));
        }

        if (m_bIsEnd)
        {
            FillSolidRect(hDc, &rect, m_pMaze->GetColorManager().GetColor(clPtEnd));
        }
    }
}

void CMazeCell::FillSolidRect(HDC hDc, RECT *pRect, COLORREF color)
{
    SetBkColor(hDc, color);
    ExtTextOut(hDc, 0, 0, ETO_OPAQUE, pRect, NULL, 0, NULL);
}

bool CMazeCell::CanGoGenerate(Direction dir)
{
    CMazeCell *pCell = GetCell(dir);

    if (pCell == NULL)
    {
        return false;
    }

    if (pCell->m_bGenerated)
    {
        return false;
    }

    return true;
}

void CMazeCell::GoGenerate(bool *pGetNewCell)
{
    if (pGetNewCell != NULL)
    {
        *pGetNewCell = false;
    }

    BOOL bFoundDirection = FALSE;
    Direction dir = GetRandomDirection();

    for (int i = 0; i < DIR_TOTAL; ++i)
    {
        if (CanGoGenerate(dir))
        {
            bFoundDirection = TRUE;
            break;
        }

        dir = GetNextDirection(dir);
    }

    if (!bFoundDirection)
    {
        m_pMaze->GetCellStack().Pop();
    }
    else
    {
        CMazeCell *pCell = GetCell(dir);

        SetWall(dir, false);
        pCell->m_bGenerated = true;
        m_pMaze->GetCellStack().Push(pCell);

        if (pGetNewCell != NULL)
        {
            *pGetNewCell = true;
        }
    }
}

bool CMazeCell::CanGoResolve(Direction dir)
{
    CMazeCell *pCell = GetCell(dir);

    if (pCell == NULL)
    {
        return false;
    }

    if (pCell->m_bVisited)
    {
        return false;
    }

    if (pCell->m_bDied)
    {
        return false;
    }

    return !GetWall(dir);
}

void CMazeCell::GoResolve(bool *pGetNewCell)
{
    if (pGetNewCell != NULL)
    {
        *pGetNewCell = false;
    }

    BOOL bFoundDirection = FALSE;
    Direction dir = GetRandomDirection();

    for (int i = 0; i < DIR_TOTAL; ++i)
    {
        if (CanGoResolve(dir))
        {
            bFoundDirection = TRUE;
            break;
        }

        dir = GetNextDirection(dir);
    }

    if (!bFoundDirection)
    {
        m_bDied = true;
        ReDraw();
        m_pMaze->GetCellStack().Pop();
    }
    else
    {
        CMazeCell *pCell = GetCell(dir);

        pCell->m_bVisited = true;
        pCell->m_pPrevCell = this;
        m_pMaze->GetCellStack().Push(pCell);

        if (pGetNewCell != NULL)
        {
            *pGetNewCell = true;
        }
    }
}

Direction CMazeCell::GetRandomDirection() const
{
    return (Direction)(rand() % DIR_TOTAL);
}

Direction CMazeCell::GetNextDirection(Direction dir) const
{
    return (Direction)((dir + DIR_TOTAL + m_nDirectionAddtion) % DIR_TOTAL);
}

CMazeCell *CMazeCell::GetCell(Direction dir)
{
    switch (dir)
    {
    case DIR_LEFT:
        return m_pMaze->GetCell(m_x - 1, m_y);

    case DIR_TOP:
        return m_pMaze->GetCell(m_x, m_y - 1);

    case DIR_RIGHT:
        return m_pMaze->GetCell(m_x + 1, m_y);

    case DIR_BOTTOM:
        return m_pMaze->GetCell(m_x, m_y + 1);

    default:
        return NULL;
    }
}

bool CMazeCell::GetWall(Direction dir)
{
    CMazeCell *pCell = GetCell(dir);

    switch (dir)
    {
    case DIR_LEFT:
        if (pCell != NULL)
        {
            return pCell->m_bRightWall;
        }
        break;

    case DIR_TOP:
        if (pCell != NULL)
        {
            return pCell->m_bBottomWall;
        }
        break;

    case DIR_RIGHT:
        return m_bRightWall;

    case DIR_BOTTOM:
        return m_bBottomWall;

    default:
        break;
    }

    return true;
}

void CMazeCell::SetWall(Direction dir, bool bWall)
{
    CMazeCell *pCell = GetCell(dir);

    switch (dir)
    {
    case DIR_LEFT:
        if (pCell != NULL)
        {
            pCell->m_bRightWall = bWall;
        }
        break;

    case DIR_TOP:
        if (pCell != NULL)
        {
            pCell->m_bBottomWall = bWall;
        }
        break;

    case DIR_RIGHT:
        m_bRightWall = bWall;
        break;

    case DIR_BOTTOM:
        m_bBottomWall = bWall;
        break;

    default:
        break;
    }
}
