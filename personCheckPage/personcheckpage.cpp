#include "personcheckpage.h"

PersonCheckPage::PersonCheckPage(QWidget *parent) : QWidget(parent)
{
    layoutInit();
    devInit();
    signalAndSlotInit();
}

void PersonCheckPage::devInit() {
    hc_sr501 = new Hc_Sr501();
    timer = new QTimer(this);
    timer->start(1000);
}

void PersonCheckPage::layoutInit() {
    mainWidget = new QWidget(this);
    mainVBoxLayout = new QVBoxLayout();
    stateLabel = new QLabel();
    exitPushButton = new QPushButton();

    mainWidget->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    mainWidget->setLayout(mainVBoxLayout);

    mainVBoxLayout->setContentsMargins(0, 0, 0, 0);
    mainVBoxLayout->setAlignment(Qt::AlignCenter);

    mainVBoxLayout->addWidget(stateLabel);
    mainVBoxLayout->addWidget(exitPushButton);

    QFont font;
    font.setPixelSize(30);
    stateLabel->setFont(font);
    exitPushButton->setFont(font);

    stateLabel->setText("initialization...");
    exitPushButton->setText("exit");
}

void PersonCheckPage::exitPushButtonClicked() {
    timer->stop();
    this->close();
}

void PersonCheckPage::signalAndSlotInit() {
    connect(exitPushButton, SIGNAL(clicked()), this, SLOT(exitPushButtonClicked()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void PersonCheckPage::devStateChanged(unsigned int state) {
    unsigned int value = state;
    value &= 0xff;
    if (value == 1)
        stateLabel->setText("have people!");
    else if (value == 0)
        stateLabel->setText("no people");
}

void PersonCheckPage::timerTimeout() {
    int ret = 0;
    unsigned int value = 0;
    ret = hc_sr501->getDevState(&value);

    if (ret) {
        stateLabel->setText("dev err!");
        return;
    }

    value &= 0xff;
    if (value == 1)
        stateLabel->setText("have people!");
    else if (value == 0)
        stateLabel->setText("no people");
}

void PersonCheckPage::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    mainWidget->resize(this->size());
}
