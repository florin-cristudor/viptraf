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

MenuItemEntry::MenuItemEntry(const char *text, const char *help_text, int use_command, int (*call_back_func)(int)):
        MenuItem(use_command),
        ViewText(0, 0, ATTR_MENU_NORMAL, 0, text)
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

int MenuItemEntry::Draw(int win_descriptor, int y, int x, int size)
{
    MenuItem::Draw(win_descriptor, y, x, size);
    bool highlight = false;
    pVideo->WMove(win_descriptor, y, x);
    for(unsigned int i=0; i<text.size() && (int)i<size; i++)
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
            size++;
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
    return MENU_COMMAND_EMPTY;
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
    if(exec_func)
        return (*exec_func)(command);

    if(command >= 1000)
        return command;

    return MENU_COMMAND_EMPTY;
}

bool MenuItemEntry::IsMyHotKey(int ch)
{
    if(hot_key == ch)
        return true;
    return false;
}
