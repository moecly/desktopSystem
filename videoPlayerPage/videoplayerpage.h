#ifndef VIDEOPLAYERPAGE_H
#define VIDEOPLAYERPAGE_H

#include <QWidget>
#include <utils/common.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QSlider>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QFile>

#define DEFAULT_VIDEO_FILE_NAME "~/"

class VideoPlayerPage : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPlayerPage(QWidget *parent = nullptr);
    int setVideoFileName(QString videoFileName);

private:
    /*
     * @params hBoxLayout[0]: 播放器和播放列表
     * @params hBoxLayout[1]: 播放进度条下的按钮
     */
    QHBoxLayout *hBoxLayout[2];
    QWidget *hWidget[2];

    QVideoWidget *videoWidget;
    QListWidget *listWidget;

    /*
     * @params slider[0]: 播放进度条
     * @params slider[1]: 音量条
     */
    QSlider *slider[2];

    /*
     * @params label[0]: 当前播放时长
     * @params label[1]: 视频总时长
     */
    QLabel *label[2];

    /*
     * @params pushButton[0]: 播放键
     * @params pushButton[1]: 下一首
     * @params pushButton[2]: 音量减
     * @params pushButton[3]: 音量加
     * @params pushButton[4]: 退出
     * @params pushButton[5]: 选择文件路劲
     */
    QPushButton *pushButton[6];

    /*
     * 整体layout.
     */
    QVBoxLayout *vBoxLayout;
    QWidget *mainWidget;

    QFile videoFile;

    int layoutInit();
    void resizeEvent(QResizeEvent *event);
    void signalAndSlotInit();

signals:

public slots:
    void selectVideoFileName();
};

#endif // VIDEOPLAYERPAGE_H
