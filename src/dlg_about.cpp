/*
 * VIPTraf Dialog About Class
 */
#include <stdio.h>

#include "video.h"
#include "attrs.h"

#include "vtext.h"
#include "listviewsbox.h"
#include "mitem.h"

#include "dlg_about.h"

DlgAbout::DlgAbout(int nlines, int ncols, int begin_y, int begin_x, int attributes):
    ListViewsBox(nlines, ncols, begin_y, begin_x, attributes)
{
}

DlgAbout::~DlgAbout()
{
}

int DlgAbout::Draw(void)
{
    ListViewsBox::Draw();
    return pVideo->Update();
}

int DlgAbout::Run(void)
{
    Show();
    Draw();
    while(true)
    {   int rc = ReadKeyboard();
        if(VideoResized)
            break;
        if(rc != 0)
            break;
    }

    Hide();
    return MENU_ABORT;
}

int RunDlgAbout(void)
{
    DlgAbout *dlg = new DlgAbout(16, 49, (VideoMaxLines - 16) / 2, (VideoMaxCols - 49) / 2+1, BOXATTR);
    if(!dlg) return 0;

    dlg->AddField(new ViewText(0, 2, STDATTR, "%s - %s", VIPTRAF_NAME, VIPTRAF_VERSION));
    dlg->AddField(new ViewText(2, 2, STDATTR, "A View IP Traffic Utility"));
    dlg->AddField(new ViewText(4, 2, STDATTR, "Copyright (c) Florin Cristudor 2020-"));
    dlg->AddField(new ViewText(5, 2, STDATTR, "Copyright (c) Markus Ullmann (IPv6) ?-2020"));
    dlg->AddField(new ViewText(6, 2, STDATTR, "Copyright (c) Guy Martin ?-?"));
    dlg->AddField(new ViewText(7, 2, STDATTR, "Copyright (c) Gerard Paul Java 1997-2004"));
    dlg->AddField(new ViewText(9, 2, STDATTR, "This program is open-source software released"));
    dlg->AddField(new ViewText(10, 2, STDATTR, "under the terms of the GNU General Public"));
    dlg->AddField(new ViewText(11, 2, STDATTR, "License Version 2 or any later version."));
    dlg->AddField(new ViewText(12, 2, STDATTR, "See the included LICENSE file for details."));

    dlg->AddField(new ViewText(14,2, HIGHATTR, "Press a key to continue"));

    int rc = dlg->Run();
    delete dlg;

    return rc;
}
