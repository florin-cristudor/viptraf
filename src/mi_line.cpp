/*
 * VIPTraf Menu Item Line Class
 */
#include "video.h"
#include "attrs.h"

#include "mi_line.h"

MenuItemLine::MenuItemLine(int y, int x, int size):
        MenuItem(y, x, size, MENUITEM_COMMAND_NONE)
{
}

int MenuItemLine::Draw(int win_descriptor)
{
    pVideo->WSetAttribute(win_descriptor, ATTR_MENU_BOX);
    return pVideo->MvWHLine(win_descriptor, pos_y, pos_x, 0, item_size);
}

int MenuItemLine::Execute()
{
    return MENUITEM_COMMAND_EMPTY;
}

int MenuItemLine::ExecuteHotKey(int ch __attribute__((unused)))
{
    return MENUITEM_COMMAND_EMPTY;
}
