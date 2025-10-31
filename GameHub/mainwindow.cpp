#include "mainwindow.h"

#include <qcoreapplication.h>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("🎮 GameHub");
    resize(400, 300);

    QWidget *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    auto *btn1 = new QPushButton("99matrix");
    auto *btn2 = new QPushButton("Minesweeper");
    auto *btn3 = new QPushButton("Tetris");

    layout->addWidget(btn1);
    layout->addWidget(btn2);
    layout->addWidget(btn3);

    setCentralWidget(central);

    connect(btn1, &QPushButton::clicked, this, &MainWindow::launch99matrix);
    connect(btn2, &QPushButton::clicked, this, &MainWindow::launchMinesweeper);
    connect(btn3, &QPushButton::clicked, this, &MainWindow::launchSnake);
}

MainWindow::~MainWindow() = default;

void MainWindow::launch99matrix() {
    QString path = QCoreApplication::applicationDirPath() + "/games/99matrix";
    qDebug() << "Launching:" << path;
    QProcess::startDetached(path);
}

void MainWindow::launchMinesweeper() {
    QString path = QCoreApplication::applicationDirPath() + "/games/mines_sweeper";
    qDebug() << "Launching:" << path;
    QProcess::startDetached(path);
}

void MainWindow::launchSnake() {
    QString path = QCoreApplication::applicationDirPath() + "/games/Tetris";
    qDebug() << "Launching:" << path;
    QProcess::startDetached(path);
}
