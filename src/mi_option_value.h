#ifndef MI_OPTION_VALUE_H
#define MI_OPTION_VALUE_H

#include "mi_entry.h"

class MenuOptionValue: public MenuItemEntry
{
public:
    MenuOptionValue(const char *text, const char *help_text, int opt_idx);

    virtual int Draw(int win_descriptor, int y, int x, int size);
    virtual int Execute(void);

private:
    int index;
};

#endif // MI_OPTION_VALUE_H
