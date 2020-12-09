/*
 * VIPTraf Menu Item Line Class
 */
#include "video.h"
#include "attrs.h"

#include "mi_line.h"

MenuItemLine::MenuItemLine(int y, int x, int size):
        MenuItem(y, x, size, MENUITEM_COMMAND_NONE)
{
    view_attr = ATTR_MENU_BOX;
}

int MenuItemLine::DrawItem(int win_descriptor)
{
    MenuItem::DrawItem(win_descriptor);
    return pVideo->MvWHLine(win_descriptor, position_y, position_x, 0, item_size);
}
