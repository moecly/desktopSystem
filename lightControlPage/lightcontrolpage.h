#ifndef LIGHTCONTROLPAGE_H
#define LIGHTCONTROLPAGE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <utils/common.h>
#include <QPushButton>
#include <lightControl/lightcontrol.h>

class LightControlPage : public QWidget
{
    Q_OBJECT
public:
    explicit LightControlPage(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVBoxLayout;
    QWidget *mainWidget;
    QLabel *lightStatusLabel;
    QPushButton *controlLightPushButton;
    QPushButton *exitPushButton;
    LightControl lightControl;

    int queryAndSetLightStatusLabel();
    int setDevFileName(QString devFileName);
    void layoutInit();
    void paintEvent(QPaintEvent *event);
    void signalAndSlotInit();
signals:

public slots:
    void controlLight();
    void exitPushButtonClick();
};

#endif // LIGHTCONTROLPAGE_H
