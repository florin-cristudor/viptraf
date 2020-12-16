#ifndef VIEW_BOX_H
#define VIEW_BOX_H

class ViewBox
{
public:
    ViewBox(int nlines, int ncols, int begin_y, int begin_x, int attributes, int offset_y, int offset_x);
    ~ViewBox();

    int Show(void);
    int Hide(void);
    int Draw(void);

    int SetWindowOffset(int offset_y, int offset_x);
    int MoveOrigin(int begin_y, int begin_x);
    int Resize(int nlines, int ncols, int begin_y, int begin_x);

    int ReadKeyboard(void);

protected:
    int win;
    int panel;
    bool draw_box;
    int size_x;
    int size_y;
    int origin_x;
    int origin_y;
    int ofs_x;
    int ofs_y;

private:


    int box_attr;
};

#endif // VIEW_BOX_H
