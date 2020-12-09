/*
 * VIPTraf Menu Class
 */
#include <stdio.h>
#include <ncurses.h>

#include "video.h"

#include "menu.h"

#include "iptraf-ng-compat.h"
#include "log.h"

Menu::Menu(int nlines, int ncols, int begin_y, int begin_x, int attributes):
        ViewBox(nlines, ncols, begin_y, begin_x, attributes)
{
    entries = NULL;
    crsi = NULL;
}

Menu::~Menu()
{
    MenuItem *crs = entries;
    while(crs)
    {
        MenuItem *crsnext = crs->nexti;
        delete crs;
        crs = crsnext;

    }
}

int Menu::AddItem(MenuItem *pmenuitem)
{
    if(!pmenuitem)
        return -1;

    if(!entries)
    {
        entries = pmenuitem;
        pmenuitem->Select();
    }
    else
    {
        MenuItem *crs = entries;
        while(crs->nexti)
            crs = crs->nexti;
        crs->nexti = pmenuitem;
        pmenuitem->previ = crs;
    }

    return 0;
}

int Menu::Show(void)
{
    return ViewBox::Show();
}

int Menu::Hide(int ret_code)
{
    ViewBox::Hide();

    return ret_code;
}

int Menu::Draw(void)
{
    Show();
    ViewBox::DrawBox();

    MenuItem *crs = entries;
    while(crs)
    {
        crs->DrawItem(win);
        crs = crs->nexti;
    }

    return pVideo->Update();
}

int Menu::Run()
{
    bool redraw = true;
    if(!crsi)
        crsi = entries;

    if(!crsi) return Hide(MENUITEM_ABORT);

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
        debug_log("%s: key %c", __FUNCTION__, ch);
        MenuItem *crsnext = crsi;
        switch(ch)
        {
            case KEY_UP:
            debug_log("%s: key up", __FUNCTION__);
                crsnext = GetPrevSelectableEntry(crsi);
                redraw = true;
                break;
            case KEY_DOWN:
            debug_log("%s: key down", __FUNCTION__);
            crsnext = GetNextSelectableEntry(crsi);
                redraw = true;
                break;
            case 'x':
            case 'X':
                return Hide(MENUITEM_ABORT);
            default:
                break;
        }
        if(VideoResized)
            return Hide(MENUITEM_RESIZE);
        if(crsnext)
            crsi = crsnext;
    }
}

void Menu::UnselectAll()
{
    MenuItem *crs = entries;
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
