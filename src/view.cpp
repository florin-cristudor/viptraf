/*
 * VIPTraf Text Field Class
 */
#include <stdio.h>

#include "video.h"
#include "attrs.h"

#include "view.h"

View::View(int y, int x, int attributes)
{
    position_x = x;
    position_y = y;

    view_attr = attributes;

    next = NULL;
}

int View::Draw(int win_descriptor)
{
    return pVideo->WSetAttribute(win_descriptor, view_attr);
}
