#ifndef APPLICATION_H
#define APPLICATION_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class Application : public QWidget
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = nullptr);
    explicit Application(QString appIconUrl, QString appName, QWidget *parent = nullptr);
    explicit Application(QString appIconUrl, QString appName, void (*clickedFunc)(), QWidget *parent = nullptr);
    QString getAppIconUrl();
    QString getAppName();
    void setAppIconUrl(QString appIconUrl);
    void setAppName(QString appName);
    void setAppLink(void (*clickedFunc)());
    void enableAppLink();
    void disableAppLink();

private:
    void (*appLinkFunc)();
    QString appIconUrl;
    QString appName;
    QPushButton *pushButton;
    QLabel *label;
    QVBoxLayout *VBoxLayout;
    QWidget *mainWidget;
    void layoutInit();

signals:

public slots:
    void clickedFunc();
};

#endif // APPLICATION_H
