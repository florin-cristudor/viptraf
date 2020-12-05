/*
 * VIPTraf Dialog Class
 */
#ifndef DIALOG_H
#define DIALOG_H

#include "text_box.h"

class Dialog
{
public:
    Dialog();
    virtual ~Dialog() {}

    virtual int Run() = 0;

    int ReadKeyboard(void);
    int Show(void);

protected:
    TextBox *tbox;
};

#endif // DIALOG_H
