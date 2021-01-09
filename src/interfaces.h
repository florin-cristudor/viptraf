#ifndef INTERFACES_H
#define INTERFACES_H

#include <stdio.h>
#include <string>

class Interfaces
{
public:
    Interfaces();
    ~Interfaces();

    int Open(void);
    int Close(void);
    std::string GetNextInterfaceName(void);
private:
    bool err;

    FILE *fdevs;
};

#endif // INTERFACES_H
