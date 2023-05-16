#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

#define EACH_LIST(element, list) \
    int length = list.length(); \
    if (length != 0) \
    element = list.at(0); \
    for (int eachListInt = 0; eachListInt < length; \
        element = list.at(eachListInt), eachListInt++)

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 480
#define MIN_WIDTH 400
#define MIN_HEIGHT 300

class Common
{
public:
    Common();
};

#endif // COMMON_H
