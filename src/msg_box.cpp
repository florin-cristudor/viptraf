/*
 * VIPTraf Message Box Class
 */
#include <string.h>
#include <string>
#include <ncurses.h>

#include "video.h"

#include "msg_box.h"

MessageBox::MessageBox(const char *new_text, int text_a, int button_a, unsigned int opts):
        ListViewsBox(0, 0, 0, 0, text_a),
        ViewText(2, 2, text_a, new_text)
{
    attr_buttons = button_a;
    options = opts;

    nr_buttons = 0;
    if(opts & MB_BUTTON_CANCEL)
        nr_buttons++;
    if(opts & MB_BUTTON_OK)
        nr_buttons++;

    size_y = 6;
    size_x = 4;

    if(!new_text)
        return;

    size_x = strlen(new_text) + 4;
    if(size_x > VideoMaxCols)
        size_x = VideoMaxCols;
    switch(nr_buttons)
    {
        case 1:
            if(size_x < 10)
                size_x = 10;
            break;
        case 2:
            if(size_x < 19)
                size_x = 19;
    }

    origin_x = VideoMaxCols / 2;
    origin_y = VideoMaxLines / 2;
    ofs_x = -size_x / 2;
    ofs_y = -size_y / 2;
}

int MessageBox::Execute(void)
{
    Draw();
    while(true)
    {
        int ch = ReadKeyboard();
        switch(ch)
        {
            case KEY_RESIZE:
                return ch;
            case KEY_ENTER:
            case 0x0D:
                return KEY_ENTER;
            case 'x':
            case 'X':
            case 27:
                return 0;

        }
    }
}

int MessageBox::Draw(void)
{
    Show();
    ListViewsBox::Draw();
    ViewText::position_x = (size_x - text.size())/2;
    ViewText::Draw(win);
    int space = (size_x - nr_buttons * 8) / (nr_buttons + 1);
    int opts = options;
    for(int i=space; i<size_x; i+=space)
    {
        pVideo->WSetAttribute(win, attr_buttons);
        pVideo->WMove(win, 4, i+1);
        if(opts & MB_BUTTON_OK)
        {
            pVideo->WPrint(win, "   OK   ");
            opts &= ~MB_BUTTON_OK;
            i += 8;
            continue;
        }
        if(opts & MB_BUTTON_CANCEL)
        {
            pVideo->WPrint(win, " Cancel ");
            opts &= ~MB_BUTTON_CANCEL;
            i += 8;
            continue;
        }
    }

    return 0;
}
