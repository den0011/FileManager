#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QDebug>
#include <QStorageInfo>
#include <QDragEnterEvent>

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

    void Update1();

private slots:
    void comboBoxActivated1(int index);
    void comboBoxActivated2(int index);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // MAINWINDOW_H
