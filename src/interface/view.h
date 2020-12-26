#ifndef VIEW_H
#define VIEW_H

#define VIEW_NEEDS_REDRAW   -1

class View
{
public:
    View(int y, int x);
    virtual ~View() {}

    virtual int Draw(int win_descriptor, int size) = 0;

    View *nextv;
    bool is_input;

protected:
    int position_x;
    int position_y;
};

#endif // VIEW_H
