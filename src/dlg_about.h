/*
 * VIPTraf Dialog About Class
 */
#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "dialog.h"

class DlgAbout: public Dialog
{
public:
    DlgAbout(int nlines, int ncols, int begin_y, int begin_x);
    ~DlgAbout();

};

extern int RunDlgAbout(void);

#endif // DLG_ABOUT_H
