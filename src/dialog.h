/*
 * VIPTraf Dialog Class
 */
#ifndef DIALOG_H
#define DIALOG_H

#include "listviewsbox.h"

#define DLG_NO_BUTTONS      0x00
#define DLG_BUTTON_OK       0x01
#define DLG_BUTTON_CANCEL   0x02

#define DLG_ESC             27

class Dialog: public ListViewsBox
{
public:
    Dialog(int nlines, int ncols, int begin_y, int begin_x, unsigned int opts);

    int Execute(void);

    int Draw(void);

protected:
    int nr_buttons;
    unsigned int options;

    int button_attr;
};

#endif // DIALOG_H
