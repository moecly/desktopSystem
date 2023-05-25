#ifndef TEMPANDHUMIDITYPAGE_H
#define TEMPANDHUMIDITYPAGE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <utils/common.h>
#include <QPushButton>
#include <temperatureHumiditySensor/temperaturehumiditysensor.h>

class TempAndHumidityPage : public QWidget
{
    Q_OBJECT
public:
    explicit TempAndHumidityPage(QWidget *parent = nullptr);

private:
    QLabel *tempNameLabel;
    QLabel *tempValueLabel;
    QVBoxLayout *tempVBoxLayout;
    QWidget *tempWidget;
    QLabel *humidityNameLabel;
    QLabel *humidityValueLabel;
    QWidget *humidityWidget;
    QVBoxLayout *humidityVBoxLayout;
    QHBoxLayout *hBoxLayout;
    QWidget *hWidget;
    QWidget *mainWidget;
    QVBoxLayout *mainVBoxLayout;
    QPushButton *refreshPushButton;
    QPushButton *exitPushButton;
    TemperatureHumiditySensor tHSensor;

    void layoutInit();
    void refreshTemperatureHumidityData();
    void paintEvent(QPaintEvent *event);
    void signalAndSlotInit();
signals:

public slots:
    void exitPushButtonClick();
    void refreshPushButtonClick();
};

#endif // TEMPANDHUMIDITYPAGE_H
