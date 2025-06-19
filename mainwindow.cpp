#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QToolBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *tb = addToolBar("Tools");
    QAction *AddAction = tb->addAction("Add Display ➕");
    AddAction->setToolTip("Add new Video Viewer");
    connect(AddAction, &QAction::triggered, this, &MainWindow::AddDock);

    this->setCentralWidget(MakeVideoWidget());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddDock()
{
    QString title("Video Display");
    auto *dock = new QDockWidget(title, this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(MakeVideoWidget());

    addDockWidget(Qt::RightDockWidgetArea, dock);
}

QWidget* MainWindow::MakeVideoWidget()
{
    // Connect Media player and VideoWidget
    auto *videoWidget = new QVideoWidget;
    auto *player = new QMediaPlayer(videoWidget, QMediaPlayer::StreamPlayback);
    player->setVideoOutput(videoWidget);

    // Add Try button and Video Address Line Edit
    auto *urlEdit = new QLineEdit("rtsp://172.20.0.95:6000/net0");
    auto *browseBtn = new QPushButton("Select SDP File");
    auto *playBtn = new QPushButton("Try to play");

    connect(browseBtn, &QPushButton::clicked, this, [=]() {
        QString fileName =
                QFileDialog::getOpenFileName(
                    nullptr,
                    "Select SDP File",
                    "/home/user",
                    "*.sdp"
                    );

        if (!fileName.isEmpty()) {
            urlEdit->setText(QUrl::fromLocalFile(fileName).toString());
        }
    });

    connect(playBtn, &QPushButton::clicked, this, [=]() {
        QUrl url(urlEdit->text());
        player->stop();
        player->setMedia(url);
        player->play();
    });
    playBtn->click();

    // Align Contents
    auto urlRowLayout = new QHBoxLayout;
    urlRowLayout->addWidget(urlEdit);
    urlRowLayout->addWidget(browseBtn);

    auto *layout = new QVBoxLayout;
    layout->addLayout(urlRowLayout);
    layout->addWidget(playBtn);
    layout->addWidget(videoWidget);

    auto *container = new QWidget;
    container->setLayout(layout);

    return container;
}
