#include "sliderpage.h"
#include <QDebug>
#include <QCursor>
#include <QPropertyAnimation>
#include <QScrollBar>

// TODO
SliderPage::SliderPage(QWidget *parent)
    : QWidget(parent),
      draggingFlag(false),
      currentPageIndex(0)
{
    layoutInit();
    desktopPageIndicator.clear();
    timer = new QTimer(this);
    dataTimer = new QTimer(this);
    dataTimer->start(1000);
    signalAndSlotInit();
}

// TODO
void SliderPage::signalAndSlotInit() {
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    connect(scroller, SIGNAL(stateChanged(QScroller::State)), this, SLOT(scrollerStateChanged(QScroller::State)));
    connect(this, SIGNAL(currentPageIndexChanged(int)), this, SLOT(desktopCurrentPageIndexChanged(int)));
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(dataTimerTimeout()));
}

void SliderPage::dataTimerTimeout() {
    QString currentData = dataTime.currentDateTime().toString();
    topDataLabel->setText(currentData);
}

void SliderPage::ARMInit() {
#if __arm__
    setCursor(Qt::BlankCursor);
#endif
}

void SliderPage::layoutInit() {
    this->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ARMInit();

    topHBoxLayout = new QHBoxLayout();
    topHBoxLayout->setContentsMargins(0, 0, 0, 0);
    topWidget = new QWidget(this);
    topWidget->setStyleSheet("background: transparent");
    topWidget->setLayout(topHBoxLayout);
    topHBoxLayout->setAlignment(Qt::AlignCenter);
    topDataLabel = new QLabel();
    topDataLabel->setText("周一 1月 1 00:00:00 0000");
    topHBoxLayout->addWidget(topDataLabel);
    QPalette palette;
    QFont font;
    palette.setColor(QPalette::WindowText, Qt::white);
    font.setPixelSize(18);
    topDataLabel->setFont(font);
    topDataLabel->setPalette(palette);

    scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet("border: none; background-color: transparent;");

    mainWidget = new QWidget();
    mainWidget->setStyleSheet("background: transparent");
    scrollArea->setWidget(mainWidget);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scroller = QScroller::scroller(scrollArea);
    scroller->grabGesture(scrollArea, QScroller::LeftMouseButtonGesture);

    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::SnapTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::MinimumVelocity, 1);
    scroller->setScrollerProperties(properties);

    hBoxLayout = new QHBoxLayout();
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget->setLayout(hBoxLayout);

    bottomHBoxLayout = new QHBoxLayout();
    bottomHBoxLayout->setContentsMargins(0, 0, 0, 0);
    bottomWidget = new QWidget(this);
    bottomWidget->setStyleSheet("background: transparent");
    bottomWidget->setLayout(bottomHBoxLayout);
    bottomHBoxLayout->setAlignment(Qt::AlignCenter);
}

int SliderPage::getDesktopPageCount() {
    return desktopPageList.length();
}

void SliderPage::desktopCurrentPageIndexChanged(int index) {
    QLabel *label;
    for (int i = 0; i < desktopPageIndicator.length(); i++) {
        label = desktopPageIndicator[i];
        if (i == index) {
            label->setPixmap(QPixmap(":/sliderPage/icons/indicator2.png"));
        } else {
            label->setPixmap(QPixmap(":/sliderPage/icons/indicator1.png"));
        }
    }
}

// TODO
void SliderPage::timerTimeout() {
    draggingFlag = true;
    timer->stop();
}

// TODO
void SliderPage::scrollerStateChanged(QScroller::State newState) {
    static int pressedValue = 0;
    static int releasedValue = 0;
    static int pressedPageIndex = 0;

    if (desktopPageList.length() == 0)
        return;

    if (newState == QScroller::Inactive) {
        timer->stop();
        releasedValue = QCursor::pos().x();
        if (releasedValue == pressedValue)
            return;

        if (!draggingFlag) {
            if (pressedValue - releasedValue > 20) {
                currentPageIndex++;
            } else if (releasedValue - pressedValue > 20) {
                currentPageIndex--;
            }
        }
        if (currentPageIndex < 0) {
            currentPageIndex = 0;
        }

        if (currentPageIndex >= desktopPageList.length()) {
            currentPageIndex = desktopPageList.length() - 1;
        }

        QPropertyAnimation *animation = new QPropertyAnimation(scrollArea->horizontalScrollBar(), "value");
        animation->setDuration(200);
        animation->setStartValue(scrollArea->horizontalScrollBar()->value());
        animation->setEasingCurve(QEasingCurve::OutCurve);
        animation->setEndValue(currentPageIndex * this->width());
        animation->start();

        if (pressedPageIndex != currentPageIndex) {
            emit currentPageIndexChanged(currentPageIndex);
        }

    } else if (newState == QScroller::Pressed) {
        pressedPageIndex = scrollArea->horizontalScrollBar()->value() / this->width();
        releasedValue = 0;
        draggingFlag = false;
        pressedValue = QCursor::pos().x();
        timer->start(1000);
    }
}

void SliderPage::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)

    int pageCount = desktopPageList.length();
//    scrollArea->resize(this->width(), this->height());
    scrollArea->setGeometry(0, 20, this->width(), this->height() - 40);
    topWidget->setGeometry(0, 0, this->width(), 20);
    mainWidget->setGeometry(0, 20, this->width() * pageCount, this->height() - 40);
    DesktopPage *desktopPage;
    EACH_LIST(desktopPage, desktopPageList) {
        desktopPage->resize(this->size());
    }
    bottomWidget->setGeometry(0, this->height() - 20, this->width(), 20);
    if (pageCount != 0) {
        emit currentPageIndexChanged(0);
    }
}

int SliderPage::addDesktopPage(DesktopPage *desktopPage) {
    DesktopPage *desktopPageTmp;
    EACH_LIST(desktopPageTmp, this->desktopPageList) {
        if (desktopPageTmp->getName() == desktopPage->getName())
            return -1;
    }

    this->desktopPageList.append(desktopPage);
    hBoxLayout->addWidget(desktopPage);
    desktopPageIndicatorAdd();
    return 0;
}

int SliderPage::insertDesktopPage(DesktopPage *desktopPage, int index) {
    DesktopPage *desktopPageTmp;
    EACH_LIST(desktopPageTmp, this->desktopPageList) {
        if (desktopPageTmp->getName() == desktopPage->getName())
            return -1;
    }

    desktopPageList.insert(index, desktopPage);
    hBoxLayout->addWidget(desktopPage);
    desktopPageIndicatorAdd();

    return 0;
}

void SliderPage::desktopPageIndicatorAdd() {
    QLabel *label = new QLabel();
    desktopPageIndicator.append(label);
    label->setPixmap(QPixmap(":/sliderPage/icons/indicator1.png"));
    bottomHBoxLayout->addWidget(label);
}

int SliderPage::desktopPageIndicatorDel() {
    if (desktopPageIndicator.empty()) {
        return -1;
    }
    desktopPageIndicator.pop_back();
    return 0;
}

int SliderPage::delDesktopPageFromIndex(int index) {
    if (desktopPageList.length() >= index) {
        return -1;
    }

    hBoxLayout->removeWidget(desktopPageList.at(index));
    desktopPageList.removeAt(index);
    desktopPageIndicatorDel();
    return 0;
}

int SliderPage::delDesktopPageFromName(QString name) {
    DesktopPage *desktopPage;
    EACH_LIST(desktopPage, this->desktopPageList) {
        if (desktopPage->getName() == name) {
            hBoxLayout->removeWidget(desktopPage);
            desktopPageList.removeOne(desktopPage);
            desktopPageIndicatorDel();
            return 0;
        }
    }
    return -1;
}

DesktopPage *SliderPage::getDesktopPageFromIndex(int index) {
    return desktopPageList.at(index);
}

DesktopPage *SliderPage::getDesktopPageFromName(QString name) {
    DesktopPage *desktopPage;
    EACH_LIST(desktopPage, this->desktopPageList) {
        if (desktopPage->getName() != name) {
            continue;
        }
        return desktopPage;
    }
    return nullptr;
}
