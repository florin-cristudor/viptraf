/*
 * VIPTraf Text View Class
 */
#include <stdio.h>
#include <stdarg.h>

#include "video.h"
#include "view.h"
#include "vtext.h"

#define BUFFER_SIZE 2048

ViewText::ViewText(int y, int x, int attributes, const char *format, ...):
        View(y, x)
{
    text_attr = attributes;

    char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    text = std::string(buffer);
}

int ViewText::Draw(int win_descriptor)
{
    pVideo->WSetAttribute(win_descriptor, text_attr);
    return pVideo->MvWPrint(win_descriptor, position_y, position_x, text.c_str());
}

int ViewText::SetText(const char *format, ...)
{
    if(!format)
        return -1;

    char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    text = std::string(buffer);

    return 0;
}
