/*
 * VIPTraf Desktop Class
 */
#include "iptraf.h"
#include "video.h"
#include "attrs.h"

#include "desktop.h"

Desktop::Desktop()
{
    TestScreenSize();
}

int Desktop::Run()
{
    Draw();

    while(true)
    {
        int ch = pVideo->GetCh();
        if(ch == 'x' || ch =='X')
            break;
        if(VideoResized)
        {
            VideoResized = false;
            TestScreenSize();
            Draw();
        }
    }

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
