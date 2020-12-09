/*
 * VIPTraf Menu Item Class
 */
#include <stdio.h>

#include "video.h"
#include "attrs.h"

#include "mitem.h"

MenuItem::MenuItem(int y, int x, int size, int use_command):
        View(y, x, ATTR_MENU_NORMAL)
{
    command = use_command;
    item_size = size;
    is_selected = false;

    previ = NULL;
    nexti = NULL;
}

int MenuItem::Draw(int win_descriptor)
{
    return DrawItem(win_descriptor);
}

int MenuItem::DrawItem(int win_descriptor)
{
    if(is_selected)
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_NORMAL_HIGHLIGHT);
    else
        View::Draw(win_descriptor);
    return pVideo->MvWHLine(win_descriptor, position_y, position_x, ' ', item_size);
}
