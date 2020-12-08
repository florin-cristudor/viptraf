/*
 * VIPTraf Text Field Text Class
 */
#include <stdio.h>
#include <stdarg.h>

#include "video.h"
#include "view.h"
#include "vtext.h"

ViewText::ViewText(int y, int x, int attributes, const char *format, ...):View(y, x, attributes)
{
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    text = std::string(buffer);
}

int ViewText::Draw(int win_descriptor)
{
    View::Draw(win_descriptor);
    return pVideo->MvWPrint(win_descriptor, position_y, position_x, text.c_str());
}
