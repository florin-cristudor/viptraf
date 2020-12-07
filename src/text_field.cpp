/*
 * VIPTraf Text Field Class
 */
#include <stdio.h>

#include "video.h"
#include "attrs.h"

#include "text_field.h"

TextField::TextField(int y, int x, int attributes)
{
    position_x = x;
    position_y = y;

    field_attr = attributes;

    next = NULL;
}

int TextField::Draw(int win_descriptor)
{
    return pVideo->WSetAttribute(win_descriptor, field_attr);
}
