#ifndef _MAZE_H
#define _MAZE_H

#include <Windows.h>
#include "MazeColor.h"
#include "MazeCell.h"
#include "MazeCellStack.h"

enum MazeState
{
    MS_INITIALIZED,     // 方格状
    MS_GENERATING,      // 生成中
    MS_GENERATED,       // 已生成
    MS_SETTING,         // 设定中
    MS_SET,             // 已设定
    MS_RESOLVING,       // 求解中
    MS_RESOLVED,        // 已求解完毕
    MS_RESOLVEFAILED,   // 已求解错误
    MS_PAUSED,          // 已暂停
    MS_ERROR,           // 已错误
};

class CMaze
{
public:
    CMaze(int nWidth, int nHeight, int nCellWidth, int nCellHeight, bool bCopyScreen);
    ~CMaze(void);

public:
    void Next(BOOL bDelay);
    HDC GetDc() const;
    CMazeColor &GetColorManager();
    CMazeCellStack &GetCellStack();
    void Restart();

public:
    MazeState GetState() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetCellWidth() const;
    int GetCellHeight() const;
    int GetRows() const;
    int GetCols() const;
    int GetOffsetX() const;
    int GetOffsetY() const;
    CMazeCell *GetCell(int i, int j);
    void ReDrawAllCells();
    void ResetAllCells();
    void ResetAndReDrawAllCells();
    void ReDrawSides();
    static COLORREF GetReverseColor(COLORREF cl);
    void SetReDrawBeforeGenerate(BOOL bReDrawBeforeGenerate);
    void SetFlashPath(BOOL bFlashPath);
    void UpdateColors();

private:
    void InitializeCells();
    CMazeCell *GetRandomCell();
    void MakeRandomBeginAndEndCell();
    void ChangeState(MazeState state);
    int GetCounter();

private:
    int m_nWidth;
    int m_nHeight;
    int m_nCellWidth;
    int m_nCellHeight;
    int m_nRows;
    int m_nCols;
    int m_nOffsetX;
    int m_nOffsetY;

    BOOL m_bReDrawBeforeGenerate;
    BOOL m_bFlashPath;

    HDC m_hWorkDc;
    HBITMAP m_hWorkBmp;
    HGDIOBJ m_hOldBmp;
    HGDIOBJ m_hOldPen;

    MazeState m_state;
    MazeState m_stateOld;

    CMazeCellStack m_cellStack;
    CMazeCell *m_cells;
    CMazeCell *m_cellBegin;
    CMazeCell *m_cellEnd;
    CMazeCell *m_cellGenerating;

    CMazeColor m_mazeColor;

    int m_nCounter;
};

#endif /* _MAZE_H */