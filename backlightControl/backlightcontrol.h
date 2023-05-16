#ifndef BACKLIGHTCONTROL_H
#define BACKLIGHTCONTROL_H

#include <QString>
#include <QFile>
#include "utils/common.h"

#define DEFAULT_BACKLIGHT_PATH "/sys/devices/soc0/backlight/backlight/backlight/brightness"
#define DEFAULT_BACKLIGHT_ACTUAL_PATH "/sys/devices/soc0/backlight/backlight/backlight/actual_brightness"

class BacklightControl
{
public:
    BacklightControl();
    BacklightControl(QString backlightBrighenessFileName, QString backlightBrighenessActualFileName);
    int setBacklightBrighenessFile(QString backlightBrighenessFileName);
    int setBacklightBrighenessActualFile(QString backlightBrighenessActualFileName);
    int getCurrentBacklightValue();
    int setCurrentBacklightvalue(int value);

private:
    QFile backlightBrighenessFile;
    QFile backlightBrighenessActualFile;
};

#endif // BRIGHTNESSCONTROL_H
