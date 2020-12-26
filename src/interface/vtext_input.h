#ifndef VTEXT_INPUT_H
#define VTEXT_INPUT_H

#include "vtext.h"

class ViewTextInput : public ViewText
{
public:
    ViewTextInput(int y, int x, int size, const char *new_text);

    virtual int Draw(int win_descriptor, int size);

    int MoveCursor(int win_descriptor);
    int ProcessChar(int ch);

private:
    int size_x;
    size_t crs;
    size_t crs_view;

};

#endif // VTEXT_INPUT_H
