#include "desktoppage.h"

#define APP_COL_NUMBER 7
#define APP_ROW_NUMBER 4

DesktopPage::DesktopPage(QWidget *parent)
    : QWidget(parent),
      applicationNumber(0),
      name("default name")
{
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    layoutInit();
}

void DesktopPage::ARMInit() {
#if __arm__
    setCursor(Qt::BlankCursor);
#endif
}

void DesktopPage::layoutInit() {
    ARMInit();
    gridLayout = new QGridLayout();
    mainWidget = new QWidget(this);
    mainWidget->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    mainWidget->setLayout(gridLayout);
}

void DesktopPage::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    mainWidget->resize(this->size());
}

DesktopPage::DesktopPage(QString name, QWidget *parent)
    : QWidget(parent),
      applicationNumber(0) {
    this->name = name;
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    layoutInit();
}

int DesktopPage::addApplication(Application *app) {
    int row, col;
    appList.append(app);
    if (getApplicationLocation(&row, &col)) {
        return -1;
    }
    gridLayout->addWidget(app, row, col, 1, 1);
    return 0;
}

int DesktopPage::getApplicationLocation(int *row, int *col) {
    *row = applicationNumber / APP_COL_NUMBER;
    *col = applicationNumber % APP_COL_NUMBER;

    if (*row >= APP_ROW_NUMBER) {
        return -1;
    }

    applicationNumber++;
    return 0;
}

QString DesktopPage::getName() {
    return this->name;
}

void DesktopPage::setName(QString name) {
    this->name = name;
}
