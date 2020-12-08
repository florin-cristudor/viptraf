#ifndef VIEW_H
#define VIEW_H


class View
{
public:
    View(int y, int x, int attributes);
    virtual ~View() {}

    virtual int Draw(int win_descriptor) = 0;

    View *next;
protected:
    int position_x;
    int position_y;

    int view_attr;
};

#endif // VIEW_H
