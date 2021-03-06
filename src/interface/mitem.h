#ifndef MITEM_H
#define MITEM_H

#define MENU_COMMAND_NONE       0
#define MENU_COMMAND_EMPTY      1
#define MENU_EXECUTE_DONE       2
#define MENU_ABORT              3
#define MENU_RESIZE             4
#define MENU_ERROR              5

//define executable commands with IDs higher than 1000

class MenuItem
{
public:
    MenuItem(int use_command);
    virtual ~MenuItem() {}

    virtual int Draw(int win_descriptor, int y, int x, int size) = 0;
    virtual int Execute(void) = 0;
    virtual bool IsMyHotKey(int ch) = 0;
    virtual int ExecuteHotKey(int ch) = 0;

    bool IsSelected(void) { return is_selected; }
    bool IsSelectable(void) { return command != MENU_COMMAND_NONE; }
    virtual void Select() { is_selected = true; }
    void Unselect() { is_selected = false; }
    bool IsMenu() { return is_menu; }

    MenuItem *previ;
    MenuItem *nexti;

protected:
    int command;
    bool is_selected;
    bool is_menu;
};

#endif // MITEM_H
