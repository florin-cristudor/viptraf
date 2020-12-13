#ifndef VIEW_BOX_H
#define VIEW_BOX_H

class ViewBox
{
public:
    ViewBox(int nlines, int ncols, int begin_y, int begin_x, int attributes);
    ~ViewBox();

    int Show(void);
    int Hide(void);
    int Draw(void);

    int Move(int begin_y, int begin_x);
    int Resize(int nlines, int ncols, int begin_y, int begin_x);

    int ReadKeyboard(void);

protected:
    int win;
    int panel;
    bool draw_box;
    int size_x;
    int size_y;

private:
    int pos_x;
    int pos_y;


    int box_attr;
};

#endif // VIEW_BOX_H
