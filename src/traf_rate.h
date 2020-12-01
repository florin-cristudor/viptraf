#ifndef TRAF_RATE_H
#define TRAF_RATE_H

#include <stdint.h>

#include "iptraf-ng-compat.h"

class Rate
{
public:
    Rate();
    ~Rate();

    void Add(unsigned long bytes, unsigned long ms);
    void Alloc(unsigned int new_n);
    unsigned long GetAverage() { return sma; }
    int Print(char *buf, unsigned bufsize);
    int PrintNoUnits(char *buf, unsigned bufsize);
    int PrintPps(char *buf, unsigned bufsize);

private:

    void Init(void);

    unsigned int        n;      // number of elements
    unsigned int        index;  // index into the values array
    unsigned long long  *rates; // units are: bytes per second
    unsigned long       sma;    // simple moving average

};

#endif // TRAF_RATE_H
