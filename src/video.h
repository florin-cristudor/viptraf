#ifndef VIDEO_H
#define VIDEO_H

#define VIDEO_MODE_TERM 1
#define VIDEO_MODE_X    2

class Video
{
public:
    Video();
    virtual ~Video() {}

    bool IsEnabled(void) { return enabled; }
    int  GetMode(void) { return mode; }

    //Interfete
    virtual int Init(void) = 0;
    virtual int Down(void) = 0;

    virtual int GetMaxY(void) = 0;
    virtual int GetMaxX(void) = 0;

    virtual int InputTimeout(int value) = 0;
    virtual int WInputTimeout(int descriptor, int value) = 0;
    virtual int WGetCh(int descriptor) = 0;

    virtual int SetAttribute(unsigned long attr) = 0;
    virtual int WSetAttribute(int descriptor, unsigned long attr) = 0;

    virtual int NewWindow(int nlines, int ncols, int begin_y, int begin_x) = 0;
    virtual int ClearWindow(int descriptor) = 0;

    virtual int NewPanel(int win_descriptor) = 0;

    virtual int DelWindow(int descriptor) = 0;
    virtual int DelPanel(int descriptor) = 0;

    virtual int Move(int y, int x) = 0;
    virtual int WMove(int descriptor, int y, int x) = 0;
    virtual int Print(const char *format, ...) = 0;
    virtual int MvPrint(int y, int x, const char *format, ...) = 0;
    virtual int WPrint(int descriptor, const char * format, ...) = 0;
    virtual int MvWPrint(int descriptor, int y, int x, const char *format, ...) = 0;
    virtual int PrintCh(unsigned long ch) = 0;
    virtual int WPrintCh(int descriptor, unsigned long ch) = 0;
    virtual int MvWPrintCh(int descriptor, int y, int x, unsigned long ch) = 0;
    virtual int WHLine(int descriptor, unsigned long ch, int size) = 0;
    virtual int MvWHLine(int descriptor, int y, int x, unsigned long ch, int size) = 0;
    virtual int WBorder(int descriptor, unsigned long  ls, unsigned long  rs, unsigned long  ts,
            unsigned long  bs, unsigned long  tl, unsigned long  tr, unsigned long  bl, unsigned long  br) = 0;

    virtual int InitColors(bool use_colors) = 0;

    virtual int Refresh(void) = 0;
    virtual int WRefresh(int descriptor) = 0;
    virtual int Update(void) = 0;

protected:
    bool enabled;
    int mode;
};

extern Video *pVideo;
extern int VideoMaxLines;
extern int VideoMaxCols;

#endif // VIDEO_H
