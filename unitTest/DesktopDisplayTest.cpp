#include "desktopsystem.h"

#include <QApplication>
#include <brightnessControlPage/brightnesscontrolpage.h>
#include <videoPlayerPage/videoplayerpage.h>

#if 1

static DesktopSystem *desktopSystem;

void lightClickedFunc() {
    BrightnessControlPage *w = new BrightnessControlPage();
    w->setGeometry(desktopSystem->geometry());
    w->show();
}

void videoClickedFunc() {
    VideoPlayerPage *w = new VideoPlayerPage();
    w->setGeometry(desktopSystem->geometry());
    w->show();
}

void ARMInit() {
#if __arm__
    QApplication::setOverrideCursor(Qt::BlankCursor);
#endif
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ARMInit();

    DesktopSystem w;
    desktopSystem = &w;
    DesktopPage *desktopPage[3];
    w.setDesktopBackground(":/images/bg.png");
    for (int i = 0; i < 3; i++) {
        desktopPage[i] = new DesktopPage(QString::number(i));
        desktopPage[i]->addApplication(new Application(":/application/icons/video-player.png", "video", videoClickedFunc));
        desktopPage[i]->addApplication(new Application(":/application/icons/brightness-control.png", "blight", lightClickedFunc));
        desktopPage[i]->addApplication(new Application(":/application/icons/temp-and-humidity.png", "t&h", videoClickedFunc));
        desktopPage[i]->addApplication(new Application(":/application/icons/light-control.png", "light", lightClickedFunc));
        w.addDesktopPage(desktopPage[i]);
    }

    w.show();
    return a.exec();
}

#endif
