/*
 * VIPTraf Text Box Class
 */
#include <stdio.h>
#include <ncurses.h>

#include "attrs.h"
#include "video.h"

#include "vbox.h"

ViewBox::ViewBox(int nlines, int ncols, int begin_y, int begin_x, int attributes):
        View(begin_y, begin_x, attributes)
{
    number_columns = ncols;
    number_lines = nlines;

    fields = NULL;

    win = -1;
    panel = -1;
}

ViewBox::~ViewBox()
{
    Hide();

    View *crs = fields;
    while(crs)
    {
        View *bak = crs->next;
        delete crs;
        crs = bak;
    }
}

int ViewBox::AddField(View *pfield)
{
    if(!pfield)
        return -1;

    if(!fields)
        fields = pfield;
    else
    {
        View *crs = fields;
        while(crs->next)
            crs = crs->next;
        crs->next = pfield;
    }

    return 0;
}

int ViewBox::Show()
{
    if(win != -1)
        return -1;

    win = pVideo->NewWindow(number_lines, number_columns, position_y, position_x);
    if(win != -1)
        panel = pVideo->NewPanel(win);

    if(win == -1 || panel == -1)
        return -1;

    return pVideo->WKeybSetKeypad(win, true);
}

int ViewBox::Hide()
{
    if(win == -1)
        return -1;

    if(panel != -1)
        pVideo->DelPanel(panel);
    panel = -1;
    if(win != -1)
        pVideo->DelWindow(win);
    win = -1;

    pVideo->Update();

    return 0;
}

int ViewBox::Move(int begin_y, int begin_x)
{
    position_y = begin_y;
    position_x = begin_x;

    return 0;
}

int ViewBox::DrawBox(int win_descriptor)
{
    if(win_descriptor == -1)
        return -1;

    View::Draw(win_descriptor);
    pVideo->ClearWindow(win_descriptor);
    return pVideo->WBorder(win_descriptor, 0, 0, 0, 0, 0, 0, 0, 0);
}

int ViewBox::Draw(int win_descriptor)
{
    if(win_descriptor == -1)
        return -1;

    DrawBox(win_descriptor);

    View *crs = fields;
    while(crs)
    {
        crs->Draw(win);
        crs = crs->next;
    }
    return 0;
}

int ViewBox::DrawBox()
{
    return DrawBox(win);
}

int ViewBox::Draw()
{
    return Draw(win);
}

int ViewBox::ReadKeyboard()
{
    if(win == -1)
        return 0;
    return pVideo->WGetCh(win);
}
