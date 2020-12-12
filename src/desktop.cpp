/*
 * VIPTraf Desktop Class
 */
#include "iptraf.h"
#include "video.h"
#include "attrs.h"

#include "mi_entry.h"
#include "mi_line.h"
#include "vbox.h"
#include "menu.h"

#include "desktop.h"

Desktop::Desktop()
{
    TestScreenSize();
}

#define COMMAND_TRAFMON         1000
#define COMMAND_GENITFSTATS     1001
#define COMMAND_DETITFSTATS     1002
#define COMMAND_STATBREAKS      1003
#define COMMAND_LANMON          1004
#define COMMAND_FILTERS         1005
#define COMMAND_CONFIGURE       1006
#define COMMAND_ABOUT           1007

int Desktop::Run()
{
    Draw();

    int ncols = 31;
    int nlines = 15;
    Menu *pmenu = new Menu(nlines, ncols, (VideoMaxLines - nlines) / 2, (VideoMaxCols - ncols) / 2, BOXATTR);
    if(!pmenu)
        return shutdown(-10, "Memory error");
    pmenu->AddItem(new MenuItemEntry(1, 1, ncols-2, "IP traffic ^m^onitor", COMMAND_TRAFMON));
    pmenu->AddItem(new MenuItemEntry(2, 1, ncols-2, "General interface ^s^tatistics", COMMAND_GENITFSTATS));
    pmenu->AddItem(new MenuItemEntry(3, 1, ncols-2, "^D^etailed interface statistics", COMMAND_DETITFSTATS));
    pmenu->AddItem(new MenuItemEntry(4, 1, ncols-2, "Statistical ^b^reakdowns...", COMMAND_STATBREAKS));
    pmenu->AddItem(new MenuItemEntry(5, 1, ncols-2, "^L^AN station monitor", COMMAND_LANMON));
    pmenu->AddItem(new MenuItemLine(6, 1, ncols-2));
    pmenu->AddItem(new MenuItemEntry(7, 1, ncols-2, "^F^ilters...", COMMAND_FILTERS));
    pmenu->AddItem(new MenuItemLine(8, 1, ncols-2));
    pmenu->AddItem(new MenuItemEntry(9, 1, ncols-2, "C^o^nfigure...", COMMAND_CONFIGURE));
    pmenu->AddItem(new MenuItemLine(10, 1, ncols-2));
    pmenu->AddItem(new MenuItemEntry(11, 1, ncols-2, "^A^bout...", COMMAND_ABOUT));
    pmenu->AddItem(new MenuItemLine(12, 1, ncols-2));
    pmenu->AddItem(new MenuItemEntry(13, 1, ncols-2, "E^x^it", MENUITEM_ABORT));

    //        tx_initmenu(&menu, 15, 35, (VideoMaxLines - 16) / 2, (VideoMaxCols - 35) / 2, BOXATTR,
    //                STDATTR, HIGHATTR, BARSTDATTR, BARHIGHATTR, DESCATTR);
    //        tx_additem(&menu, " IP traffic ^m^onitor",
    //               "Displays current IP traffic information");
    //        tx_additem(&menu, " General interface ^s^tatistics",
    //               "Displays some statistics for attached interfaces");
    //        tx_additem(&menu, " ^D^etailed interface statistics",
    //               "Displays more statistics for a selected interface");
    //                tx_additem(&menu, " Statistical ^b^reakdowns...",
    //               "Facilities for traffic counts by packet size or TCP/UDP port");
    //        tx_additem(&menu, " ^L^AN station monitor",
    //               "Displays statistics on detected LAN stations");
    //        tx_additem(&menu, NULL, NULL);
    //        tx_additem(&menu, " ^F^ilters...",
    //               "Allows you to select traffic display and logging criteria");
    //        tx_additem(&menu, NULL, NULL);
    //        tx_additem(&menu, " C^o^nfigure...", "Set various program options");
    //        tx_additem(&menu, NULL, NULL);
    //        tx_additem(&menu, " ^A^bout...", "Displays program info");
    //        tx_additem(&menu, NULL, NULL);
    //        tx_additem(&menu, " E^x^it", "Exits program");

    bool run = true;
    while(run)
    {
        int cmd = pmenu->Run();
        pmenu->Hide();
        switch(cmd)
        {
            case MENUITEM_ABORT:
                run = false;
                break;
            case MENUITEM_RESIZE:
                VideoResized = false;
                TestScreenSize();
                pmenu->Move((VideoMaxLines - nlines) / 2, (VideoMaxCols - ncols) / 2);
                Draw();
                break;
        }
    }

    delete pmenu;
    return 0;
}

int Desktop::Draw()
{
    pVideo->SetAttribute(STATUSBARATTR);
    pVideo->MvHLine(0, 0, ' ', VideoMaxCols);
    pVideo->MvHLine(VideoMaxLines - 1, 0, ' ', VideoMaxCols);
    pVideo->MvPrint(0, 1, "%s %s", VIPTRAF_NAME, VIPTRAF_VERSION);

    /* draw the background */
    pVideo->SetAttribute(FIELDATTR);
    for (int row = 1; row <= VideoMaxLines - 2; row++)
        pVideo->MvHLine(row, 0, ' ', VideoMaxCols);

    pVideo->Refresh();
    return 0;
}

int Desktop::TestScreenSize()
{
    if(VideoMaxLines < 24 || VideoMaxCols < 80)
        return shutdown(-10, "Screen must have minimum 24 lines and 80 columns");

    return 0;
}
