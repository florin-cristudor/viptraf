/*
 * VIPTraf Menu Item Entry Class
 */
#include <string.h>
#include <string>

#include "video.h"
#include "attrs.h"
#include "text_line.h"

#include "mi_entry.h"

extern TextLine *pHelpBar;

MenuItemEntry::MenuItemEntry(int y, int x, int size, const char *text, const char *help_text, int use_command, int (*call_back_func)(void)):
        MenuItem(y, x, size, use_command),
        ViewText(y, x, ATTR_MENU_NORMAL, text)
{
    hot_key = 0;
    exec_func = call_back_func;

    if(text)
    {
        for(unsigned int i=0; i<strlen(text); i++)
        {
            if(text[i] == '^')
            {
                hot_key = text[i+1];
                break;
            }
        }
    }

    help = std::string(help_text);
}

int MenuItemEntry::Draw(int win_descriptor)
{
    MenuItem::Draw(win_descriptor);
    bool highlight = false;
    pVideo->WMove(win_descriptor, MenuItem::pos_y, MenuItem::pos_x);
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

int MenuItemEntry::ExecuteHotKey(int ch)
{
    if(toupper(hot_key) == toupper(ch))
        return Execute();
    return MENUITEM_COMMAND_EMPTY;
}

void MenuItemEntry::Select()
{
    MenuItem::Select();
    if(pHelpBar)
    {
        pHelpBar->SetText(help.c_str());
        pHelpBar->Draw();
    }
}

int MenuItemEntry::Execute(void)
{
    if(command >= 1000)
        return command;

    if(exec_func)
    {
        (*exec_func)();
        return MENUITEM_EXECUTE_DONE;
    }

    return MENUITEM_COMMAND_EMPTY;
}
