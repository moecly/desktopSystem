#include "application.h"

Application::Application(QWidget *parent) : QWidget(parent)
{
    layoutInit();
}

Application::Application(QString appIconUrl, QString appName, QWidget *parent)
    : QWidget(parent) {
    layoutInit();
    setAppName(appName);
    setAppIconUrl(appIconUrl);
}

Application::Application(QString appIconUrl, QString appName, void (*clickedFunc)(), QWidget *parent) : QWidget(parent)
{
    layoutInit();
    setAppName(appName);
    setAppIconUrl(appIconUrl);
    setAppLink(clickedFunc);
    enableAppLink();
}

void Application::layoutInit() {
    pushButton = new QPushButton();
    label = new QLabel();
    VBoxLayout = new QVBoxLayout();

    mainWidget = new QWidget(this);
    mainWidget->setLayout(VBoxLayout);
    mainWidget->setFixedSize(70, 100);

    VBoxLayout->setContentsMargins(0, 0, 0, 0);
    VBoxLayout->addWidget(pushButton);
    VBoxLayout->addWidget(label);

    pushButton->setFixedSize(70, 70);
    label->setAlignment(Qt::AlignCenter);

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);
    label->setPalette(palette);
}

QString Application::getAppIconUrl() {
    return appIconUrl;
}

QString Application::getAppName() {
    return appName;
}

void Application::setAppIconUrl(QString appIconUrl) {
    QString url = "border-image: url("+ appIconUrl + ")";
    this->appIconUrl = appIconUrl;
    this->pushButton->setStyleSheet(url);
}

void Application::setAppName(QString appName) {
    this->label->setText(appName);
    this->appName = appName;
}

void Application::setAppLink(void (*clickedFunc)()) {
    appLinkFunc = clickedFunc;
//    connect(this->pushButton, SIGNAL(clicked()), this, SLOT(clickedFunc()));
}

void Application::clickedFunc() {
    appLinkFunc();
}

void Application::enableAppLink() {
    connect(this->pushButton, SIGNAL(clicked()), this, SLOT(clickedFunc()));
}

void Application::disableAppLink() {
    disconnect(this->pushButton, SIGNAL(clicked()), this, SLOT(clickedFunc()));
}
