/*
 * VIPTraf Error Box Class
 */
#include "attrs.h"

#include "err_box.h"

ErrorBox::ErrorBox(const char *text):
    MessageBox(text, ATTR_ERR_BOX, ATTR_ERR_BOX_REVERSED, MB_BUTTON_OK)
{

}
