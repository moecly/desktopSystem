#ifndef SLIDERPAGE_H
#define SLIDERPAGE_H

#include <QWidget>
#include <desktopPage/desktoppage.h>
#include <utils/common.h>
#include <QList>
#include <QScrollArea>
#include <QScroller>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QLabel>

class SliderPage : public QWidget
{
    Q_OBJECT
public:
    explicit SliderPage(QWidget *parent = nullptr);
    int addDesktopPage(DesktopPage *desktopPage);
    int insertDesktopPage(DesktopPage *desktopPage, int index);
    int delDesktopPageFromIndex(int index);
    int delDesktopPageFromName(QString name);
    int getDesktopPageCount();
    DesktopPage *getDesktopPageFromIndex(int index);
    DesktopPage *getDesktopPageFromName(QString name);

private:
    QDateTime dataTime;
    QTimer *dataTimer;
    QHBoxLayout *topHBoxLayout;
    QWidget *topWidget;
    QLabel *topDataLabel;
    QList<DesktopPage *> desktopPageList;
    QList<QLabel *> desktopPageIndicator;
    QScrollArea *scrollArea;
    QScroller *scroller;
    QHBoxLayout *hBoxLayout;
    QWidget *mainWidget;
    QTimer *timer;
    QHBoxLayout *bottomHBoxLayout;
    QWidget *bottomWidget;
    bool draggingFlag;
    int currentPageIndex;
    void layoutInit();
    void resizeEvent(QResizeEvent *event);
    void signalAndSlotInit();
    void desktopPageIndicatorAdd();
    int desktopPageIndicatorDel();
    void ARMInit();

private slots:
    void scrollerStateChanged(QScroller::State newState);
    void timerTimeout();
    void dataTimerTimeout();
    void desktopCurrentPageIndexChanged(int index);

signals:
    void currentPageIndexChanged(int index);

public slots:
};

#endif // SLIDERPAGE_H
