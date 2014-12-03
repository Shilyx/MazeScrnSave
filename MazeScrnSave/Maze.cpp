#include "Maze.h"

CMaze::CMaze(int nWidth, int nHeight, int nCellWidth, int nCellHeight, bool bCopyScreen)
{
    m_nCounter = 0;

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nCellWidth = nCellWidth;
    m_nCellHeight = nCellHeight;
    m_nCols = m_nWidth / m_nCellWidth - 1;
    m_nRows = m_nHeight / m_nCellHeight - 1;
    m_nOffsetX = (m_nWidth - m_nCols * m_nCellWidth) / 2;
    m_nOffsetY = (m_nHeight - m_nRows * m_nCellHeight) / 2;

    m_bReDrawBeforeGenerate = FALSE;
    m_bFlashPath = TRUE;

//     m_nOffsetX = 2;
//     m_nOffsetY = 2;

    HDC hScreenDc = GetDC(NULL);

    m_hWorkDc = CreateCompatibleDC(NULL);
    m_hWorkBmp = CreateCompatibleBitmap(hScreenDc, m_nWidth, m_nHeight);
    m_hOldBmp = SelectObject(m_hWorkDc, (HGDIOBJ)m_hWorkBmp);
    m_hOldPen = SelectObject(m_hWorkDc, m_mazeColor.GetPen(clWall));

    if (bCopyScreen)
    {
        BitBlt(m_hWorkDc, 0, 0, m_nWidth, m_nHeight, hScreenDc, 0, 0, SRCCOPY);
    }

    ReleaseDC(NULL, hScreenDc);

    RECT rect = {0, 0, m_nWidth, m_nHeight};
    FillRect(m_hWorkDc, &rect, m_mazeColor.GetBrush(clBackGround));

    if (m_nCols > 0 && m_nRows > 0)
    {
        m_cells = new CMazeCell[m_nCols * m_nRows];

        InitializeCells();
        ResetAndReDrawAllCells();
        m_state = MS_INITIALIZED;
    }
    else
    {
        m_state = MS_ERROR;
        m_cells = NULL;
    }
}

CMaze::~CMaze(void)
{
    SelectObject(m_hWorkDc, m_hOldPen);
    SelectObject(m_hWorkDc, m_hOldBmp);
    DeleteDC(m_hWorkDc);
    DeleteObject(m_hWorkBmp);

    if (m_cells != NULL)
    {
        delete []m_cells;
    }
}

CMazeCell *CMaze::GetCell(int i, int j)
{
    if (i < 0 || j < 0 || i >= m_nCols || j >= m_nRows)
    {
        return NULL;
    }

    return &m_cells[j * m_nCols + i];
}

void CMaze::InitializeCells()
{
    for (int i = 0; i < m_nCols; ++i)
    {
        for (int j = 0; j < m_nRows; ++j)
        {
            CMazeCell *pCell = GetCell(i, j);

            pCell->SetParent(this, i, j);
        }
    }
}

void CMaze::ReDrawSides()
{
    int nPicWidth = m_nCols * m_nCellWidth;
    int nPicHeight = m_nRows * m_nCellHeight;

    MoveToEx(m_hWorkDc, m_nOffsetX, m_nOffsetY, NULL);
    LineTo(m_hWorkDc, m_nOffsetX + nPicWidth, m_nOffsetY);
    LineTo(m_hWorkDc, m_nOffsetX + nPicWidth, m_nOffsetY + nPicHeight);
    LineTo(m_hWorkDc, m_nOffsetX, m_nOffsetY + nPicHeight);
    LineTo(m_hWorkDc, m_nOffsetX, m_nOffsetY);
}

void CMaze::ResetAllCells()
{
    for (int i = 0; i < m_nCols; ++i)
    {
        for (int j = 0; j < m_nRows; ++j)
        {
            CMazeCell *pCell = GetCell(i, j);

            pCell->Reset();
        }
    }
}

void CMaze::ResetAndReDrawAllCells()
{
    for (int i = 0; i < m_nCols; ++i)
    {
        for (int j = 0; j < m_nRows; ++j)
        {
            CMazeCell *pCell = GetCell(i, j);

            pCell->Reset();
            pCell->ReDraw();
        }
    }

    ReDrawSides();
}

void CMaze::ReDrawAllCells()
{
    for (int i = 0; i < m_nCols; ++i)
    {
        for (int j = 0; j < m_nRows; ++j)
        {
            CMazeCell *pCell = GetCell(i, j);

            pCell->ReDraw();
        }
    }

    ReDrawSides();
}

void CMaze::Next(BOOL bDelay)
{
    DWORD dwDelayValue = 0;

    switch (m_state)
    {
    case MS_INITIALIZED:
        m_cellGenerating = GetRandomCell();
        m_cellStack.Init();
        m_cellStack.Push(m_cellGenerating);
        m_cellGenerating->m_bGenerated = true;
        ChangeState(MS_GENERATING);
        break;

    case MS_GENERATING:
        if (m_cellStack.IsEmpty())
        {
            ChangeState(MS_GENERATED);
        }
        else
        {
            bool bGetNewCell = false;
            CMazeCell *pTopCell = m_cellStack.Top();

            pTopCell->ReDraw();
            pTopCell->GoGenerate(&bGetNewCell);

            if (bGetNewCell)
            {
                //dwDelayValue = 1;
            }
        }
        break;

    case MS_GENERATED:
        ReDrawAllCells();
        ChangeState(MS_SETTING);
        break;

    case MS_SETTING:
        if (true)
        {
            int nCounter = GetCounter();

            if (nCounter == 0)
            {
                if (rand() % 5 == 0)
                {
                    int bx[] = {0, 0, 1, 1};
                    int by[] = {0, 1, 0, 1};
                    int ex[] = {1, 1, 0, 0};
                    int ey[] = {1, 0, 1, 0};
                    int offset = rand() % 4;

                    m_cellBegin = GetCell((GetCols() - 1) * bx[offset], (GetRows() - 1) * by[offset]);
                    m_cellEnd = GetCell((GetCols() - 1) * ex[offset], (GetRows() - 1) * ey[offset]);
                }
                else
                {
                    m_cellBegin = GetRandomCell();

                    do
                    {
                        m_cellEnd = GetRandomCell();

                    } while (m_cellEnd == m_cellBegin);
                }

                m_cellBegin->m_bIsBegin = true;
                m_cellEnd->m_bIsEnd = true;
            }
            else if (nCounter < 19)
            {
                m_cellBegin->m_bIsBegin = !m_cellBegin->m_bIsBegin;
                m_cellBegin->ReDraw();
                dwDelayValue = 55;
            }
            else if (nCounter < 37)
            {
                m_cellEnd->m_bIsEnd = !m_cellEnd->m_bIsEnd;
                m_cellEnd->ReDraw();
                dwDelayValue = 55;
            }
            else
            {
                ChangeState(MS_SET);
            }
        }
        break;

    case MS_SET:
        m_cellStack.Init();
        m_cellStack.Push(m_cellBegin);
        m_cellBegin->m_bVisited = true;
        ChangeState(MS_RESOLVING);
        break;

    case MS_RESOLVING:
        if (m_cellStack.IsEmpty())
        {
            ChangeState(MS_RESOLVEFAILED);
        }
        else
        {
            bool bGetNewCell = false;
            CMazeCell *pTopCell = m_cellStack.Top();

            pTopCell->ReDraw();

            if (pTopCell == m_cellEnd)
            {
                ChangeState(MS_RESOLVED);
                break;
            }

            pTopCell->GoResolve(&bGetNewCell);

            if (bGetNewCell)
            {
                //dwDelayValue = 1;
            }
        }
        dwDelayValue = 10;
        break;

    case MS_RESOLVED:
        {
            int nCounter = GetCounter();

            if (nCounter > 5)
            {
                if (m_bReDrawBeforeGenerate)
                {
                    ResetAndReDrawAllCells();
                }
                else
                {
                    ResetAllCells();
                }

                ChangeState(MS_INITIALIZED);
            }
            else
            {
                if (m_bFlashPath)
                {
                    COLORREF cl = m_mazeColor.GetColor(clVisited);

                    m_mazeColor.SetColor(clVisited, GetReverseColor(cl));

                    ReDrawAllCells();
                }

                dwDelayValue = 500;
            }
        }
        break;

    case MS_RESOLVEFAILED:
        dwDelayValue = 3000;
        if (m_bReDrawBeforeGenerate)
        {
            ResetAndReDrawAllCells();
        }
        else
        {
            ResetAllCells();
        }
        ChangeState(MS_INITIALIZED);

    case MS_PAUSED:
        break;

    case MS_ERROR:
        break;

    default:
        break;
    }

    if (bDelay && dwDelayValue > 0)
    {
        Sleep(dwDelayValue);
    }
}

HDC CMaze::GetDc() const
{
    return m_hWorkDc;
}

CMazeColor &CMaze::GetColorManager()
{
    return m_mazeColor;
}

MazeState CMaze::GetState() const
{
    return m_state;
}

CMazeCell *CMaze::GetRandomCell()
{
    return GetCell(rand() % m_nCols, rand() % m_nRows);
}

void CMaze::MakeRandomBeginAndEndCell()
{
    m_cellBegin = GetRandomCell();
    m_cellEnd = GetRandomCell();

    while (m_cellEnd == m_cellBegin)
    {
        m_cellEnd = GetRandomCell();
    }
}

int CMaze::GetWidth() const
{
    return m_nWidth;
}

int CMaze::GetHeight() const
{
    return m_nHeight;
}

int CMaze::GetCellWidth() const
{
    return m_nCellWidth;
}

int CMaze::GetCellHeight() const
{
    return m_nCellHeight;
}

int CMaze::GetRows() const
{
    return m_nRows;
}

int CMaze::GetCols() const
{
    return m_nCols;
}

int CMaze::GetOffsetX() const
{
    return m_nOffsetX;
}

int CMaze::GetOffsetY() const
{
    return m_nOffsetY;
}

CMazeCellStack &CMaze::GetCellStack()
{
    return m_cellStack;
}

void CMaze::ChangeState(MazeState state)
{
    if (m_state != state)
    {
        m_state = state;
        m_nCounter = 0;
    }
}

int CMaze::GetCounter()
{
    return m_nCounter++;
}

COLORREF CMaze::GetReverseColor(COLORREF cl)
{
    return RGB(~GetRValue(cl), ~GetGValue(cl), ~GetBValue(cl));
}

void CMaze::SetReDrawBeforeGenerate(BOOL bReDrawBeforeGenerate)
{
    m_bReDrawBeforeGenerate = bReDrawBeforeGenerate;
}

void CMaze::SetFlashPath(BOOL bFlashPath)
{
    m_bFlashPath = bFlashPath;
}

void CMaze::UpdateColors()
{
    SelectObject(m_hWorkDc, m_mazeColor.GetPen(clWall));    
}

void CMaze::Restart()
{
    if (m_bReDrawBeforeGenerate)
    {
        ResetAndReDrawAllCells();
    }
    else
    {
        ResetAllCells();
    }

    ChangeState(MS_INITIALIZED);
}
