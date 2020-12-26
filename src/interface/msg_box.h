#ifndef MSG_BOX_H
#define MSG_BOX_H

#include "vtext.h"
#include "dialog.h"

class MessageBox: public Dialog
{
public:
    MessageBox(const char *new_text, int box_attributes, unsigned int opts);

private:
    int ParseText(const char *new_text);
};

#endif // MSG_BOX_H
