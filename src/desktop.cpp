/*
 * VIPTraf Desktop Class
 */
#include "iptraf.h"

#include "viptraf.h"
#include "close.h"
#include "interface/video.h"
#include "attrs.h"
#include "options.h"

#include "interface/mi_entry.h"
#include "interface/mi_option_bit.h"
#include "interface/mi_option_value.h"
#include "interface/mi_line.h"
#include "interface/vbox.h"
#include "interface/menu.h"
#include "interface/text_line.h"

#include "interface/dlg_about.h"

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

/*
tx_initmenu(menu, 20, 40, (VideoMaxLines - 19) / 2 - 1, (VideoMaxCols - 40) / 16,
        BOXATTR, STDATTR, HIGHATTR, BARSTDATTR, BARHIGHATTR,
        DESCATTR);

tx_additem(menu, NULL, NULL);
tx_additem(menu, " ^A^dditional ports...",
       "Allows you to add port numbers higher than 1023 for the service stats");
tx_additem(menu, " ^D^elete port/range...",
       "Deletes a port or range of ports earlier added");
tx_additem(menu, NULL, NULL);
tx_additem(menu, " ^E^thernet/PLIP host descriptions...",
       "Manages descriptions for Ethernet and PLIP addresses");
tx_additem(menu, " ^F^DDI host descriptions...",
       "Manages descriptions for FDDI and FDDI addresses");
tx_additem(menu, NULL, NULL);
tx_additem(menu, " E^x^it configuration", "Returns to main menu");
*/
#define MENU_MAIN_SIZE_Y        15
#define MENU_MAIN_SIZE_X        31

int Desktop::Run()
{
    Draw();

    int pos_x;
    int pos_y;

    pos_y = (VideoMaxLines - MENU_MAIN_SIZE_Y) / 2;
    pos_x = (VideoMaxCols - MENU_MAIN_SIZE_X) / 2;

    Menu *pmntimers = new Menu(6, 41, pos_y, pos_x, ATTR_MENU_BOX, 2, 3,
                               "^T^imers...", "Configures timeouts and intervals");
    if(!pmntimers)
        return shutdown(-10, "Memory error");
    pmntimers->AddItem(new MenuOptionValue("TCP ^t^imeout...",
            "Sets the length of time before inactive TCP entries are considered idle", OPTION_TIMEOUT));
    pmntimers->AddItem(new MenuOptionValue("^L^ogging interval...",
            "Sets the time between loggings for interface, host, and service stats", OPTION_LOGSPAN));
    pmntimers->AddItem(new MenuOptionValue("^S^creen update interval...",
            "Sets the screen update interval in seconds (set to 0 for fastest updates)", OPTION_UPDRATE));
    pmntimers->AddItem(new MenuOptionValue("TCP closed/idle ^p^ersistence...",
            "Determines how long closed/idle/reset entries stay onscreen", OPTION_CLOSEDINT));

    Menu *pmncfg = new Menu(20, 41, pos_y, pos_x, ATTR_MENU_BOX, -3, -18,
                              "C^o^nfigure...", "Set various program options");
    if(!pmncfg)
        return shutdown(-10, "Memory error");
    pmncfg->AddItem((new MenuOptionBit("^R^everse DNS lookups",
                "Toggles resolution of IP addresses into host names", OPTION_REVLOOK)));
    pmncfg->AddItem(new MenuOptionBit("TCP/UDP ^s^ervice names",
                "Displays TCP/UDP service names instead of numeric ports", OPTION_SERVNAMES));
    pmncfg->AddItem(new MenuOptionBit("Force ^p^romiscuous mode",
                "Toggles capture of all packets by LAN interfaces", OPTION_PROMISC));
    pmncfg->AddItem(new MenuOptionBit("^C^olor",
                "Turns color on or off (restart IPTraf to effect change)", OPTION_COLOR));
    pmncfg->AddItem(new MenuOptionBit("^L^ogging",
                "Toggles logging of traffic to a data file", OPTION_LOGGING));
    pmncfg->AddItem(new MenuOptionBit("Acti^v^ity mode",
                "Toggles activity indicators between kbits/s and kbytes/s", OPTION_ACTMODE));
    pmncfg->AddItem(new MenuOptionBit("Source ^M^AC addrs in traffic monitor",
                "Toggles display of source MAC addresses in the IP Traffic Monitor", OPTION_MAC));
    pmncfg->AddItem(new MenuOptionBit("^S^how v6-in-v4 traffic as IPv6",
                "Toggled display of IPv6 tunnel in IPv4 as IPv6 traffic", OPTION_V6INV4ASV6));
    pmncfg->AddItem(new MenuItemLine());
    pmncfg->AddItem(pmntimers);
    pmncfg->AddItem(new MenuItemLine());

    Menu *pMenu = new Menu(MENU_MAIN_SIZE_Y, MENU_MAIN_SIZE_X, pos_y, pos_x, ATTR_MENU_BOX, 0, 0,
                           "Main Menu", "");
    if(!pMenu)
        return shutdown(-10, "Memory error");
    pMenu->AddItem(new MenuItemEntry("IP traffic ^m^onitor",
                "Displays current IP traffic information", COMMAND_TRAFMON));
    pMenu->AddItem(new MenuItemEntry("General interface ^s^tatistics",
                "Displays some statistics for attached interfaces", COMMAND_GENITFSTATS));
    pMenu->AddItem(new MenuItemEntry("^D^etailed interface statistics",
                "Displays more statistics for a selected interface", COMMAND_DETITFSTATS));
    pMenu->AddItem(new MenuItemEntry("Statistical ^b^reakdowns...",
                "Facilities for traffic counts by packet size or TCP/UDP port", COMMAND_STATBREAKS));
    pMenu->AddItem(new MenuItemEntry("^L^AN station monitor",
                "Displays statistics on detected LAN stations", COMMAND_LANMON));
    pMenu->AddItem(new MenuItemLine());
    pMenu->AddItem(new MenuItemEntry("^F^ilters...",
                "Allows you to select traffic display and logging criteria", COMMAND_FILTERS));
    pMenu->AddItem(new MenuItemLine());
    pMenu->AddItem(pmncfg);
    pMenu->AddItem(new MenuItemLine());
    pMenu->AddItem(new MenuItemEntry("^A^bout...",
                "Displays program info", MENU_COMMAND_EMPTY, &RunDlgAbout));
    pMenu->AddItem(new MenuItemLine());
    pMenu->AddItem(new MenuItemEntry("E^x^it",
                "Exits program", COMMAND_EXIT));

    //        tx_initmenu(&menu, 15, 35, (VideoMaxLines - 16) / 2, (VideoMaxCols - 35) / 2, BOXATTR,
    //                STDATTR, HIGHATTR, BARSTDATTR, BARHIGHATTR, DESCATTR);

    bool run = true;
    while(run)
    {
        int cmd = pMenu->Execute();
        pMenu->Hide();
        debug_log("%s: Command %i", __FUNCTION__, cmd);
        switch(cmd)
        {
            case COMMAND_EXIT:
            case MENU_ABORT:
                run = false;
                break;
            case MENU_RESIZE:
                VideoResized = false;
                TestScreenSize();
                if(pHelpBar)
                    pHelpBar->Resize(1, VideoMaxCols, VideoMaxLines - 2, 0);
                pos_y = (VideoMaxLines - MENU_MAIN_SIZE_Y) / 2;
                pos_x = (VideoMaxCols - MENU_MAIN_SIZE_X) / 2;
                pMenu->MoveOrigin(pos_y, pos_x);
                Draw();
                break;
        }
    }

    delete pMenu;
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
        return exit_program(ERROR_SCREEN_SIZE, "");

    return 0;
}
