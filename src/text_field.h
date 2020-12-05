#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H


class TextField
{
public:
    TextField(int y, int x, int attributes);
    virtual ~TextField() {}

    virtual int Draw(int win_descriptor) = 0;

    TextField *next;
protected:
    int position_x;
    int position_y;

    int field_attr;
};

#endif // TEXT_FIELD_H
