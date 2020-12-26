/*
 * VIPTraf Dialog Class
 */
#ifndef DIALOG_H
#define DIALOG_H

#include "listviewsbox.h"

#define DLG_NO_BUTTONS      0x00
#define DLG_BUTTON_OK       0x01
#define DLG_BUTTON_CANCEL   0x02

#define DLG_ESC_PRESSED     27
#define DLG_OK_PRESSED      0x0D

class Dialog: public ListViewsBox
{
public:
    Dialog(int nlines, int ncols, int begin_y, int begin_x, unsigned int opts);
    ~Dialog(void);

    int Execute(void);

    int Draw(void);

protected:
    int nr_buttons;
    unsigned int options;

    int button_attr;

    View *focused;

private:
    void ShowCursor(void);
    void HideCursor(void);
    View *GetFirstFocused(void);
};

#endif // DIALOG_H
