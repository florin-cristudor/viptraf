/*
 * VIPTraf Menu Item Entry Class
 */
#include <string>

#include "video.h"
#include "attrs.h"

#include "mi_entry.h"

MenuItemEntry::MenuItemEntry(int y, int x, int size, const char *text, int use_command):
        MenuItem(y, x, size, use_command),
        ViewText(y, x, ATTR_MENU_NORMAL, text)
{

}

int MenuItemEntry::DrawItem(int win_descriptor)
{
    MenuItem::DrawItem(win_descriptor);
    bool highlight = false;
    pVideo->WMove(win_descriptor, MenuItem::position_y, MenuItem::position_x);
    for(unsigned int i=0; i<text.size(); i++)
    {
        if(text.c_str()[i] == '^')
        {
            if(is_selected)
            {
                if(highlight)
                {
                    pVideo->WSetAttribute(win_descriptor, ATTR_MENU_NORMAL_HIGHLIGHT);
                    highlight = false;
                }
                else
                {
                    pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_HIGHLIGHT);
                    highlight = true;
                }

            }
            else
            {
                if(highlight)
                {
                    pVideo->WSetAttribute(win_descriptor, ATTR_MENU_NORMAL);
                    highlight = false;
                }
                else
                {
                    pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_NORMAL);
                    highlight = true;
                }
            }
            continue;
        }
        else
            pVideo->WPrintCh(win_descriptor, text.c_str()[i]);
    }
    return 0;
}
