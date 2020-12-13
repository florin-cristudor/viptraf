#ifndef VTEXT_H
#define VTEXT_H

#include <string>

#include "view.h"

class ViewText : public View
{
public:
    ViewText(int y, int x, int attributes, const char *format, ...);

    virtual int Draw(int win_descriptor);

    int SetText(const char *format, ...);

protected:
    std::string text;

private:
    int text_attr;
};

#endif // VTEXT_H
