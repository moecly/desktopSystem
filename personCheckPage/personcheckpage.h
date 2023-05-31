#ifndef PERSONCHECKPAGE_H
#define PERSONCHECKPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <utils/common.h>
#include <QTimer>
#include <personCheckDev/hc_sr501.h>

class PersonCheckPage : public QWidget
{
    Q_OBJECT
public:
    explicit PersonCheckPage(QWidget *parent = nullptr);

private:
    QWidget *mainWidget;
    QVBoxLayout *mainVBoxLayout;
    QLabel *stateLabel;
    QPushButton *exitPushButton;
    Hc_Sr501 *hc_sr501;
    QTimer *timer;

    void layoutInit();
    void paintEvent(QPaintEvent *event);
    void signalAndSlotInit();
    void devInit();
signals:

public slots:
    void exitPushButtonClicked();
    void devStateChanged(unsigned int state);
    void timerTimeout();
};

#endif // PERSONCHECKPAGE_H
