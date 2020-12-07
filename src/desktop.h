/*
 * VIPTraf Desktop Class
 */
#ifndef DESKTOP_H
#define DESKTOP_H


class Desktop
{
public:
    Desktop();

    int Run(void);
    int Draw(void);

private:
    int TestScreenSize();
};

#endif // DESKTOP_H
