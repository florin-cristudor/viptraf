/*
 * VIPTraf Menu Item Option bit Class
 */
#include <stdio.h>
#include <string.h>

#include "video.h"
#include "attrs.h"

#include "options.h"

#include "mi_option_bit.h"

MenuOptionBit::MenuOptionBit(const char *text, const char *help_text, int opt_idx):
        MenuItemEntry(text, help_text, MENUITEM_COMMAND_EMPTY)
{
    index = opt_idx;
}

int MenuOptionBit::Draw(int win_descriptor, int y, int x, int size)
{
    MenuItemEntry::Draw(win_descriptor, y, x, size);
    const char *value = opts.GetValueString(index);
    if(!value)
        return -1;

    if(is_selected)
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_HIGHLIGHT);
    else
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_NORMAL);
    return pVideo->MvWPrint(win_descriptor, y, x + size - strlen(value) -1, value);
}

int MenuOptionBit::Execute()
{
    opts.ToggleBit(index);

    return MENUITEM_EXECUTE_DONE;
}
