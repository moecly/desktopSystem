#include "brightnesscontrolpage.h"
#include <QPushButton>

BrightnessControlPage::BrightnessControlPage(QWidget *parent)
    : QWidget(parent),
      currentValue(8),
      maxValue(8),
      minValue(4)
{
    layoutInit();
    signalAndSlotInit();
    brightnessControlInit();
}

void BrightnessControlPage::ARMInit() {
#if __arm__
    setCursor(Qt::BlankCursor);
#endif
}

void BrightnessControlPage::brightnessControlInit() {
    int value = backlightControl.getCurrentBacklightValue();
    backlightControl.setCurrentBacklightvalue(value);
    slider->setValue(value);
}

void BrightnessControlPage::layoutInit() {
    resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    ARMInit();
    slider = new QSlider(Qt::Horizontal);
    slider->setRange(minValue, maxValue);
    slider->setValue(currentValue);
    slider->setMinimumHeight(40);
    slider->setMaximumHeight(40);

    middleHBoxLayout = new QHBoxLayout();
    middleHBoxLayout->setContentsMargins(0, 0, 0, 0);
    middleWidget = new QWidget();
    middleWidget->setLayout(middleHBoxLayout);

    additionBrightnessPushButton = new QPushButton();
    additionBrightnessPushButton->setText("+");

    reduceBrightnessPushButton = new QPushButton();
    reduceBrightnessPushButton->setText("-");
    middleHBoxLayout->addWidget(reduceBrightnessPushButton);
    middleHBoxLayout->addWidget(slider);
    middleHBoxLayout->addWidget(additionBrightnessPushButton);

    titleLabel = new QLabel();
    titleLabel->setText("brightness control");
    titleLabel->setAlignment(Qt::AlignCenter);

    exitPushButton = new QPushButton();
    exitPushButton->setText("exit");
    exitPushButton->setMinimumWidth(80);
    exitPushButton->setMaximumWidth(80);

    exitHBoxLayout = new QHBoxLayout();
    exitHBoxLayout->setAlignment(Qt::AlignCenter);
    exitHBoxLayout->setContentsMargins(0, 0, 0, 0);
    exitWidget = new QWidget();
    exitWidget->setLayout(exitHBoxLayout);
    exitHBoxLayout->addWidget(exitPushButton);

    vBoxLayout = new QVBoxLayout();
    vBoxLayout->setAlignment(Qt::AlignCenter);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget = new QWidget(this);
    mainWidget->setLayout(vBoxLayout);

    vBoxLayout->addWidget(titleLabel);
    vBoxLayout->addWidget(middleWidget);
    vBoxLayout->addWidget(exitWidget);
}

void BrightnessControlPage::signalAndSlotInit() {
    connect(exitPushButton, SIGNAL(clicked()), this, SLOT(exitClickedFunc()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(reduceBrightnessPushButton, SIGNAL(clicked()), this, SLOT(reduceBrightnessPushButtonClicked()));
    connect(additionBrightnessPushButton, SIGNAL(clicked()), this, SLOT(additionBrightnessPushButtonClicked()));
}

void BrightnessControlPage::reduceBrightnessPushButtonClicked() {
    int value = slider->value() - 1;
    if (value < minValue)
        return;
    slider->setValue(value);
    backlightControl.setCurrentBacklightvalue(value);
}

void BrightnessControlPage::additionBrightnessPushButtonClicked() {
    int value = slider->value() + 1;
    if (value > maxValue)
        return;
    slider->setValue(value);
    backlightControl.setCurrentBacklightvalue(value);
}

void BrightnessControlPage::sliderValueChanged(int value) {
    int backlightValue;
    backlightControl.setCurrentBacklightvalue(value);
    backlightValue = backlightControl.getCurrentBacklightValue();
}

void BrightnessControlPage::exitClickedFunc() {
    this->close();
}

void BrightnessControlPage::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    mainWidget->resize(this->size());
}

int BrightnessControlPage::setCurrentValue(int value) {
    if (value > maxValue)
        return -1;

    this->slider->setValue(value);
    return 0;
}

int BrightnessControlPage::setMaxValue(int value) {
    if (value < currentValue)
        return -1;

    this->maxValue = value;
    slider->setRange(minValue, maxValue);
    return 0;
}

int BrightnessControlPage::setMinValue(int value) {
    if (value > currentValue)
        return -1;

    this->minValue = value;
    slider->setRange(minValue, maxValue);
    return 0;
}
