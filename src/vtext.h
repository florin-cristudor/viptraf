#ifndef VTEXT_H
#define VTEXT_H

#include <string>

#include "view.h"

class ViewText : public View
{
public:
    ViewText(int y, int x, int attributes, const char *format, ...);

    int Draw(int win_descriptor);
    int DrawText(int win_descriptor);

protected:
    std::string text;
};

#endif // VTEXT_H
