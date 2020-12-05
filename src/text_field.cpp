/*
 * VIPTraf Text Field Class
 */
#include <stdio.h>

#include "attrs.h"

#include "text_field.h"

TextField::TextField(int y, int x, int attributes)
{
    position_x = x;
    position_y = y;

    field_attr = attributes;

    next = NULL;
}
