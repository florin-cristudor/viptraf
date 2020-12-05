/*
 * VIPTraf Dialog About Class
 */
#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "dialog.h"

class DlgAbout: public Dialog
{
public:
    DlgAbout();
    ~DlgAbout();

    int Run();
};

#endif // DLG_ABOUT_H
