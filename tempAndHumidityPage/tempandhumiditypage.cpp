#include "tempandhumiditypage.h"

TempAndHumidityPage::TempAndHumidityPage(QWidget *parent) : QWidget(parent)
{
    layoutInit();
    signalAndSlotInit();
    refreshTemperatureHumidityData();
}

void TempAndHumidityPage::signalAndSlotInit() {
    connect(exitPushButton, SIGNAL(clicked()), this, SLOT(exitPushButtonClick()));
    connect(refreshPushButton, SIGNAL(clicked()), this, SLOT(refreshPushButtonClick()));
}

void TempAndHumidityPage::refreshTemperatureHumidityData() {
    int ret = 0;
    float temp, humidity;
    ret = tHSensor.getTemperatureHumidity(&temp, &humidity);
    if (ret) {
        tempValueLabel->setText("--.-");
        humidityValueLabel->setText("--.-");
        return;
    }
    tempValueLabel->setText(QString::number(temp, 'f', 1));
    humidityValueLabel->setText(QString::number(humidity, 'f', 1));
}

void TempAndHumidityPage::refreshPushButtonClick() {
    refreshTemperatureHumidityData();
}

void TempAndHumidityPage::exitPushButtonClick() {
    this->close();
}

void TempAndHumidityPage::layoutInit() {
    tempNameLabel = new QLabel();
    tempValueLabel = new QLabel();
    humidityNameLabel = new QLabel();
    humidityValueLabel = new QLabel();
    hBoxLayout = new QHBoxLayout();
    tempVBoxLayout = new QVBoxLayout();
    humidityVBoxLayout = new QVBoxLayout();
    hWidget = new QWidget();
    humidityWidget = new QWidget();
    tempWidget = new QWidget();
    mainWidget = new QWidget(this);
    mainVBoxLayout = new QVBoxLayout();
    refreshPushButton = new QPushButton();
    exitPushButton = new QPushButton();

    QFont font;
    font.setPixelSize(30);
    tempNameLabel->setFont(font);
    tempValueLabel->setFont(font);
    humidityNameLabel->setFont(font);
    humidityValueLabel->setFont(font);
    exitPushButton->setFont(font);
    refreshPushButton->setFont(font);

    hWidget->setLayout(hBoxLayout);
    hWidget->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);

    tempVBoxLayout->setContentsMargins(0, 0, 0, 0);
    tempVBoxLayout->setAlignment(Qt::AlignCenter);
    tempWidget->setLayout(tempVBoxLayout);
    tempVBoxLayout->addWidget(tempNameLabel);
    tempVBoxLayout->addWidget(tempValueLabel);

    humidityVBoxLayout->setContentsMargins(0, 0, 0, 0);
    humidityVBoxLayout->setAlignment(Qt::AlignCenter);
    humidityWidget->setLayout(humidityVBoxLayout);
    humidityVBoxLayout->addWidget(humidityNameLabel);
    humidityVBoxLayout->addWidget(humidityValueLabel);

    hBoxLayout->addWidget(tempWidget);
    hBoxLayout->addWidget(humidityWidget);

    tempNameLabel->setText("temp: ");
    tempValueLabel->setText("--.-°c");
    humidityNameLabel->setText("humidity: ");
    humidityValueLabel->setText("--.-°c");

    mainWidget->setLayout(mainVBoxLayout);
    mainVBoxLayout->setContentsMargins(0, 0, 0, 0);
    mainVBoxLayout->setAlignment(Qt::AlignCenter);
    mainVBoxLayout->addWidget(hWidget);
    mainVBoxLayout->addWidget(refreshPushButton);
    mainVBoxLayout->addWidget(exitPushButton);

    refreshPushButton->setText("refresh");
    refreshPushButton->setMinimumHeight(50);
    refreshPushButton->setMaximumHeight(50);
    exitPushButton->setText("exit");
    exitPushButton->setMinimumHeight(50);
    exitPushButton->setMaximumHeight(50);
}

void TempAndHumidityPage::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    mainWidget->resize(this->size());
}
