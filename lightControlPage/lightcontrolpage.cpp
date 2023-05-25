#include "lightcontrolpage.h"

#define LIGHT_STATUS "light status: "
#define LIGHT_ON "on"
#define LIGHT_OFF "off"

LightControlPage::LightControlPage(QWidget *parent) : QWidget(parent)
{
    layoutInit();
    queryAndSetLightStatusLabel();
    signalAndSlotInit();
}

void LightControlPage::signalAndSlotInit() {
    connect(controlLightPushButton, SIGNAL(clicked()), this, SLOT(controlLight()));
    connect(exitPushButton, SIGNAL(clicked()), this, SLOT(exitPushButtonClick()));
}

void LightControlPage::exitPushButtonClick() {
    this->close();
}

int LightControlPage::queryAndSetLightStatusLabel() {
    char status;
    int ret = 0;

    ret = lightControl.getLightStatus(&status);
    if (ret)
        return -1;

    QString lightStatus = LIGHT_STATUS;
    if (status == 1) {
        lightStatus += LIGHT_ON;
        controlLightPushButton->setText(LIGHT_OFF);
    } else {
        lightStatus += LIGHT_OFF;
        controlLightPushButton->setText(LIGHT_ON);
    }
    lightStatusLabel->setText(lightStatus);

    return 0;
}

void LightControlPage::layoutInit() {
    mainVBoxLayout = new QVBoxLayout();
    mainWidget = new QWidget(this);
    lightStatusLabel = new QLabel();
    controlLightPushButton = new QPushButton();
    exitPushButton = new QPushButton();

    mainWidget->setLayout(mainVBoxLayout);
    mainVBoxLayout->setAlignment(Qt::AlignCenter);
    mainVBoxLayout->setContentsMargins(0, 0, 0, 0);

    mainVBoxLayout->addWidget(lightStatusLabel);
    QFont font;
    font.setPixelSize(30);
    lightStatusLabel->setFont(font);
    exitPushButton->setFont(font);
    exitPushButton->setText("exit");
    QString status = LIGHT_STATUS;
    status += LIGHT_OFF;
    lightStatusLabel->setText(status);
    controlLightPushButton->setText(LIGHT_ON);
    controlLightPushButton->setFont(font);
    mainVBoxLayout->addWidget(lightStatusLabel);
    mainVBoxLayout->addWidget(controlLightPushButton);
    mainVBoxLayout->addWidget(exitPushButton);
}

int LightControlPage::setDevFileName(QString devFileName) {
    return lightControl.setDevFileName(devFileName);
}

void LightControlPage::controlLight() {
    char status;
    QString currentStatus = controlLightPushButton->text();
    if (currentStatus == LIGHT_ON) {
        status = 1;
    } else {
        status = 0;
    }
    lightControl.setLightStatus(status);
    queryAndSetLightStatusLabel();
}

void LightControlPage::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    mainWidget->resize(this->size());
}
