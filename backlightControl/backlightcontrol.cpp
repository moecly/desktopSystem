#include "backlightcontrol.h"

BacklightControl::BacklightControl()
{
    setBacklightBrighenessFile(DEFAULT_BACKLIGHT_PATH);
    setBacklightBrighenessActualFile(DEFAULT_BACKLIGHT_ACTUAL_PATH);
}

BacklightControl::BacklightControl(QString backlightBrighenessFileName, QString backlightBrighenessActualFileName) {
    setBacklightBrighenessFile(backlightBrighenessFileName);
    setBacklightBrighenessActualFile(backlightBrighenessActualFileName);
}

int BacklightControl::getCurrentBacklightValue() {
    if (!backlightBrighenessActualFile.exists() || !backlightBrighenessActualFile.open(QIODevice::ReadOnly))
        return -1;

    char backlightValue;
    backlightBrighenessActualFile.read(&backlightValue, sizeof(backlightValue));
    backlightBrighenessActualFile.close();

    return ((int)backlightValue - '0');
}

int BacklightControl::setCurrentBacklightvalue(int value) {
    if (!backlightBrighenessFile.exists() || !backlightBrighenessFile.open(QIODevice::ReadWrite))
        return -1;

    char backlightValue = (char)value + '0';
    backlightBrighenessFile.write(&backlightValue, sizeof(backlightValue));
    backlightBrighenessFile.close();

    return 0;
}

int BacklightControl::setBacklightBrighenessFile(QString backlightBrighenessFileName) {
    backlightBrighenessFile.setFileName(backlightBrighenessFileName);
    if (!backlightBrighenessFile.exists())
        return -1;
    return 0;
}

int BacklightControl::setBacklightBrighenessActualFile(QString backlightBrighenessActualFileName) {
    backlightBrighenessActualFile.setFileName(backlightBrighenessActualFileName);
    if (!backlightBrighenessActualFile.exists())
        return -1;
    return 0;
}

