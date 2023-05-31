#ifndef HC_SR501_H
#define HC_SR501_H

#include <QString>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QObject>
#include <signal.h>

#define DEFAULT_DEV_FILENAME "/dev/hc_sr501"
#ifndef O_NONBLOCK
#define O_NONBLOCK	0000400
#endif
#ifndef SIGIO
#define SIGIO	23
#endif

class Hc_Sr501 : public QObject
{
    Q_OBJECT
public:
    Hc_Sr501(QObject *parent = nullptr);
    ~Hc_Sr501();
    int setDevFileName(QString devFileName);
    int getDevState(unsigned int *state);
private:
    int fd;
    QString devFileName;

signals:
public slots:
};

#endif // HC_SR501_H
