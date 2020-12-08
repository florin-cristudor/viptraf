#ifndef MENU_ITEM_LINE_H
#define MENU_ITEM_LINE_H

#include "view.h"

class MenuItemLine : public View
{
public:
    MenuItemLine(int y, int x, int attributes, int size);

    int Draw(int win_descriptor);

private:
    int line_size;
};

#endif // MENU_ITEM_LINE_H
