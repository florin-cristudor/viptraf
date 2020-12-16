#ifndef ERR_BOX_H
#define ERR_BOX_H

#include "msg_box.h"

class ErrorBox: public MessageBox
{
public:
    ErrorBox(const char *text);
};

#endif // ERR_BOX_H
