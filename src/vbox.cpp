/*
 * VIPTraf View Box Class
 */
#include <stdio.h>
#include <ncurses.h>

#include "attrs.h"
#include "video.h"

#include "vbox.h"

ViewBox::ViewBox(int nlines, int ncols, int begin_y, int begin_x, int attributes)
{
    pos_x = begin_x;
    pos_y = begin_y;
    size_x = ncols;
    size_y = nlines;

    draw_box = true;

    box_attr = attributes;

    win = -1;
    panel = -1;
}

ViewBox::~ViewBox()
{
    Hide();
}

int ViewBox::Show()
{
    if(win != -1)
        return -1;

    win = pVideo->NewWindow(size_y, size_x, pos_y, pos_x);
    if(win != -1)
        panel = pVideo->NewPanel(win);

    if(win == -1 || panel == -1)
        return -1;

    return pVideo->WKeybSetKeypad(win, true);
}

int ViewBox::Hide(void)
{
    if(win == -1)
        return -1;

    if(panel != -1)
        pVideo->DelPanel(panel);
    panel = -1;
    if(win != -1)
        pVideo->DelWindow(win);
    win = -1;

    return pVideo->Update();
}

int ViewBox::Draw(void)
{
    if(win == -1)
        return -1;

    pVideo->WSetAttribute(win, box_attr);
    pVideo->ClearWindow(win);
    if(draw_box)
        return pVideo->WBorder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    return 0;
}

int ViewBox::Move(int begin_y, int begin_x)
{
    pos_y = begin_y;
    pos_x = begin_x;

    if(win != -1)
    {
        Hide();
        Show();
    }

    return 0;
}

int ViewBox::Resize(int nlines, int ncols, int begin_y, int begin_x)
{
    size_y = nlines;
    size_x = ncols;

    return Move(begin_y, begin_x);
}

int ViewBox::ReadKeyboard()
{
    if(win == -1)
        return 0;
    return pVideo->WGetCh(win);
}
