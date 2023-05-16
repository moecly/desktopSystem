#ifndef BRIGHTNESSCONTROLPAGE_H
#define BRIGHTNESSCONTROLPAGE_H

#include "utils/common.h"
#include "backlightControl/backlightcontrol.h"
#include <QWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "desktopsystem.h"

class BrightnessControlPage : public QWidget
{
    Q_OBJECT
public:
    explicit BrightnessControlPage(QWidget *parent = nullptr);
    explicit BrightnessControlPage(DesktopSystem *desktopSystem, QWidget *parent = nullptr);

    int setCurrentValue(int value);
    int setMaxValue(int value);
    int setMinValue(int value);

private:
    int currentValue;
    int maxValue;
    int minValue;
    QLabel *titleLabel;
    QSlider *slider;
    QPushButton *additionBrightnessPushButton;
    QPushButton *reduceBrightnessPushButton;
    QWidget *middleWidget;
    QHBoxLayout *middleHBoxLayout;
    QHBoxLayout *exitHBoxLayout;
    QWidget *exitWidget;
    QVBoxLayout *vBoxLayout;
    QWidget *mainWidget;
    QPushButton *exitPushButton;
    BacklightControl backlightControl;

    void layoutInit();
    void resizeEvent(QResizeEvent *event);
    void signalAndSlotInit();
    void brightnessControlInit();

signals:

public slots:
    void exitClickedFunc();
    void sliderValueChanged(int value);
    void reduceBrightnessPushButtonClicked();
    void additionBrightnessPushButtonClicked();
};

#endif // BRIGHTNESSCONTROL_H
