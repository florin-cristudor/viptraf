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
#include "text_line.h"

#include "dlg_about.h"

#include "desktop.h"

#include "iptraf-ng-compat.h"
#include "log.h"

TextLine *pHelpBar;

Desktop::Desktop()
{
    TestScreenSize();

    pHelpBar = new TextLine(1, VideoMaxCols, VideoMaxLines - 2, 0, ATTR_HELP_BAR, "Test");
}

#define COMMAND_EXIT            1000
#define COMMAND_TRAFMON         1001
#define COMMAND_GENITFSTATS     1002
#define COMMAND_DETITFSTATS     1003
#define COMMAND_STATBREAKS      1004
#define COMMAND_LANMON          1005
#define COMMAND_FILTERS         1006
#define COMMAND_CONFIGURE       1007
#define COMMAND_ABOUT           1008

int Desktop::Run()
{
    Draw();

    int ncols = 31;
    int nlines = 15;
    Menu *pmenu = new Menu(nlines, ncols, (VideoMaxLines - nlines) / 2, (VideoMaxCols - ncols) / 2, BOXATTR,
                           "Main Menu", "");
    if(!pmenu)
        return shutdown(-10, "Memory error");
    pmenu->AddItem(new MenuItemEntry("IP traffic ^m^onitor",
                "Displays current IP traffic information", COMMAND_TRAFMON));
    pmenu->AddItem(new MenuItemEntry("General interface ^s^tatistics",
                "Displays some statistics for attached interfaces", COMMAND_GENITFSTATS));
    pmenu->AddItem(new MenuItemEntry("^D^etailed interface statistics",
                "Displays more statistics for a selected interface", COMMAND_DETITFSTATS));
    pmenu->AddItem(new MenuItemEntry("Statistical ^b^reakdowns...",
                "Facilities for traffic counts by packet size or TCP/UDP port", COMMAND_STATBREAKS));
    pmenu->AddItem(new MenuItemEntry("^L^AN station monitor",
                "Displays statistics on detected LAN stations", COMMAND_LANMON));
    pmenu->AddItem(new MenuItemLine());
    pmenu->AddItem(new MenuItemEntry("^F^ilters...",
                "Allows you to select traffic display and logging criteria", COMMAND_FILTERS));
    pmenu->AddItem(new MenuItemLine());
    pmenu->AddItem(new MenuItemEntry("C^o^nfigure...",
                "Set various program options", COMMAND_CONFIGURE));
    pmenu->AddItem(new MenuItemLine());
    pmenu->AddItem(new MenuItemEntry("^A^bout...",
                "Displays program info", MENUITEM_COMMAND_EMPTY, &RunDlgAbout));
    pmenu->AddItem(new MenuItemLine());
    pmenu->AddItem(new MenuItemEntry("E^x^it",
                "Exits program", COMMAND_EXIT));

    //        tx_initmenu(&menu, 15, 35, (VideoMaxLines - 16) / 2, (VideoMaxCols - 35) / 2, BOXATTR,
    //                STDATTR, HIGHATTR, BARSTDATTR, BARHIGHATTR, DESCATTR);

    bool run = true;
    while(run)
    {
        int cmd = pmenu->Execute();
        pmenu->Hide();
        debug_log("%s: Command %i", __FUNCTION__, cmd);
        switch(cmd)
        {
            case COMMAND_EXIT:
            case MENUITEM_ABORT:
                run = false;
                break;
            case MENUITEM_RESIZE:
                VideoResized = false;
                TestScreenSize();
                if(pHelpBar)
                    pHelpBar->Resize(1, VideoMaxCols, VideoMaxLines - 2, 0);
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

    if(pHelpBar)
        pHelpBar->Draw();

    pVideo->Refresh();
    return 0;
}

int Desktop::TestScreenSize()
{
    if(VideoMaxLines < 24 || VideoMaxCols < 80)
        return shutdown(-10, "Screen must have minimum 24 lines and 80 columns");

    return 0;
}
