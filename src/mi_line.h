#ifndef MI_LINE_H
#define MI_LINE_H

#include "mitem.h"

class MenuItemLine : public MenuItem
{
public:
    MenuItemLine(int y, int x, int size);

    int Draw(int win_descriptor);
    virtual int Execute(void);
    virtual int ExecuteHotKey(int ch);
};

#endif // MI_LINE_H
