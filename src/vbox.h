#ifndef VIEW_BOX_H
#define VIEW_BOX_H

#include "view.h"

class ViewBox: public View
{
public:
    ViewBox(int nlines, int ncols, int begin_y, int begin_x, int attributes);
    ~ViewBox();

    int AddField(View *pfield);

    int Draw(int win_descriptor);
    int Draw(void);
    int ReadKeyboard(void);

private:
    int win;
    int panel;

    int number_columns;
    int number_lines;

    View *fields;
};

#endif // VIEW_BOX_H
