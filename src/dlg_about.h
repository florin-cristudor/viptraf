/*
 * VIPTraf Dialog About Class
 */
#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "vbox.h"

class DlgAbout: public ViewBox
{
public:
    DlgAbout(int nlines, int ncols, int begin_y, int begin_x, int attributes);
    ~DlgAbout();

    int Draw(int win_descriptor);

    int Run(void);
};

extern int RunDlgAbout(void);

#endif // DLG_ABOUT_H
