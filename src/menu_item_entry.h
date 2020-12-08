#ifndef MENU_ITEM_ENTRY_H
#define MENU_ITEM_ENTRY_H

#include "vtext.h"

class MenuItemEntry : public ViewText
{
public:
    MenuItemEntry(int y, int x, int attributes, const char *text);
};

#endif // MENU_ITEM_ENTRY_H
