/*
 * VIPTraf Menu Item Line Class
 */
#include "video.h"
#include "menu_item_line.h"

MenuItemLine::MenuItemLine(int y, int x, int attributes, int size):View(y, x, attributes)
{
    line_size = size;
}

int MenuItemLine::Draw(int win_descriptor)
{
    View::Draw(win_descriptor);
    return pVideo->MvHLine(position_y, position_x, 0, line_size);
}
