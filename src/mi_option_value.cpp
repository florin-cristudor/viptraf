/*
 * VIPTraf Menu Option Value Class
 */
#include <stdio.h>
#include <string.h>

#include "video.h"
#include "attrs.h"

#include "options.h"

#include "mi_option_value.h"

MenuOptionValue::MenuOptionValue(const char *text, const char *help_text, int opt_idx):
        MenuItemEntry(text, help_text, MENU_COMMAND_EMPTY)
{
    index = opt_idx;
}

int MenuOptionValue::Draw(int win_descriptor, int y, int x, int size)
{
    MenuItemEntry::Draw(win_descriptor, y, x, size);
    const char *value = opts.GetValueString(index);
    if(!value)
        return -1;

    if(is_selected)
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_HIGHLIGHT);
    else
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_NORMAL);
    return pVideo->MvWPrint(win_descriptor, y, x + size - strlen(value), value);
}

int MenuOptionValue::Execute()
{
    return MENU_EXECUTE_DONE;
}
