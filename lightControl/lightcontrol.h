#ifndef LIGHTCONTROL_H
#define LIGHTCONTROL_H

#include <QString>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_LIGHT_DEV_FILENAME "/dev/led"

class LightControl
{
public:
    LightControl();
    int setDevFileName(QString devFileName);
    int getLightStatus(char *status);
    int setLightStatus(char status);
private:
    QString devFileName;
};

#endif // LIGHTCONTROL_H
