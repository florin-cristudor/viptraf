#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    union
    {
        unsigned int integer;
        struct
        {
            unsigned int color:1;
            unsigned int logging:1;
            unsigned int revlook:1;
            unsigned int servnames:1;
            unsigned int promisc:1;
            unsigned int mac:1;
            unsigned int v6inv4asv6:1;
            unsigned int reserved:9; //for sizeof(int) 4 bytes, this is half filled
            //special cases units
            unsigned int actmode:1;
        } bits;
    } un;
    unsigned int timeout;
    unsigned int logspan;
    unsigned int updrate;
    unsigned int closedint;
} TOPTIONS;

#define OPTION_COLOR        0
#define OPTION_LOGGING      1
#define OPTION_REVLOOK      2
#define OPTION_SERVNAMES    3
#define OPTION_PROMISC      4
#define OPTION_MAC          5
#define OPTION_V6INV4ASV6   6
//filling 9 bits
//special bits
#define OPTION_ACTMODE      32

#define OPTION_TIMEOUT      64
#define OPTION_LOGSPAN      65
#define OPTION_UPDRATE      66
#define OPTION_CLOSEDINT    67

class Options
{
public:
    Options();

    const char *GetValueString(int opt_idx);
    void ToggleBit(int bit_idx);
    void SetValue(int opt_idx, int value);
    int  GetValue(int opt_idx);

private:
    int ReadOptionsFromConfigFile(void);
    int SaveOptionsToConfigFile(void);
    void InitDefaultOptions(void);

    TOPTIONS options;
    char buffer[50];
};

#endif // OPTIONS_H
