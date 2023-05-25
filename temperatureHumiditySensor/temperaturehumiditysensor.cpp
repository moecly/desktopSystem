#include "temperaturehumiditysensor.h"

TemperatureHumiditySensor::TemperatureHumiditySensor()
{
    setDevFileName(DEFAULT_DEV_FILENAME);
}

int TemperatureHumiditySensor::setDevFileName(QString devFileName) {
    const char *str = devFileName.toUtf8().constData();
    int fd = ::open(str, O_RDWR);
    if (fd < 0)
        return -1;

    ::close(fd);
    this->devFileName = devFileName;
    return 0;
}


int TemperatureHumiditySensor::getTemperatureHumidity(float *temperature, float *humidity) {
    const char *str = devFileName.toUtf8().constData();
    int fd = ::open(str, O_RDWR);
    unsigned int value = 0;
    int cnt = 0;

    if (fd < 0)
        goto err_open;

    while (cnt < 100) {
        if (read(fd, &value, 4) == 4)
            break;
        if (cnt >= 100)
            goto err_read;
        cnt++;
    }

    ::close(fd);

    *humidity = (float)(value >> 24) + (float)((value >> 16) & 0xff) / 10;
    *temperature = (float)((value >> 8) & 0xff) + (float)(value & 0xff) / 10;

    return 0;

err_read:
    ::close(fd);
err_open:
    return -1;
}
