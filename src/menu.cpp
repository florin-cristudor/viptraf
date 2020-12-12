/*
 * VIPTraf Menu Class
 */
#include <stdio.h>
#include <ncurses.h>

#include "video.h"

#include "menu.h"

//#include "iptraf-ng-compat.h"
//#include "log.h"

Menu::Menu(int nlines, int ncols, int begin_y, int begin_x, int attributes,
           int y, int x, int size, const char *text):
        ViewBox(nlines, ncols, begin_y, begin_x, attributes),
        MenuItemEntry(y, x, size, text, MENUITEM_COMMAND_EMPTY)
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

int Menu::Hide(void)
{
    return ViewBox::Hide();
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
        MenuItem *crsnext = crsi;
        int exec_code = 0;
        switch(ch)
        {
            case KEY_UP:
                crsnext = GetPrevSelectableEntry(crsi);
                redraw = true;
                break;
            case KEY_DOWN:
                crsnext = GetNextSelectableEntry(crsi);
                redraw = true;
                break;
            case KEY_ENTER:
            case 0x0D:
                exec_code = crsi->GetCommand();
                if(exec_code >= 1000)
                    return exec_code;
                if(crsi->exec_func)
                {
                    exec_code = (*(crsi->exec_func))();
                    if(exec_code > 1000)
                        return exec_code;
                }
                else
                {

                }
                break;
            case 'x':
            case 'X':
                return MENUITEM_ABORT;
            default:
                break;
        }
        if(VideoResized)
            return MENUITEM_RESIZE;
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
