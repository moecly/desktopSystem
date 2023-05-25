#ifndef TEMPANDHUMIDITYPAGE_H
#define TEMPANDHUMIDITYPAGE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

class TempAndHumidityPage : public QWidget
{
    Q_OBJECT
public:
    explicit TempAndHumidityPage(QWidget *parent = nullptr);

private:
    QLabel *tempName;
    QLabel *tempValue;
    QLabel *humidityName;
    QLabel *humidityValue;
    QHBoxLayout *mainHBoxLayout;
    QWidget *mainWidget;

signals:

public slots:
};

#endif // TEMPANDHUMIDITYPAGE_H
