/*
 * VIPTraf Dialog Class
 */
#include <stdio.h>

#include "video.h"

#include "dialog.h"

Dialog::Dialog()
{
    tbox = NULL;
}

int Dialog::ReadKeyboard()
{
    if(tbox)
        return tbox->ReadKeyboard();
    return 0;
}

int Dialog::Show()
{
    if(!tbox)
        return -1;
    tbox->Draw();
    return pVideo->Update();
}
