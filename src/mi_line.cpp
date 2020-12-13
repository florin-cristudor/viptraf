/*
 * VIPTraf Menu Item Line Class
 */
#include "video.h"
#include "attrs.h"

#include "mi_line.h"

MenuItemLine::MenuItemLine():
        MenuItem(MENUITEM_COMMAND_NONE)
{
}

int MenuItemLine::Draw(int win_descriptor, int y, int x, int size)
{
    pVideo->WSetAttribute(win_descriptor, ATTR_MENU_BOX);
    return pVideo->MvWHLine(win_descriptor, y, x, 0, size);
}

int MenuItemLine::Execute()
{
    return MENUITEM_COMMAND_EMPTY;
}

int MenuItemLine::ExecuteHotKey(int ch __attribute__((unused)))
{
    return MENUITEM_COMMAND_EMPTY;
}
