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

DlgAbout::DlgAbout(int nlines, int ncols, int begin_y, int begin_x):
    Dialog(nlines, ncols, begin_y, begin_x, DLG_BUTTON_OK)
{
}

DlgAbout::~DlgAbout()
{
}

int RunDlgAbout(int command __attribute__((unused)))
{
    DlgAbout *dlg = new DlgAbout(16, 49, (VideoMaxLines - 16) / 2, (VideoMaxCols - 49) / 2+1);
    if(!dlg) return 0;

    dlg->AddField(new ViewText(0, 2, ATTR_DLG_BOX, 0, "%s - %s", VIPTRAF_NAME, VIPTRAF_VERSION));
    dlg->AddField(new ViewText(2, 2, ATTR_DLG_BOX, 0, "A View IP Traffic Utility"));
    dlg->AddField(new ViewText(4, 2, ATTR_DLG_BOX, 0, "Copyright (c) Florin Cristudor 2020-"));
    dlg->AddField(new ViewText(5, 2, ATTR_DLG_BOX, 0, "Copyright (c) Markus Ullmann (IPv6) ?-2020"));
    dlg->AddField(new ViewText(6, 2, ATTR_DLG_BOX, 0, "Copyright (c) Guy Martin ?-?"));
    dlg->AddField(new ViewText(7, 2, ATTR_DLG_BOX, 0, "Copyright (c) Gerard Paul Java 1997-2004"));
    dlg->AddField(new ViewText(9, 2, ATTR_DLG_BOX, 0, "This program is open-source software released"));
    dlg->AddField(new ViewText(10, 2, ATTR_DLG_BOX, 0, "under the terms of the GNU General Public"));
    dlg->AddField(new ViewText(11, 2, ATTR_DLG_BOX, 0, "License Version 2 or any later version."));
    dlg->AddField(new ViewText(12, 2, ATTR_DLG_BOX, 0, "See the included LICENSE file for details."));

    int rc = dlg->Execute();
    delete dlg;

    return rc;
}
