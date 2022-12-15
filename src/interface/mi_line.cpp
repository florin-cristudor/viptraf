/*
 * VIPTraf Menu Item Line Class
 */
#include "video.h"
#include "attrs.h"

#include "mi_line.h"

MenuItemLine::MenuItemLine():
        MenuItem(MENU_COMMAND_NONE)
{
}

MenuItemLine::~MenuItemLine()
{
}

int MenuItemLine::Draw(int win_descriptor, int y, int x, int size)
{
    pVideo->WSetAttribute(win_descriptor, ATTR_MENU_BOX);
    return pVideo->MvWHLine(win_descriptor, y, x, 0, size);
}

int MenuItemLine::Execute()
{
    return MENU_COMMAND_EMPTY;
}

int MenuItemLine::ExecuteHotKey(int ch __attribute__((unused)))
{
    return MENU_COMMAND_EMPTY;
}
