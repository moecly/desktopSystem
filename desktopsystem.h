#ifndef DESKTOPSYSTEM_H
#define DESKTOPSYSTEM_H

#include <QWidget>
#include <sliderPage/sliderpage.h>
#include <desktopPage/desktoppage.h>
#include <QList>
#include <utils/common.h>

class DesktopSystem : public QWidget
{
    Q_OBJECT

public:
    DesktopSystem(QWidget *parent = nullptr);
    ~DesktopSystem();
    int addDesktopPage(DesktopPage *desktopPage);
    int delDesktopPageFromIndex(int index);
    int delDesktopPageFromName(QString name);
    void setDesktopBackground(QString bgUrl);
    DesktopPage *getDesktopPageFromIndex(int index);
    DesktopPage *getDesktopPageFromName(QString name);

private:
    SliderPage *sliderPage;
    QWidget *widget;
    void layoutInit();
    void resizeEvent(QResizeEvent *event);
    void ARMInit();
};
#endif // DESKTOPSYSTEM_H
