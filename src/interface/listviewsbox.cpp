/*
 * VIPTraf Text Box Class
 */
#include <stdio.h>

#include "listviewsbox.h"

ListViewsBox::ListViewsBox(int nlines, int ncols, int begin_y, int begin_x, int box_attributes):
        ViewBox(nlines, ncols, begin_y, begin_x, box_attributes, 0, 0)
{
    views = NULL;
}

ListViewsBox::~ListViewsBox()
{
    View *crs = views;
    while(crs)
    {
        View *bak = crs->nextv;
        delete crs;
        crs = bak;
    }
}

int ListViewsBox::AddField(View *pfield)
{
    if(!pfield)
        return -1;

    if(!views)
        views = pfield;
    else
    {
        View *crs = views;
        while(crs->nextv)
            crs = crs->nextv;
        crs->nextv = pfield;
    }

    return 0;
}

int ListViewsBox::Draw(void)
{
    ViewBox::Draw();
    View *crs = views;
    while(crs)
    {
        crs->Draw(win, size_x - 2);
        crs = crs->nextv;
    }
    return 0;
}
