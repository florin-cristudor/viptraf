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

    win = pVideo->NewWindow(nlines, ncols, begin_y, begin_x);
    panel = -1;
    if(win != -1)
        panel = pVideo->NewPanel(win);
}

ViewBox::~ViewBox()
{
    View *crs = fields;
    while(crs)
    {
        View *bak = crs->next;
        delete crs;
        crs = bak;
    }
    if(panel != -1)
        pVideo->DelPanel(panel);
    if(win != -1)
        pVideo->DelWindow(win);

    pVideo->Update();
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

int ViewBox::Draw(int win_descriptor)
{
    if(win_descriptor == -1)
        return -1;

    View::Draw(win);
    pVideo->ClearWindow(win);
    pVideo->WBorder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    View *crs = fields;
    while(crs)
    {
        crs->Draw(win);
        crs = crs->next;
    }
    return 0;
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
