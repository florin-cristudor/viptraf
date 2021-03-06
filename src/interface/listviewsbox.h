#ifndef LIST_VIEWS_BOX_H
#define LIST_VIEWS_BOX_H

#include "view.h"
#include "vbox.h"

class ListViewsBox: public ViewBox
{
public:
    ListViewsBox(int nlines, int ncols, int begin_y, int begin_x, int box_attributes);
    ~ListViewsBox();

    int AddField(View *pfield);
    int Draw(void);

protected:
    View *views;
};

#endif // LIST_VIEWS_BOX_H
