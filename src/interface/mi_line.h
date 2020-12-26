#ifndef MI_LINE_H
#define MI_LINE_H

#include "mitem.h"

class MenuItemLine : public MenuItem
{
public:
    MenuItemLine();

    int Draw(int win_descriptor, int y, int x, int size);
    virtual int Execute(void);
    virtual bool IsMyHotKey(int ch __attribute__((unused)) ) {  return false; }
    virtual int ExecuteHotKey(int ch);
};

#endif // MI_LINE_H
