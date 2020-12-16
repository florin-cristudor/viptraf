/*
 * VIPTraf Text Line Class
 */
#include "video.h"

#include "text_line.h"

TextLine::TextLine(int nlines, int ncols, int begin_y, int begin_x, int attributes, const char *text):
        ViewBox(nlines, ncols, begin_y, begin_x, attributes, 0, 0),
        ViewText(0, 0, attributes, text)
{
    draw_box = false;
}

TextLine::~TextLine()
{
    Hide();
}

int TextLine::Draw()
{
    Show();
    ViewBox::Draw();
    return ViewText::Draw(win);
}
