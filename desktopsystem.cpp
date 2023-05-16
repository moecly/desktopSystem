#include "desktopsystem.h"

DesktopSystem::DesktopSystem(QWidget *parent)
    : QWidget(parent)
{
    layoutInit();
}

DesktopSystem::~DesktopSystem()
{
}

// TODO
void DesktopSystem::layoutInit() {
    this->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    this->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    widget = new QWidget(this);
    widget->resize(this->size());

    /*
     * 初始化滑动页面
     */
    sliderPage = new SliderPage(this);
    sliderPage->resize(this->size());
}

void DesktopSystem::setDesktopBackground(QString bgUrl) {
    QString url = "border-image: url(" + bgUrl + ")";
    widget->setStyleSheet(url);
}

void DesktopSystem::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    /*
     * 将滑动页面设置成this->size()
     */
    sliderPage->resize(this->size());
    widget->resize(this->size());
}

int DesktopSystem::addDesktopPage(DesktopPage *desktopPage) {
    return this->sliderPage->addDesktopPage(desktopPage);
}

int DesktopSystem::delDesktopPageFromIndex(int index) {
    return sliderPage->delDesktopPageFromIndex(index);
}

int DesktopSystem::delDesktopPageFromName(QString name) {
    return sliderPage->delDesktopPageFromName(name);
}

DesktopPage *DesktopSystem::getDesktopPageFromIndex(int index) {
    return sliderPage->getDesktopPageFromIndex(index);
}

DesktopPage *DesktopSystem::getDesktopPageFromName(QString name) {
    return sliderPage->getDesktopPageFromName(name);
}

