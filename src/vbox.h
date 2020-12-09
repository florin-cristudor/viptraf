#ifndef VIEW_BOX_H
#define VIEW_BOX_H

#include "view.h"

class ViewBox: public View
{
public:
    ViewBox(int nlines, int ncols, int begin_y, int begin_x, int attributes);
    ~ViewBox();

    int AddField(View *pfield);

    int Show(void);
    int Hide(void);
    int Move(int begin_y, int begin_x);

    int DrawBox(int win_descriptor);
    int Draw(int win_descriptor);
    int DrawBox(void);
    int Draw(void);

    int ReadKeyboard(void);

protected:
    int win;
    int panel;

private:
    int number_columns;
    int number_lines;

    View *fields;
};

#endif // VIEW_BOX_H
