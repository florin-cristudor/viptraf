/*
 * VIPTraf Text Box Class
 */
#include <stdio.h>
#include <ncurses.h>

#include "attrs.h"
#include "video.h"

#include "text_box.h"

TextBox::TextBox(int nlines, int ncols, int begin_y, int begin_x, int attributes)
{
    position_x = begin_x;
    position_y = begin_y;
    number_columns = ncols;
    number_lines = nlines;

    box_attr = attributes;

    fields = NULL;

    win = pVideo->NewWindow(nlines, ncols, begin_y, begin_x);
    panel = -1;
    if(win != -1)
        panel = pVideo->NewPanel(win);
}

TextBox::~TextBox()
{
    TextField *crs = fields;
    while(crs)
    {
        TextField *bak = crs->next;
        delete crs;
        crs = bak;
    }
    if(panel != -1)
        pVideo->DelPanel(panel);
    if(win != -1)
        pVideo->DelWindow(win);

    pVideo->Update();
}

int TextBox::AddField(TextField *pfield)
{
    if(!pfield)
        return -1;

    if(!fields)
        fields = pfield;
    else
    {
        TextField *crs = fields;
        while(crs->next)
            crs = crs->next;
        crs->next = pfield;
    }

    return 0;
}

int TextBox::Draw()
{
    if(win == -1)
        return -1;

    TextField *crs = fields;
    pVideo->WSetAttribute(win, box_attr);
    pVideo->ClearWindow(win);
    pVideo->WBorder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    while(crs)
    {
        crs->Draw(win);
        crs = crs->next;
    }
    return 0;
}

int TextBox::ReadKeyboard()
{
    if(win == -1)
        return 0;
    return pVideo->WGetCh(win);
}
