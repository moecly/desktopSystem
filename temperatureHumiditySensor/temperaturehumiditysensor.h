#ifndef TEMPERATUREHUMIDITYSENSOR_H
#define TEMPERATUREHUMIDITYSENSOR_H

#include <QString>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_DEV_FILENAME "/dev/dht11"

class TemperatureHumiditySensor
{
public:
    TemperatureHumiditySensor();
    int setDevFileName(QString devFlleName);
    int getTemperatureHumidity(float *temperature, float *humidity);

private:
    QString devFileName;
};

#endif // TEMPERATUREHUMIDITYSENSOR_H
