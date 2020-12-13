#ifndef MENU_ITEM_ENTRY_H
#define MENU_ITEM_ENTRY_H

#include <string>

#include "mitem.h"
#include "vtext.h"

class MenuItemEntry : public MenuItem, public ViewText
{
public:
    MenuItemEntry(const char *text, const char *help_text,
                  int use_command, int (*call_back_func)(void)=NULL);

    int Draw(int win_descriptor, int y, int x, int size);
    virtual int Execute(void);
    virtual int ExecuteHotKey(int ch);

    virtual void Select(void);

protected:
    int hot_key;

    std::string help;
    int (*exec_func)(void);
};

#endif // MENU_ITEM_ENTRY_H
