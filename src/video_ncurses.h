#ifndef VIDEO_NCURSES_H
#define VIDEO_NCURSES_H

#include <ncurses.h>
#include <panel.h>

#include "video.h"

#define VIDEO_NCURSES_DESCRIPTOR_MAX 200

struct nc_window
{
    WINDOW *win;
};

struct nc_panel
{
    PANEL *panel;
};

class VideoNcurses: public Video
{
public:
    VideoNcurses();
    ~VideoNcurses();

    virtual int Init(void);
    virtual int Down(void);

    int GetMaxY(void);
    int GetMaxX(void);

    int WKeybSetKeypad(int descriptor, bool value);
    int InputTimeout(int value);
    int WInputTimeout(int descriptor, int value);
    int GetCh(void);
    int WGetCh(int descriptor);
    int SetCursor(int value);

    int SetAttribute(unsigned long attr);
    int WSetAttribute(int descriptor, unsigned long attr);

    int NewWindow(int nlines, int ncols, int begin_y, int begin_x);
    int Clear(void);
    int ClearWindow(int descriptor);

    int NewPanel(int win_descriptor);

    int DelWindow(int descriptor);
    int DelPanel(int descriptor);

    int Move(int y, int x);
    int WMove(int descriptor, int y, int x);
    int Print(const char *format, ...) __attribute__ (( format( printf, 2, 3 ) ));
    int MvPrint(int y, int x, const char *format, ...);
    int WPrint(int descriptor, const char * format, ...);
    int MvWPrint(int descriptor, int y, int x, const char *format, ...);
    int PrintCh(unsigned long ch);
    int WPrintCh(int descriptor, unsigned long ch);
    int MvWPrintCh(int descriptor, int y, int x, unsigned long ch);
    int HLine(unsigned long ch, int size);
    int MvHLine(int y, int x, unsigned long ch, int size);
    int WHLine(int descriptor, unsigned long ch, int size);
    int MvWHLine(int descriptor, int y, int x, unsigned long ch, int size);
    int WBorder(int descriptor, unsigned long  ls, unsigned long  rs, unsigned long  ts,
            unsigned long  bs, unsigned long  tl, unsigned long  tr, unsigned long  bl, unsigned long  br);

    int InitColors(bool use_colors);

    int Refresh(void);
    int WRefresh(int descriptor);
    int Update(void);

private:
    int UpdateScreenSize(void);

    WINDOW *scr;

    struct nc_window wins[VIDEO_NCURSES_DESCRIPTOR_MAX];
    struct nc_panel panels[VIDEO_NCURSES_DESCRIPTOR_MAX];

    char buffer[65535];
};

#endif // VIDEO_NCURSES_H
