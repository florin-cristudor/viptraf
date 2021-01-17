/*
 * VIPTraf Interface Class
 */
#include <linux/if.h>

#include "viptraf.h"
#include "close.h"
#include "attrs.h"
#include "interface/video.h"
#include "interface/menu.h"

#include "interfaces.h"

#define BUFFER_SIZE 1024

Interfaces::Interfaces()
{
    err = false;
    fdevs = NULL;
}

Interfaces::~Interfaces()
{
    Close();
}

int Interfaces::Open()
{
    if(fdevs)
        Close();

    fdevs = fopen("/proc/net/dev", "r");
    if(fdevs == NULL)
    {
        err = true;
        return -1;
    }

    char buffer[BUFFER_SIZE];

    if (fgets(buffer, sizeof(buffer), fdevs) == NULL ||
        fgets(buffer, sizeof(buffer), fdevs) == NULL )
    {
        err = true;
        return -2;
    }

    return 0;
}

int Interfaces::Close()
{
    if(fdevs)
        fclose(fdevs);
    fdevs = NULL;
    err = false;

    return 0;
}

std::string Interfaces::GetNextInterfaceName(void)
{
    if(err || fdevs == NULL)
        return std::string("error");

    char buffer[BUFFER_SIZE];



/*
    strcpy(ifname, "");

    if (!feof(fd)) {
        strcpy(buf, "");
        if (fgets(buf, 160, fd) == NULL) {}
        if (strcmp(buf, "") != 0) {
            memset(ifname, 0, n);
            strncpy(ifname, skip_whitespace(strtok(buf, ":")), n);
            if (ifname[n - 1] != '\0')
                strcpy(ifname, "");
            return 1;
        }
    }*/
    return 0;
}

std::string MenuSelectInterface(bool all_included)
{
    Menu *pMenu = new Menu(10, 30, (VideoMaxLines - 10) / 2, (VideoMaxCols - 30) / 2, ATTR_MENU_BOX, 0, 0, "Interfaces Menu", "");

    if(pMenu == NULL)
        exit_program(ERROR_MEMORY, "");

    return std::string("");
}
