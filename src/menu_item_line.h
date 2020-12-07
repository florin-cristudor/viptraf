#ifndef MENU_ITEM_LINE_H
#define MENU_ITEM_LINE_H

#include "text_field.h"

class MenuItemLine : public TextField
{
public:
    MenuItemLine(int y, int x, int attributes, int size);

    int Draw(int win_descriptor);

private:
    int line_size;
};

#endif // MENU_ITEM_LINE_H
