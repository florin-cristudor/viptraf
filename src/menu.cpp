/*
 * VIPTraf Menu Class
 */
#include <stdio.h>
#include <ncurses.h>

#include "video.h"
#include "text_line.h"

#include "menu.h"

extern TextLine *pHelpBar;

#include "iptraf-ng-compat.h"
#include "log.h"

Menu::Menu(int nlines, int ncols, int begin_y, int begin_x, int attributes, const char *text, const char *help_text):
        ViewBox(nlines, ncols, begin_y, begin_x, attributes),
        MenuItemEntry(text, help_text, MENUITEM_COMMAND_EMPTY)
{
    mitems = NULL;
    crsi = NULL;
    is_menu = true;
}

Menu::~Menu()
{
    MenuItem *crs = mitems;
    while(crs)
    {
        MenuItem *crsnext = crs->nexti;
        delete crs;
        crs = crsnext;

    }
}

int Menu::ExecuteHotKey(int ch)
{
    if(toupper(hot_key) == toupper(ch))
        return Execute();
    return MENUITEM_COMMAND_EMPTY;
}

int Menu::Execute(void)
{
    bool redraw = true;
    if(!crsi)
        crsi = mitems;
    if(!crsi) return MENUITEM_ABORT;

    while(true)
    {
        if(redraw)
        {
            UnselectAll();
            crsi->Select();
            Draw();
            redraw = false;
        }
        int ch = ViewBox::ReadKeyboard();
        if(VideoResized)
            return MENUITEM_RESIZE;
        debug_log("%s ch %i", __FUNCTION__, ch);
        MenuItem *crsnext = crsi;
        int exec_code = 0;
        switch(ch)
        {
            case 0:
                break;
            case KEY_UP:
                crsnext = GetPrevSelectableEntry(crsi);
                break;
            case KEY_DOWN:
                crsnext = GetNextSelectableEntry(crsi);
                break;
            case KEY_END:
            case KEY_RIGHT:
                crsnext = GetLastSelectableEntry();
                break;
            case KEY_HOME:
            case KEY_LEFT:
                crsnext = GetFirstSelectableEntry();
                break;
            case KEY_ENTER:
            case 0x0D:
                exec_code = crsi->Execute();
                if(exec_code >= 1000 || exec_code == MENUITEM_RESIZE)
                    return exec_code;
                break;
            case 'x':
            case 'X':
            case 27: //Esc
                return MENUITEM_ABORT;
            default:
                MenuItem *crs = mitems;
                while(crs)
                {
                    exec_code = crs->ExecuteHotKey(ch);
                    if(exec_code >= 1000 || exec_code == MENUITEM_RESIZE || exec_code == MENUITEM_EXECUTE_DONE)
                    {
                        crsi = crs;
                        crsnext = crs;
                        redraw = true;
                        UnselectAll();
                        crs->Select();
                        if(exec_code == MENUITEM_EXECUTE_DONE)
                            break;
                        return exec_code;
                    }
                    crs = crs->nexti;
                }
                break;
        }
        if(crsnext && crsi != crsnext)
        {
            crsi = crsnext;
            redraw =true;
        }
    }
}

int Menu::AddItem(MenuItem *pmenuitem)
{
    if(!pmenuitem)
        return -1;

    if(!mitems)
    {
        mitems = pmenuitem;
        pmenuitem->Select();
    }
    else
    {
        MenuItem *crs = mitems;
        while(crs->nexti)
            crs = crs->nexti;
        crs->nexti = pmenuitem;
        pmenuitem->previ = crs;
    }

    return 0;
}

int Menu::Draw(void)
{
    Show();
    ViewBox::Draw();
    MenuItem *crs = GetFirstPrintableItem(crsi, size_y-2);
    for(int i=1; crs && i<size_y-1; i++, crs=crs->nexti)
        crs->Draw(win, i, 1, size_x-2);

    return pVideo->Update();
}

void Menu::UnselectAll()
{
    MenuItem *crs = mitems;
    if(!crs)
        return;
    while(crs)
    {
        crs->Unselect();
        crs = crs->nexti;
    }
}

MenuItem *Menu::GetPrevSelectableEntry(MenuItem *crs)
{
    if(!crs)
        return NULL;

    while(true)
    {
        if(!crs->previ)
            return NULL;
        if(crs->previ->IsSelectable())
            return crs->previ;
        else
            crs = crs->previ;
    }
}

MenuItem *Menu::GetNextSelectableEntry(MenuItem *crs)
{
    if(!crs)
        return NULL;

    while(true)
    {
        if(!crs->nexti)
            return NULL;
        if(crs->nexti->IsSelectable())
            return crs->nexti;
        else
            crs = crs->nexti;
    }
}

MenuItem *Menu::GetLastSelectableEntry(void)
{
    MenuItem *crs = mitems;
    if(!crs)
        return NULL;
    while(crs->nexti)
        crs = crs->nexti;
    while(crs)
    {
        if(crs->IsSelectable())
            return crs;
        crs = crs->previ;
    }
    return crs;
}

MenuItem *Menu::GetFirstSelectableEntry()
{
    MenuItem *crs = mitems;
    while(crs)
    {
        if(crs->IsSelectable())
            return crs;
        crs = crs->nexti;
    }
    return NULL;
}

MenuItem *Menu::GetFirstPrintableItem(MenuItem *crs, int size)
{
    if(!crs)
        return mitems;

    int i = size-1;
    for(; i>0 && crs->previ; i--, crs=crs->previ);
    return crs;
}
