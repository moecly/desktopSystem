#include "videoplayerpage.h"
#include <QFileDialog>

VideoPlayerPage::VideoPlayerPage(QWidget *parent) : QWidget(parent)
{
    layoutInit();
    setVideoFileName(DEFAULT_VIDEO_FILE_NAME);
    signalAndSlotInit();
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
    pushButton[5]->setCheckable(true);

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
    connect(pushButton[5], SIGNAL(clicked()), this, SLOT(selectVideoFileName()));
}

void VideoPlayerPage::selectVideoFileName() {
    pushButton[5]->setChecked(true);
    QString videoFileName = QFileDialog::getExistingDirectory();
    pushButton[5]->setChecked(false);
    if (videoFileName != "") {
        setVideoFileName(videoFileName);
        qDebug() << "videoFileName = " << videoFileName;
    }
}

int VideoPlayerPage::setVideoFileName(QString videoFileName) {
    QFile file(videoFileName);
    if (!file.exists())
        return -1;
    videoFile.setFileName(videoFileName);
    return 0;
}

void VideoPlayerPage::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)

    mainWidget->resize(this->size());
}
