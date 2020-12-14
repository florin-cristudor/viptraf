#ifndef MENU_H
#define MENU_H

#include "vbox.h"
#include "mitem.h"
#include "mi_entry.h"

class Menu: public ViewBox, public MenuItemEntry
{
public:
    Menu(int nlines, int ncols, int begin_y, int begin_x, int attributes,
         const char *text, const char *help_text);
    ~Menu();

    virtual int Execute(void);
    virtual int ExecuteHotKey(int ch);

    int AddItem(MenuItem *pmenuitem);

    int Draw(void);

private:
    MenuItem *mitems;
    MenuItem *crsi;

    void UnselectAll(void);
    MenuItem *GetPrevSelectableEntry(MenuItem *crs);
    MenuItem *GetNextSelectableEntry(MenuItem *crs);
    MenuItem *GetLastSelectableEntry(void);
    MenuItem *GetFirstSelectableEntry(void);
    MenuItem *GetFirstPrintableItem(MenuItem *crs, int size);
    MenuItem *GetItemHotKey(int ch);
};

#endif // MENU_H
