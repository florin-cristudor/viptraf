#ifndef VIEW_H
#define VIEW_H


class View
{
public:
    View(int y, int x);
    virtual ~View() {}

    virtual int Draw(int win_descriptor) = 0;

    View *nextv;

protected:
    int position_x;
    int position_y;
};

#endif // VIEW_H
