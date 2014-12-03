#ifndef _MAZECOLOR_H
#define _MAZECOLOR_H

#include <Windows.h>

enum ColorType
{
    clBackGround,   // 背景
    clWall,         // 墙色
    clBadPath,      // 不可用的路径
    clVisited,      // 正在使用的路径
    clPtStart,      // 开始点
    clPtEnd,        // 结束点
    clCount,        // 颜色种类
};

class CMazeColor
{
public:
    CMazeColor();
    ~CMazeColor();

    HPEN GetPen(ColorType ct);
    HBRUSH GetBrush(ColorType ct);
    COLORREF GetColor(ColorType ct) const;
    COLORREF SetColor(ColorType ct, COLORREF color);

private:
    COLORREF m_colors[clCount];
    HPEN m_pens[clCount];
    HBRUSH m_brushs[clCount];
};

#endif /* _MAZECOLOR_H */