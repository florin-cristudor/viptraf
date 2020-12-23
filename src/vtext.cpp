/*
 * VIPTraf Text View Class
 */
#include <stdio.h>
#include <stdarg.h>

#include "video.h"
#include "view.h"
#include "vtext.h"

#define BUFFER_SIZE 2048

ViewText::ViewText(int y, int x, int new_attr, unsigned int opts, const char *format, ...):
        View(y, x)
{
    text_attr = new_attr;
    options = opts;

    char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    text = std::string(buffer);
}

ViewText::ViewText(int y, int x, int new_attr, unsigned int opts, std::string new_text):
        View(y, x)
{
    text_attr = new_attr;
    options = opts;

    text = new_text;
}

int ViewText::Draw(int win_descriptor, int size)
{
    pVideo->WSetAttribute(win_descriptor, text_attr);
    int x = position_x;
    if(options & VIEW_TEXT_CENTER)
        x = (size - text.size())/2 + 1;
    pVideo->WMove(win_descriptor, position_y, x);
    for(unsigned int i=0; i<text.size() && x<size+1; i++, x++)
        pVideo->WPrintCh(win_descriptor, text.c_str()[i]);
    return 0;
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

