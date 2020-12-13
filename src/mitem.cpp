/*
 * VIPTraf Menu Item Class
 */
#include <stdio.h>

#include "video.h"
#include "attrs.h"

#include "mitem.h"

MenuItem::MenuItem(int y, int x, int size, int use_command)
{
    pos_x = x;
    pos_y = y;
    item_size = size;

    command = use_command;
    is_selected = false;
    is_menu = false;

    previ = NULL;
    nexti = NULL;
}

int MenuItem::Draw(int win_descriptor)
{
    if(is_selected)
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_NORMAL_HIGHLIGHT);
    else
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_NORMAL);
    return pVideo->MvWHLine(win_descriptor, pos_y, pos_x, ' ', item_size);
}

