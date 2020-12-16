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

Menu::Menu(int nlines, int ncols, int begin_y, int begin_x, int attributes, int offset_y, int offset_x, const char *text, const char *help_text):
        ViewBox(nlines, ncols, begin_y, begin_x, attributes, offset_y, offset_x),
        MenuItemEntry(text, help_text, MENU_COMMAND_EMPTY)
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
    return MENU_COMMAND_EMPTY;
}

int Menu::MoveOrigin(int begin_y, int begin_x)
{
    ViewBox::MoveOrigin(begin_y, begin_x);
    MenuItem *crs = mitems;
    while(crs)
    {
        if(crs->IsMenu())
            ((Menu*)crs)->MoveOrigin(begin_y, begin_x);
        crs = crs->nexti;
    }
    return 0;
}

int Menu::Execute(void)
{
    bool redraw = true;
    if(!crsi)
        crsi = mitems;
    if(!crsi) return MENU_ABORT;

    while(true)
    {
        if(redraw)
        {
            UnselectAll();
            crsi->Select();
            Draw();
            redraw = false;
        }
        if(VideoResized)
            return MENU_RESIZE;
        int ch = ViewBox::ReadKeyboard();
        if(VideoResized)
            return MENU_RESIZE;
        debug_log("%s ch %i", __FUNCTION__, ch);
        MenuItem *crsnext = crsi;
        int rc = 0;
        switch(ch)
        {
            case 0:
                break;
            case KEY_RESIZE:
                return MENU_RESIZE;
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
                rc = crsi->Execute();
                if(crsi->IsMenu())
                    ((Menu *)crsi)->Hide();
                if(rc >= 1000)
                    return rc;
                redraw = true;
                break;
            case 'x':
            case 'X':
            case 27: //Esc
                return MENU_ABORT;
            default:
                MenuItem *crs = GetItemHotKey(ch);
                if(!crs)
                    break;
                UnselectAll();
                crs->Select();
                crsi = crs;
                crsnext = crs;
                Draw();
                rc = crs->ExecuteHotKey(ch);
                if(crs->IsMenu())
                    ((Menu *)crs)->Hide();
                Draw();
                if(rc >= 1000)
                    return rc;
                if(rc == MENU_EXECUTE_DONE || rc == MENU_ABORT)
                    break;
                return rc;
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

MenuItem *Menu::GetItemHotKey(int ch)
{
    MenuItem *crs = mitems;
    while(crs)
    {
        if(crs->IsMyHotKey(ch))
            return crs;
        crs = crs->nexti;
    }
    return NULL;
}
