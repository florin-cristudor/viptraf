/*
 * VIPTraf Dialog About Class
 */
#include <stdio.h>

#include "video.h"
#include "attrs.h"

#include "text_box.h"
#include "text_field_text.h"
#include "dlg_about.h"

#include <unistd.h>

DlgAbout::DlgAbout()
{
    tbox = new TextBox(16, 49, (VideoMaxLines - 16) / 2, (VideoMaxCols - 49) / 2+1, BOXATTR);
    if(!tbox) return;

    tbox->AddField(new TextFieldText(0, 2, STDATTR, "%s - %s", VIPTRAF_NAME, VIPTRAF_VERSION));
    tbox->AddField(new TextFieldText(2, 2, STDATTR, "A View IP Traffic Utility"));
    tbox->AddField(new TextFieldText(4, 2, STDATTR, "Copyright (c) Florin Cristudor 2020-"));
    tbox->AddField(new TextFieldText(5, 2, STDATTR, "Copyright (c) Markus Ullmann (IPv6) ?-2020"));
    tbox->AddField(new TextFieldText(6, 2, STDATTR, "Copyright (c) Guy Martin ?-?"));
    tbox->AddField(new TextFieldText(7, 2, STDATTR, "Copyright (c) Gerard Paul Java 1997-2004"));
    tbox->AddField(new TextFieldText(9, 2, STDATTR, "This program is open-source software released"));
    tbox->AddField(new TextFieldText(10, 2, STDATTR, "under the terms of the GNU General Public"));
    tbox->AddField(new TextFieldText(11, 2, STDATTR, "License Version 2 or any later version."));
    tbox->AddField(new TextFieldText(12, 2, STDATTR, "See the included LICENSE file for details."));

    tbox->AddField(new TextFieldText(14,2, HIGHATTR, "Press a key to continue"));

/*
//    tx_stdwinset(win);
//    pVideo->WInputTimeout(win, -1);
//        if (ch == 12)
//            tx_refresh_screen();
} while (ch == 12);
*/
}

DlgAbout::~DlgAbout()
{
    if(tbox)
        delete tbox;
    tbox = NULL;
}

int DlgAbout::Run()
{
    Show();
    while(ReadKeyboard() == 0) {}
    return 0;
}

