#ifndef MENU_H
#define MENU_H

#include "vbox.h"
#include "mitem.h"
#include "mi_entry.h"

class Menu: public ViewBox, public MenuItemEntry
{
public:
    Menu(int nlines, int ncols, int begin_y, int begin_x, int attributes,
         int y, int x, int size, const char *text);
    ~Menu();

    int AddItem(MenuItem *pmenuitem);

    int Show(void);
    int Hide(void);

    int Draw(void);
    int Run(void);

private:
    MenuItem *entries;
    MenuItem *crsi;

    void UnselectAll(void);
    MenuItem *GetPrevSelectableEntry(MenuItem *crs);
    MenuItem *GetNextSelectableEntry(MenuItem *crs);
};

#endif // MENU_H
