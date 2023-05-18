#include "videoplayerpage.h"
#include <QFileDialog>

VideoPlayerPage::VideoPlayerPage(QWidget *parent) : QWidget(parent)
{
    layoutInit();
    mediaInit();
    setVideoFilePath(DEFAULT_VIDEO_FILE_NAME);
    signalAndSlotInit();
}

void VideoPlayerPage::mediaInit() {
    mediaPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this);
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    mediaPlayer->setPlaylist(mediaPlaylist);
    mediaPlayer->setVideoOutput(videoWidget);
    mediaPlayer->setVolume(slider[1]->value());
}

int VideoPlayerPage::layoutInit() {
    for (int i = 0; i < 2; i++) {
        hBoxLayout[i] = new QHBoxLayout();
        hBoxLayout[i]->setContentsMargins(0, 0, 0, 0);
    }
    hBoxLayout[0]->setAlignment(Qt::AlignCenter);
    hBoxLayout[1]->setAlignment(Qt::AlignLeft);

    for (int i = 0; i < 2; i++) {
        hWidget[i] = new QWidget();
        hWidget[i]->setLayout(hBoxLayout[i]);
        hWidget[i]->setObjectName(QString("hWidget%1").arg(i));
    }

    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet("border-image: none;"
                               "background: transparent;"
                               "border:none");
    listWidget = new QListWidget();
    listWidget->setMinimumSize(100, 300);
    listWidget->setMaximumWidth(200);
    listWidget->setObjectName("listWidget");

    vBoxLayout = new QVBoxLayout();
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setAlignment(Qt::AlignCenter);
    mainWidget = new QWidget(this);
    mainWidget->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    mainWidget->setObjectName("mainWidget");
    this->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    for (int i = 0; i < 2; i++) {
        slider[i] = new QSlider(Qt::Horizontal);
        slider[i]->setObjectName(QString("slider%1").arg(i));
        slider[i]->setMaximumHeight(15);
        slider[i]->setMinimumHeight(15);
    }
    slider[1]->setRange(0, 100);
    slider[1]->setValue(100);
    slider[1]->setMaximumWidth(120);
    slider[1]->setMinimumWidth(120);

    for (int i = 0; i < 2; i++) {
        label[i] = new QLabel();
        label[i]->setObjectName(QString("label%1").arg(i));
    }
    label[0]->setText("00:00");
    label[1]->setText("/00:00");

    mainWidget->setLayout(vBoxLayout);
    vBoxLayout->addWidget(hWidget[0]);
    vBoxLayout->addWidget(slider[0]);
    vBoxLayout->addWidget(hWidget[1]);

    hBoxLayout[0]->addWidget(videoWidget);
    hBoxLayout[0]->addWidget(listWidget);

    for (int i = 0; i < 6; i++) {
        pushButton[i] = new QPushButton();
        pushButton[i]->setMinimumSize(60, 60);
        pushButton[i]->setMaximumSize(60, 60);
        pushButton[i]->setObjectName(QString("pushButton%1").arg(i));
    }
    pushButton[0]->setCheckable(true);
    pushButton[0]->setChecked(false);
    pushButton[5]->setCheckable(true);
    pushButton[5]->setChecked(false);

    QSpacerItem *hSpacer0 = new QSpacerItem(200, 60, QSizePolicy::Expanding, QSizePolicy::Maximum);
    hBoxLayout[1]->addWidget(pushButton[0]);
    hBoxLayout[1]->addWidget(pushButton[1]);
    hBoxLayout[1]->addWidget(pushButton[2]);
    hBoxLayout[1]->addWidget(slider[1]);
    hBoxLayout[1]->addWidget(pushButton[3]);
    hBoxLayout[1]->addWidget(pushButton[5]);
    hBoxLayout[1]->addWidget(label[0]);
    hBoxLayout[1]->addWidget(label[1]);
    hBoxLayout[1]->addSpacerItem(hSpacer0);
    hBoxLayout[1]->addWidget(pushButton[4]);

    QFile file(":/videoPlayerPage/style/videoplayerpage.qss");
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
        return -1;

    QString styleSheet = file.readAll();
    this->setStyleSheet(styleSheet);

    file.close();

    return 0;
}

void VideoPlayerPage::signalAndSlotInit() {
    connect(pushButton[5], SIGNAL(clicked()), this, SLOT(selectVideoFilePath()));
    connect(pushButton[0], SIGNAL(clicked()), this, SLOT(playVideo()));
    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(mediaPlayerDurationChanged(qint64)));
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(mediaPlayerPositionChanged(qint64)));
    connect(slider[0], SIGNAL(sliderReleased()), this, SLOT(videoSliderSliderReleased()));
    connect(pushButton[1], SIGNAL(clicked()), this, SLOT(nextVideo()));
    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(listWidgetItemDoubleClicked(QListWidgetItem *)));
    connect(mediaPlaylist, SIGNAL(currentIndexChanged(int)), this, SLOT(mediaPlaylistCurrentIndexChanged(int)));
    connect(slider[1], SIGNAL(sliderReleased()), this, SLOT(volumeSliderReleased()));
    connect(pushButton[2], SIGNAL(clicked()), this, SLOT(volumeDown()));
    connect(pushButton[3], SIGNAL(clicked()), this, SLOT(volumeUp()));
    connect(pushButton[4], SIGNAL(clicked()), this, SLOT(exitPage()));
}

void VideoPlayerPage::volumeDown() {
    int value = slider[1]->value();
    if (value - 10 < 0)
        value = 0;
    else
        value -= 10;
    slider[1]->setValue(value);
    mediaPlayer->setVolume(value);
}

void VideoPlayerPage::volumeUp() {
    int value = slider[1]->value();
    if (value + 10 > 100)
        value = 100;
    else
        value += 10;
    slider[1]->setValue(value);
    mediaPlayer->setVolume(value);
}

void VideoPlayerPage::exitPage() {
    this->close();
}

void VideoPlayerPage::volumeSliderReleased() {
    mediaPlayer->setVolume(slider[1]->value());
}

void VideoPlayerPage::listWidgetItemDoubleClicked(QListWidgetItem *item) {
    int index = listWidget->row(item);
    mediaPlayer->stop();
    listWidget->setCurrentRow(index);
    mediaPlaylist->setCurrentIndex(index);
    mediaPlayer->play();
}

void VideoPlayerPage::mediaPlaylistCurrentIndexChanged(int index) {
    if (index < 0)
        return;
    listWidget->setCurrentRow(index);
}

void VideoPlayerPage::nextVideo() {
    mediaPlaylist->next();
}

void VideoPlayerPage::videoSliderSliderReleased() {
    mediaPlayer->setPosition(slider[0]->value() * 1000);
}

void VideoPlayerPage::mediaPlayerDurationChanged(qint64 duration) {
    duration /= 1000;
    qint64 minute = duration / 60;
    qint64 sec = duration % 60;
    QString str = "/";
    slider[0]->setRange(0, (int)duration);

    if (minute >= 10) {
        str += QString::number(minute);
    } else {
        str += "0" + QString::number(minute);
    }

    str += ":";

    if (sec >= 10) {
        str += QString::number(sec);
    } else {
        str += "0" + QString::number(sec);
    }

    label[1]->setText(str);
}

void VideoPlayerPage::mediaPlayerPositionChanged(qint64 position) {
    position /= 1000;
    qint64 minute = position / 60;
    qint64 sec = position % 60;
    QString str = "";
    slider[0]->setValue((int)position);

    if (minute >= 10) {
        str += QString::number(minute);
    } else {
        str += "0" + QString::number(minute);
    }

    str += ":";

    if (sec >= 10) {
        str += QString::number(sec);
    } else {
        str += "0" + QString::number(sec);
    }

    label[0]->setText(str);
}

void VideoPlayerPage::playVideo() {
    if (mediaPlaylist->isEmpty()) {
        pushButton[0]->setChecked(false);
        return;
    }

    QMediaPlayer::State state = mediaPlayer->state();
    if (state == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
    } else if (state == QMediaPlayer::PausedState) {
        mediaPlayer->play();
    } else if (state == QMediaPlayer::StoppedState) {
        listWidget->setCurrentRow(0);
        mediaPlaylist->setCurrentIndex(0);
        mediaPlayer->play();
    }
}

void VideoPlayerPage::mediaPlayerStateChanged(QMediaPlayer::State newState) {
    switch(newState) {
        case QMediaPlayer::StoppedState:
            pushButton[0]->setChecked(false);
            break;
        case QMediaPlayer::PlayingState:
            pushButton[0]->setChecked(true);
            break;
        case QMediaPlayer::PausedState:
            pushButton[0]->setChecked(false);
            break;
    }
}

void VideoPlayerPage::scanVideoDirectory(QString videoFilePath) {
    mediaPlayer->stop();
    mediaPlaylist->clear();
    listWidget->clear();

    QDir directory(videoFilePath);
    QStringList videoFilters;
    videoFilters << "*.mp4" << "*.avi" << "*.mkv";
    QStringList videoFiles = directory.entryList(videoFilters, QDir::Files);

    foreach (const QString &videoFile, videoFiles) {
        QString absoluteFilePath = directory.absoluteFilePath(videoFile);
        if (!mediaPlaylist->addMedia(QUrl::fromLocalFile(absoluteFilePath))) {
            continue;
        }
        listWidget->addItem(videoFile);
    }
}

void VideoPlayerPage::selectVideoFilePath() {
    pushButton[5]->setChecked(true);
    QString videoFilePath = QFileDialog::getExistingDirectory();
    pushButton[5]->setChecked(false);
    if (videoFilePath != "") {
        setVideoFilePath(videoFilePath);
        scanVideoDirectory(videoFilePath);
    }
}

void VideoPlayerPage::setVideoFilePath(QString videoFilePath) {
    this->videoFilePath = videoFilePath;
}

void VideoPlayerPage::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    mainWidget->resize(this->size());
}
