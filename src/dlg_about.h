/*
 * VIPTraf Dialog About Class
 */
#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "listviewsbox.h"

class DlgAbout: public ListViewsBox
{
public:
    DlgAbout(int nlines, int ncols, int begin_y, int begin_x, int attributes);
    ~DlgAbout();

    int Draw(void);

    int Run(void);
};

extern int RunDlgAbout(void);

#endif // DLG_ABOUT_H
