#include "hc_sr501.h"

Hc_Sr501::Hc_Sr501(QObject *parent) : QObject(parent)
{
    setDevFileName(DEFAULT_DEV_FILENAME);
}

Hc_Sr501::~Hc_Sr501()
{
    ::close(this->fd);
}

int Hc_Sr501::setDevFileName(QString devFileName) {
    const char *str = devFileName.toUtf8().constData();
    ::close(fd);
    fd = ::open(str, O_RDWR | O_NONBLOCK);
    if (fd < 0)
        return -1;

    this->devFileName = devFileName;
    return 0;
}

int Hc_Sr501::getDevState(unsigned int *state) {
    unsigned int value = 0;
    if (::read(fd, &value, 4) != 4)
        goto err_read;
    *state = value;

    return 0;
err_read:
    return -1;
}
