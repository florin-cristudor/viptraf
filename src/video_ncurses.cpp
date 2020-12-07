/*
 * VIPTraf Video Ncurses Class
 */
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>

#include "ncurses.h"

#include "attrs.h"
#include "video_ncurses.h"

VideoNcurses::VideoNcurses()
{
    mode = VIDEO_MODE_TERM;

    int i;
    for(i=0; i<VIDEO_NCURSES_DESCRIPTOR_MAX; i++)
    {
        wins[i].win = NULL;
        panels[i].panel = NULL;
    }
}

VideoNcurses::~VideoNcurses()
{
    int i;
    for(i=0; i<VIDEO_NCURSES_DESCRIPTOR_MAX; i++)
        DelPanel(i);
    for(i=0; i<VIDEO_NCURSES_DESCRIPTOR_MAX; i++)
        DelWindow(i);

    Down();
}

int VideoNcurses::Init()
{
    setlocale(LC_ALL, "");	/* needed to properly init (n)curses library */
    scr = initscr();

    noecho();
    nonl();
    cbreak();
    curs_set(0);

    if(scr)
    {
        enabled = true;
        UpdateScreenSize();
        return 0;
    }

    return -1;
}

int VideoNcurses::Down()
{
    if(!enabled)
        return 0;

    nodelay(stdscr, false);
    keypad(stdscr, false);
    nocbreak();
    nl();
    echo();
    erase();
    update_panels();
    doupdate();
    noraw();
    return endwin();
}

int VideoNcurses::GetMaxY()
{
    if(!enabled)
        return 0;

    return LINES;
}

int VideoNcurses::GetMaxX()
{
    if(!enabled)
        return 0;

    return COLS;
}

int VideoNcurses::UpdateScreenSize(void)
{
    if(!enabled)
        return 0;
    VideoMaxLines = GetMaxY();
    VideoMaxCols = GetMaxX();

    return 0;
}

int VideoNcurses::InputTimeout(int value)
{
    if(!enabled)
        return 0;

    timeout(value);
    return 0;
}

int VideoNcurses::WInputTimeout(int descriptor, int value)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    wtimeout(win, value);
    return  0;
}

int VideoNcurses::GetCh()
{
    if(!enabled)
        return 0;

    int ch = getch();
    if(ch == ERR)
        return 0;
    if(ch == KEY_RESIZE)
    {
        VideoResized = true;
        UpdateScreenSize();
        return 0;
    }

    return ch;
}

int VideoNcurses::WGetCh(int descriptor)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    int ch = wgetch(win);
    if(ch == ERR)
        return 0;

    if(ch == KEY_RESIZE)
    {
        VideoResized = true;
        UpdateScreenSize();
        return 0;
    }

    return ch;
}

int VideoNcurses::SetAttribute(unsigned long attr)
{
    if(!enabled)
        return 0;

    return attrset((attr_t)attr);
}

int VideoNcurses::WSetAttribute(int descriptor, unsigned long attr)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return wattrset(win, attr);
}

int VideoNcurses::NewWindow(int nlines, int ncols, int begin_y, int begin_x)
{
    if(!enabled)
        return 0;

    int i;
    for(i=0; i<VIDEO_NCURSES_DESCRIPTOR_MAX; i++)
    {
        if(wins[i].win == NULL)
        {
            wins[i].win = newwin(nlines, ncols, begin_y, begin_x);
            if(wins[i].win)
                return i;
            else return -1;
        }
    }

    return -1;
}

int VideoNcurses::Clear()
{
    if(!enabled)
        return 0;

    return clear();
}

int VideoNcurses::ClearWindow(int descriptor)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    scrollok(win, 0);
    for (int row = 0; row < getmaxy(win); row++)
        mvwprintw(win, row, 0, "%*c", getmaxx(win), ' ');
    scrollok(win, 1);
    return 0;
}

int VideoNcurses::NewPanel(int win_descriptor)
{
    if(!enabled)
        return 0;

    if(win_descriptor < 0 || win_descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[win_descriptor].win;
    if(!win)
        return -1;

    int i;
    for(i=0; i<VIDEO_NCURSES_DESCRIPTOR_MAX; i++)
    {
        if(panels[i].panel == NULL)
        {
            panels[i].panel = new_panel(win);
            if(panels[i].panel)
                return i;
            else return -1;
        }
    }

    return -1;
}

int VideoNcurses::DelWindow(int descriptor)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    wins[descriptor].win = NULL;
    return delwin(win);
}

int VideoNcurses::DelPanel(int descriptor)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    PANEL *panel = panels[descriptor].panel;
    if(!panel)
        return -1;
    panels[descriptor].panel = NULL;
    return del_panel(panel);
}

int VideoNcurses::Move(int y, int x)
{
    if(!enabled)
        return 0;

    return move(y, x);
}

int VideoNcurses::WMove(int descriptor, int y, int x)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return wmove(win, y, x);
}

int VideoNcurses::Print(const char *format, ...)
{
    if(!enabled)
        return 0;

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    return printw(buffer);
}

int VideoNcurses::MvPrint(int y, int x, const char *format, ...)
{
    if(!enabled)
        return 0;

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    return mvprintw(y, x, buffer);
}

int VideoNcurses::WPrint(int descriptor, const char *format, ...)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    return wprintw(win, buffer);
}

int VideoNcurses::MvWPrint(int descriptor, int y, int x, const char *format, ...)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    return mvwprintw(win, y, x, buffer);
}

int VideoNcurses::PrintCh(unsigned long ch)
{
    if(!enabled)
        return 0;

    return addch(ch);
}

int VideoNcurses::WPrintCh(int descriptor, unsigned long ch)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return waddch(win, ch);
}

int VideoNcurses::MvWPrintCh(int descriptor, int y, int x, unsigned long ch)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return mvwaddch(win, y, x, ch);
}

int VideoNcurses::HLine(unsigned long ch, int size)
{
    if(!enabled)
        return 0;

    return hline(ch, size);
}

int VideoNcurses::MvHLine(int y, int x, unsigned long ch, int size)
{
    if(!enabled)
        return 0;

    return mvhline(y, x, ch, size);
}

int VideoNcurses::WHLine(int descriptor, unsigned long ch, int size)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return whline(win, ch, size);
}

int VideoNcurses::MvWHLine(int descriptor, int y, int x, unsigned long ch, int size)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return mvwhline(win, y, x, ch, size);
}

int VideoNcurses::WBorder(int descriptor, unsigned long ls, unsigned long rs, unsigned long ts, unsigned long bs, unsigned long tl, unsigned long tr, unsigned long bl, unsigned long br)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return wborder(win, ls, rs, ts, bs, tl, tr, bl, br);
}

int VideoNcurses::InitColors(bool use_colors)
{
    if(!enabled)
        return 0;

    if (use_colors && has_colors())
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_WHITE);
        init_pair(2, COLOR_BLACK, COLOR_CYAN);
        init_pair(3, COLOR_CYAN, COLOR_BLUE);
        init_pair(4, COLOR_YELLOW, COLOR_RED);
        init_pair(5, COLOR_WHITE, COLOR_RED);
        init_pair(6, COLOR_BLUE, COLOR_CYAN);
        init_pair(7, COLOR_BLUE, COLOR_WHITE);
        init_pair(9, COLOR_RED, COLOR_WHITE);
        init_pair(10, COLOR_GREEN, COLOR_BLUE);
        init_pair(11, COLOR_CYAN, COLOR_BLACK);
        init_pair(12, COLOR_RED, COLOR_CYAN);
        init_pair(14, COLOR_YELLOW, COLOR_BLUE);
        init_pair(15, COLOR_YELLOW, COLOR_BLACK);
        init_pair(16, COLOR_WHITE, COLOR_CYAN);
        init_pair(17, COLOR_YELLOW, COLOR_CYAN);
        init_pair(18, COLOR_GREEN, COLOR_BLACK);
        init_pair(19, COLOR_WHITE, COLOR_BLUE);

        STDATTR = COLOR_PAIR(14) | A_BOLD;
        HIGHATTR = COLOR_PAIR(3) | A_BOLD;
        BOXATTR = COLOR_PAIR(3);
        ACTIVEATTR = COLOR_PAIR(10) | A_BOLD;
        BARSTDATTR = COLOR_PAIR(15) | A_BOLD;
        BARHIGHATTR = COLOR_PAIR(11) | A_BOLD;
        BARPTRATTR = COLOR_PAIR(18) | A_BOLD;
        DESCATTR = COLOR_PAIR(2);
        DLGTEXTATTR = COLOR_PAIR(2);
        DLGBOXATTR = COLOR_PAIR(6);
        DLGHIGHATTR = COLOR_PAIR(12);
        STATUSBARATTR = STDATTR;
        IPSTATLABELATTR = COLOR_PAIR(2);
        IPSTATATTR = COLOR_PAIR(12);
        DESKTEXTATTR = COLOR_PAIR(7);
        PTRATTR = COLOR_PAIR(10) | A_BOLD;
        FIELDATTR = COLOR_PAIR(1);
        ERRBOXATTR = COLOR_PAIR(5) | A_BOLD;
        ERRTXTATTR = COLOR_PAIR(4) | A_BOLD;
        OSPFATTR = COLOR_PAIR(2);
        UDPATTR = COLOR_PAIR(9);
        IGPATTR = COLOR_PAIR(12);
        IGMPATTR = COLOR_PAIR(10) | A_BOLD;
        IGRPATTR = COLOR_PAIR(16) | A_BOLD;
        ARPATTR = COLOR_PAIR(5) | A_BOLD;
        GREATTR = COLOR_PAIR(1);
        UNKNIPATTR = COLOR_PAIR(19) | A_BOLD;
        ICMPV6ATTR = COLOR_PAIR(19) | A_BOLD;
        IPV6ATTR = COLOR_PAIR(19);
        UNKNATTR = COLOR_PAIR(4) | A_BOLD;
    }
    else
    {
        STDATTR = A_REVERSE;
        HIGHATTR = A_REVERSE;
        BOXATTR = A_REVERSE;
        ACTIVEATTR = A_BOLD;
        BARSTDATTR = A_NORMAL;
        BARHIGHATTR = A_BOLD;
        BARPTRATTR = A_NORMAL;
        DESCATTR = A_BOLD;
        DLGBOXATTR = A_REVERSE;
        DLGTEXTATTR = A_REVERSE;
        DLGHIGHATTR = A_BOLD;
        STATUSBARATTR = A_REVERSE;
        IPSTATLABELATTR = A_REVERSE;
        IPSTATATTR = A_STANDOUT;
        DESKTEXTATTR = A_NORMAL;
        PTRATTR = A_REVERSE;
        FIELDATTR = A_BOLD;
        ERRBOXATTR = A_BOLD;
        ERRTXTATTR = A_NORMAL;
        OSPFATTR = A_REVERSE;
        UDPATTR = A_BOLD;
        IGPATTR = A_REVERSE;
        IGMPATTR = A_REVERSE;
        IGRPATTR = A_REVERSE;
        ARPATTR = A_BOLD;
        GREATTR = A_BOLD;
        UNKNIPATTR = A_BOLD;
        ICMPV6ATTR = A_REVERSE;
        UNKNATTR = A_BOLD;
    }

    return 0;
}

int VideoNcurses::Refresh()
{
    if(!enabled)
        return 0;

    return refresh();
}

int VideoNcurses::WRefresh(int descriptor)
{
    if(!enabled)
        return 0;

    if(descriptor < 0 || descriptor >= VIDEO_NCURSES_DESCRIPTOR_MAX)
        return -1;
    WINDOW *win = wins[descriptor].win;
    if(!win)
        return -1;
    return wrefresh(win);
}

int VideoNcurses::Update()
{
    if(!enabled)
        return 0;

    update_panels();
    return doupdate();
}
