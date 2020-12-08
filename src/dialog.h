/*
 * VIPTraf Dialog Class
 */
#ifndef DIALOG_H
#define DIALOG_H

#include "vbox.h"

class Dialog
{
public:
    Dialog();
    virtual ~Dialog() {}

    virtual int Run() = 0;

    int ReadKeyboard(void);
    int Show(void);

protected:
    ViewBox *tbox;
};

#endif // DIALOG_H
