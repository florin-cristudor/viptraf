/*
 * VIPTraf Message Box Class
 */
#include <string.h>
#include <string>
#include <ncurses.h>

#include "video.h"

#include "msg_box.h"

MessageBox::MessageBox(const char *new_text, int box_attributes, unsigned int opts):
        Dialog(0, 0, 0, 0, opts)
{
    box_attr = box_attributes;

    size_y = 6;
    size_x = 4;

    ParseText(new_text);

    origin_x = VideoMaxCols / 2;
    origin_y = VideoMaxLines / 2;
    ofs_x = -size_x / 2;
    ofs_y = -size_y / 2;
}

int MessageBox::ParseText(const char *new_text)
{
    if(!new_text)
        return -1;

    size_y = 2;
    size_x = 0;
    unsigned int start = 0, icrs = 0;
    for(; icrs<strlen(new_text); icrs++)
    {
        if(new_text[icrs] != '\n')
            continue;
        //I'm on new line char
        ViewText *pntext = new ViewText(size_y, 1, box_attr, VIEW_TEXT_CENTER, std::string(&new_text[start], icrs-start));
        if(size_x < (int)(icrs - start) + 2)
            size_x = icrs - start + 2;
        start = icrs + 1;
        size_y++;
        AddField(pntext);
    }
    if(start != icrs)
    {
        ViewText *pntext = new ViewText(size_y, 1, box_attr, VIEW_TEXT_CENTER, std::string(&new_text[start], icrs-start));
        if(size_x < (int)(icrs-start) + 2)
            size_x = icrs - start + 2;
        size_y++;
        AddField(pntext);
    }
    size_y += 3;

    if(size_x > VideoMaxCols)
        size_x = VideoMaxCols;
    if(size_y > VideoMaxLines)
        size_y = VideoMaxLines;
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

    return 0;
}
