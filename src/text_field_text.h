#ifndef TEXT_FIELD_TEXT_H
#define TEXT_FIELD_TEXT_H

#include <string>

#include "text_field.h"

class TextFieldText : public TextField
{
public:
    TextFieldText(int y, int x, int attributes, const char *format, ...);

    int Draw(int win_descriptor);

private:
    std::string text;
};

#endif // TEXT_FIELD_TEXT_H
