/*
 * VIPTraf Text Field Class
 */
#include <stdio.h>

#include "video.h"
#include "attrs.h"

#include "view.h"

View::View(int y, int x)
{
    position_x = x;
    position_y = y;

    nextv = NULL;
}
