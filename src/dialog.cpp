/*
 * VIPTraf Dialog Class
 */
#include <stdio.h>
#include <ncurses.h>

#include "attrs.h"
#include "video.h"

#include "dialog.h"

Dialog::Dialog(int nlines, int ncols, int begin_y, int begin_x, unsigned int opts):
        ListViewsBox(nlines, ncols, begin_y, begin_x, ATTR_DLG_BOX)
{
    button_attr = ATTR_DLG_BOX_REVERSED;
    options = opts;

    nr_buttons = 0;
    if(opts & DLG_BUTTON_CANCEL)
        nr_buttons++;
    if(opts & DLG_BUTTON_OK)
        nr_buttons++;
}

int Dialog::Execute(void)
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
            case CTRL('x'):
            case 27:
                return DLG_ESC;

        }
    }
}

int Dialog::Draw(void)
{
    Show();
    ListViewsBox::Draw();
    int space = (size_x - nr_buttons * 8) / (nr_buttons + 1);
    int opts = options;
    pVideo->WSetAttribute(win, button_attr);
    for(int i=space; i<size_x; i+=space)
    {
        pVideo->WMove(win, size_y-2, i+1);
        if(opts & DLG_BUTTON_OK)
        {
            pVideo->WPrint(win, "   OK   ");
            opts &= ~DLG_BUTTON_OK;
            i += 8;
            continue;
        }
        if(opts & DLG_BUTTON_CANCEL)
        {
            pVideo->WPrint(win, " Cancel ");
            opts &= ~DLG_BUTTON_CANCEL;
            i += 8;
            continue;
        }
    }

    return 0;
}
