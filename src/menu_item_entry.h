#ifndef MENU_ITEM_ENTRY_H
#define MENU_ITEM_ENTRY_H

#include "text_field_text.h"

class MenuItemEntry : public TextFieldText
{
public:
    MenuItemEntry(int y, int x, int attributes, const char *text);
};

#endif // MENU_ITEM_ENTRY_H
