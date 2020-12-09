#ifndef MENU_ITEM_ENTRY_H
#define MENU_ITEM_ENTRY_H

#include "mitem.h"
#include "vtext.h"

class MenuItemEntry : public MenuItem, public ViewText
{
public:
    MenuItemEntry(int y, int x, int size, const char *text, int use_command);

    int DrawItem(int win_descriptor);
};

#endif // MENU_ITEM_ENTRY_H
