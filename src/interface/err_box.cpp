/*
 * VIPTraf Error Box Class
 */
#include "attrs.h"

#include "err_box.h"

ErrorBox::ErrorBox(const char *text):
    MessageBox(text, ATTR_ERR_BOX, DLG_BUTTON_OK | DLG_BUTTON_CANCEL)
{
    button_attr = ATTR_ERR_BOX_REVERSED;
}
