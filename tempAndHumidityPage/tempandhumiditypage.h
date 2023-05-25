#ifndef TEMPANDHUMIDITYPAGE_H
#define TEMPANDHUMIDITYPAGE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <utils/common.h>
#include <QPushButton>

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
    QPushButton *exitPushButton;

    void layoutInit();
    void paintEvent(QPaintEvent *event);
    void signalAndSlotInit();
signals:

public slots:
    void exitPushButtonClick();
};

#endif // TEMPANDHUMIDITYPAGE_H
