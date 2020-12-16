#ifndef MSG_BOX_H
#define MSG_BOX_H

#include "listviewsbox.h"
#include "vtext.h"

#define MB_NO_BUTTONS       0x00
#define MB_BUTTON_OK        0x01
#define MB_BUTTON_CANCEL    0x02

class MessageBox: public ListViewsBox, public ViewText
{
public:
    MessageBox(const char *new_text, int text_a, int button_a, unsigned int opts);

    int Execute(void);

    int Draw(void);

private:
    int attr_buttons;
    int nr_buttons;
    unsigned int options;
};

#endif // MSG_BOX_H
