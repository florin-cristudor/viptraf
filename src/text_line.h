#ifndef TEXT_LINE_H
#define TEXT_LINE_H

#include "vtext.h"
#include "vbox.h"

class TextLine : public ViewBox, public ViewText
{
public:
    TextLine(int nlines, int ncols, int begin_y, int begin_x, int attributes, const char *text);
    ~TextLine();

    int Draw(void);
};

#endif // TEXT_LINE_H
