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
    void setVideoFilePath(QString videoFilePath);

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

    QString videoFilePath;
    QMediaPlayer *mediaPlayer;
    QMediaPlaylist *mediaPlaylist;

    int layoutInit();
    void resizeEvent(QResizeEvent *event);
    void signalAndSlotInit();
    void mediaInit();
    void scanVideoDirectory(QString videoFilePath);
    void ARMInit();

signals:

public slots:
    void selectVideoFilePath();
    void playVideo();
    void nextVideo();
    void mediaPlayerStateChanged(QMediaPlayer::State newState);
    void mediaPlayerDurationChanged(qint64 duration);
    void mediaPlayerPositionChanged(qint64 position);
    void videoSliderSliderReleased();
    void listWidgetItemDoubleClicked(QListWidgetItem *item);
    void listWidgetItemClicked(QListWidgetItem *item);
    void mediaPlaylistCurrentIndexChanged(int index);
    void volumeSliderReleased();
    void volumeDown();
    void volumeUp();
    void exitPage();
};

#endif // VIDEOPLAYERPAGE_H
