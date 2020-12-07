/*
 * VIPTraf Text Field Text Class
 */
#include <stdio.h>
#include <stdarg.h>

#include "video.h"
#include "text_field_text.h"

TextFieldText::TextFieldText(int y, int x, int attributes, const char *format, ...):TextField(y, x, attributes)
{
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    text = std::string(buffer);
}

int TextFieldText::Draw(int win_descriptor)
{
    TextField::Draw(win_descriptor);
    return pVideo->MvWPrint(win_descriptor, position_y, position_x, text.c_str());
}
