#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QDebug>
#include <QStorageInfo>
#include <QDragEnterEvent>
#include <QInputDialog>
#include <QMessageBox>

#include "threadpoolmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QFileSystemModel model1;
    QFileSystemModel model2;

    ThreadPoolManager manager;

    void Update1();

private slots:
    void comboBoxActivated1(int index);
    void comboBoxActivated2(int index);
    void pushButtonNewFolderLClik();
    void pushButtonDelFolderLClik();
    void pushButtonNewFolderRClik();
    void pushButtonDelFolderRClik();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // MAINWINDOW_H
