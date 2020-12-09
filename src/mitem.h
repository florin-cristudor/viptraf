#ifndef MITEM_H
#define MITEM_H

#include "view.h"

#define MENUITEM_COMMAND_NONE       0
#define MENUITEM_ABORT              1
#define MENUITEM_RESIZE             2

//define executable commands with IDs higher than 1000

class MenuItem : public View
{
public:
    MenuItem(int y, int x, int size, int use_command);

    int Draw(int win_descriptor);

    virtual int DrawItem(int win_descriptor) = 0;
    bool IsSelected(void) { return is_selected; }
    bool IsSelectable(void) { return command != MENUITEM_COMMAND_NONE; }
    void Select() { is_selected = true; }
    void Unselect() { is_selected = false; }

    MenuItem *previ;
    MenuItem *nexti;

protected:
    int command;
    int item_size;
    bool is_selected;
};

#endif // MITEM_H
