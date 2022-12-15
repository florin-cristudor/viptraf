#ifndef VTEXT_H
#define VTEXT_H

#include <string>

#include "view.h"

#define VIEW_TEXT_CENTER    0x01

class ViewText : public View
{
public:
    ViewText(int y, int x, int new_attr, unsigned int opts, const char *format, ...);
    ViewText(int y, int x, int new_attr, unsigned int opts, std::string new_text);
    virtual ~ViewText() {}

    virtual int Draw(int win_descriptor, int size);

    int SetText(const char *format, ...);
    int GetSize(void) { return text.size(); }
    const char *GetText(void) { return text.c_str(); }

protected:
    std::string text;
    int text_attr;

private:
    unsigned int options;
};

#endif // VTEXT_H
