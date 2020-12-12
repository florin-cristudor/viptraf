#ifndef MITEM_H
#define MITEM_H

#include "view.h"

#define MENUITEM_NONE               0
#define MENUITEM_COMMAND_EMPTY      1
#define MENUITEM_ABORT              2
#define MENUITEM_RESIZE             3

//define executable commands with IDs higher than 1000

class MenuItem : public View
{
public:
    MenuItem(int y, int x, int size, int use_command);

    int Draw(int win_descriptor);
    virtual int DrawItem(int win_descriptor) = 0;

    int GetCommand(void) { return command; }
    bool IsSelected(void) { return is_selected; }
    bool IsSelectable(void) { return command != MENUITEM_NONE; }
    void Select() { is_selected = true; }
    void Unselect() { is_selected = false; }
    bool IsMenu() { return is_menu; }

    MenuItem *previ;
    MenuItem *nexti;

    int (*exec_func)(void);
protected:
    int command;
    int item_size;
    bool is_selected;
    bool is_menu;
};

#endif // MITEM_H
