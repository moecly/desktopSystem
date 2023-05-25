#include "lightcontrol.h"

LightControl::LightControl()
{
    setDevFileName(DEFAULT_LIGHT_DEV_FILENAME);
}

int LightControl::setDevFileName(QString devFileName) {
    const char *str = devFileName.toUtf8().constData();
    int fd = ::open(str, O_RDWR);
    if (fd < 0)
        return -1;

    ::close(fd);
    this->devFileName = devFileName;
    return 0;
}


int LightControl::getLightStatus(char *status) {
    const char *str = devFileName.toUtf8().constData();
    int fd = ::open(str, O_RDWR);
    char value = 0;
    if (fd < 0)
        goto err_open;

    if (read(fd, &value, 1) != 1)
        goto err_read;

    *status = value;

    ::close(fd);
    return 0;

err_read:
    ::close(fd);
err_open:
    return -1;
}

int LightControl::setLightStatus(char status) {
    const char *str = devFileName.toUtf8().constData();
    int fd = ::open(str, O_RDWR);
    if (fd < 0)
        goto err_open;

    if (write(fd, &status, 1) != 1)
        goto err_write;

    ::close(fd);
    return 0;

err_write:
    ::close(fd);
err_open:
    return -1;
}
