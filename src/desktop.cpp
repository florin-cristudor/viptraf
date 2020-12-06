/*
 * VIPTraf Desktop Class
 */
#include "iptraf-ng-compat.h"
#include "video.h"
#include "attrs.h"
#include "log.h"

#include "desktop.h"

#include <unistd.h> //to delete

Desktop::Desktop()
{

}

int Desktop::Run()
{
    Draw();

    pVideo->InputTimeout(0);
    meta(stdscr, true); //nu
    while(true)
    {
        int ch = pVideo->GetCh();
        if(ch == 'x' || ch =='X')
            break;
        if(VideoResized)
        {
            debug_log("%s: Resize got %i(%i) %i(%i) ", __FUNCTION__, LINES, VideoMaxLines, COLS, VideoMaxCols);
            VideoResized = false;
            Draw();
        }
    }

    return 0;
}

int Desktop::Draw()
{
    debug_log("%s: Draw", __FUNCTION__);
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
