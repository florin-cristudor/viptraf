#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "text_field.h"

class TextBox
{
public:
    TextBox(int nlines, int ncols, int begin_y, int begin_x, int attributes);
    ~TextBox();

    int AddField(TextField *pfield);

    int Draw(void);
    int ReadKeyboard(void);

private:
    int win;
    int panel;

    int position_x;
    int position_y;
    int number_columns;
    int number_lines;

    int box_attr;

    TextField *fields;
};

#endif // TEXT_BOX_H
