#include "MazeColor.h"

CMazeColor::CMazeColor(void)
{
    m_colors[clBackGround] = GetSysColor(COLOR_BTNFACE);
    m_colors[clWall] = RGB(0, 0, 0);
    m_colors[clBadPath] = RGB(155, 155, 155);
    m_colors[clVisited] = RGB(0, 0, 255);
    m_colors[clPtStart] = RGB(0, 0, 0);
    m_colors[clPtEnd] = RGB(255, 0, 0);

    for (int i = 0; i < clCount; i += 1)
    {
        m_pens[i] = NULL;
        m_brushs[i] = NULL;
    }
}

CMazeColor::~CMazeColor()
{
    for (int i = 0; i < clCount; i += 1)
    {
        if (m_pens[i] != NULL)
        {
            DeleteObject(m_pens[i]);
        }

        if (m_brushs[i] != NULL)
        {
            DeleteObject(m_brushs[i]);
        }
    }
}

COLORREF CMazeColor::GetColor(ColorType ct) const
{
    return m_colors[ct];
}

HPEN CMazeColor::GetPen(ColorType ct)
{
    if (m_pens[ct] == NULL)
    {
        m_pens[ct] = CreatePen(PS_SOLID, 1, m_colors[ct]);
    }

    return m_pens[ct];
}

HBRUSH CMazeColor::GetBrush(ColorType ct)
{
    if (m_brushs[ct] == NULL)
    {
        m_brushs[ct] = CreateSolidBrush(m_colors[ct]);
    }

    return m_brushs[ct];
}

COLORREF CMazeColor::SetColor(ColorType ct, COLORREF color)
{
    COLORREF oldColor = m_colors[ct];

    if (oldColor != color)
    {
        m_colors[ct] = color;

        if (m_pens[ct] != NULL)
        {
            DeleteObject(m_pens[ct]);
            m_pens[ct] = CreatePen(PS_SOLID, 1, m_colors[ct]);
        }

        if (m_brushs[ct] != NULL)
        {
            DeleteObject(m_brushs[ct]);
            m_brushs[ct] = CreateSolidBrush(m_colors[ct]);
        }
    }

    return oldColor;
}