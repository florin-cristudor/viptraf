/*
 * VIPTraf Menu Option Value Class
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <ncurses.h>
#include <limits.h>

#include "video.h"
#include "attrs.h"

#include "options.h"
#include "vtext_input.h"
#include "err_box.h"
#include "dialog.h"

#include "mi_option_value.h"

MenuOptionValue::MenuOptionValue(const char *text, const char *help_text, int opt_idx):
        MenuItemEntry(text, help_text, MENU_COMMAND_EMPTY)
{
    index = opt_idx;
}

int MenuOptionValue::Draw(int win_descriptor, int y, int x, int size)
{
    MenuItemEntry::Draw(win_descriptor, y, x, size);
    const char *value = opts.GetValueString(index);
    if(!value)
        return -1;

    if(is_selected)
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_HIGHLIGHT);
    else
        pVideo->WSetAttribute(win_descriptor, ATTR_MENU_COMMAND_NORMAL);
    return pVideo->MvWPrint(win_descriptor, y, x + size - strlen(value), value);
}

int MenuOptionValue::Execute()
{
    Dialog *pDlg = new Dialog(7, 37, (VideoMaxLines - 7) / 2, (VideoMaxCols - 37) / 2, DLG_BUTTON_OK | DLG_BUTTON_CANCEL);
    if(!pDlg)
        return MENU_ERROR;
    std::string title;
    for(unsigned int i=0; i<text.size(); i++)
    {
        if(text.c_str()[i] != '^' && text.c_str()[i] != '.')
            title.push_back(text.c_str()[i]);
    }
    const char *valueStr = opts.GetValueString(index);
    const char *pUnits = strchr(valueStr, ' ');
    if(pUnits)
    {
        title.append(" (");
        title.append(pUnits+1);
        title.append(")");
    }
    title.push_back(':');
    pDlg->AddField(new ViewText(1, 1, ATTR_DLG_BOX, 0, title.c_str()));

    std::string strInput(valueStr, pUnits - valueStr);
    ViewTextInput *pInput = new ViewTextInput(3, 2, 10, strInput.c_str());
    if(!pInput)
        return MENU_ERROR;
    pDlg->AddField(pInput);
    int rc = 0;
    switch(pDlg->Execute())
    {
        case KEY_RESIZE:
            rc = MENU_RESIZE;
            break;
        case DLG_OK_PRESSED:
        {
            const char *new_value = pInput->GetText();
            if(new_value)
            {
                long val;
                char *endptr;
                val = strtoul(new_value, &endptr, 10);
                errno = 0;
                if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0) || endptr == new_value)
                {
                    ErrorBox *pErr = new ErrorBox("Invalid value!");
                    if(pErr)
                    {
                        pErr->Execute();
                        delete pErr;
                    }
                }
                else
                {
                    opts.SetValue(index, (int)val);
                }
            }
            rc = MENU_EXECUTE_DONE;
            break;
        }
        case DLG_ESC_PRESSED:
        default:
            rc = MENU_EXECUTE_DONE;
            break;
    }

    delete pDlg;

    return rc;
}
