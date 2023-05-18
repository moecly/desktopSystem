#ifndef DESKTOPPAGE_H
#define DESKTOPPAGE_H

#include <QWidget>
#include <QPushButton>
#include <utils/common.h>
#include <application/application.h>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DesktopPage : public QWidget
{
    Q_OBJECT
public:
    DesktopPage(QWidget *parent = nullptr);
    DesktopPage(QString name, QWidget *parent = nullptr);
    QString getName();
    void setName(QString name);
    int addApplication(Application *app);

private:
    int applicationNumber;
    QGridLayout *gridLayout;
    QWidget *mainWidget;
    QList<Application *> appList;
    QString name;
    void layoutInit();
    void resizeEvent(QResizeEvent *event);
    int getApplicationLocation(int *row, int *col);

signals:

public slots:
};

#endif // DESKTOPPAGE_H
