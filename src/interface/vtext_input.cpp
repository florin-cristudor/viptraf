/*
 * VIPTraf View Text Input Class
 */
#include <string.h>
#include <string>
#include <ncurses.h>
#include <ctype.h>

#include "attrs.h"
#include "video.h"

#include "vtext_input.h"

ViewTextInput::ViewTextInput(int y, int x, int size, const char *new_text):
        ViewText(y, x, ATTR_DLG_INPUT, 0, new_text)
{
    is_input = true;

    crs = strlen(new_text);
    crs_view = 0;
    size_x = size;
}

ViewTextInput::~ViewTextInput()
{
}

int ViewTextInput::Draw(int win_descriptor, int size)
{
    pVideo->WSetAttribute(win_descriptor, text_attr);
    pVideo->MvWHLine(win_descriptor, position_y, position_x, ' ', size_x);

    pVideo->WMove(win_descriptor, position_y, position_x);

    for(unsigned int i=crs_view; i<(unsigned int)size+crs_view && i<text.size(); i++)
        pVideo->WPrintCh(win_descriptor, text.c_str()[i]);

    return 0;
}

int ViewTextInput::MoveCursor(int win_descriptor)
{
    return pVideo->WMove(win_descriptor, position_y, position_x+crs);
}

int ViewTextInput::ProcessChar(int ch)
{
    switch(ch)
    {
        case KEY_BACKSPACE:
            if(crs >= 1)
            {
                text.erase(crs-1, 1);
                crs--;
                return VIEW_NEEDS_REDRAW;
            }
            break;
        case 127: //delete
            if(crs < text.size())
            {
                text.erase(crs, 1);
                return VIEW_NEEDS_REDRAW;
            }
            break;
        case KEY_LEFT:
            if(crs >= 1)
            {
                crs--;
                return VIEW_NEEDS_REDRAW;
            }
            break;
        case KEY_RIGHT:
            if(crs < text.size())
            {
                crs++;
                return VIEW_NEEDS_REDRAW;
            }
            break;
        default:
            if(isalnum(ch) || ch == '.' || ch == ':')
            {
                text.insert(crs, 1, ch);
                crs++;
                return VIEW_NEEDS_REDRAW;
            }
    }

    return 0;
}
