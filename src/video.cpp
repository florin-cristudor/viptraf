/*
 * VIPTraf Video Class
 */

#include "video.h"

Video::Video()
{
    enabled = false;
    mode = 0;
}

Video *pVideo;
int VideoMaxLines;
int VideoMaxCols;

int STDATTR;
int HIGHATTR;
int BOXATTR;
int ACTIVEATTR;
int BARSTDATTR;
int BARHIGHATTR;
int BARPTRATTR;
int DESCATTR;
int DLGTEXTATTR;
int DLGBOXATTR;
int DLGHIGHATTR;
int STATUSBARATTR;
int IPSTATLABELATTR;
int IPSTATATTR;
int DESKTEXTATTR;
int PTRATTR;
int FIELDATTR;
int ERRBOXATTR;
int ERRTXTATTR;
int OSPFATTR;
int UDPATTR;
int IGPATTR;
int IGMPATTR;
int IGRPATTR;
int ARPATTR;
int GREATTR;
int UNKNIPATTR;
int ICMPV6ATTR;
int IPV6ATTR;
int UNKNATTR;
