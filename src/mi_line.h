#ifndef MI_LINE_H
#define MI_LINE_H

#include "mitem.h"

class MenuItemLine : public MenuItem
{
public:
    MenuItemLine(int y, int x, int size);

    int DrawItem(int win_descriptor);
};

#endif // MI_LINE_H
