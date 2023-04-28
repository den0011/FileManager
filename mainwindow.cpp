#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/icon/res/filemanager.png"));
    setWindowTitle(QString("FileManager"));

    QDir mdir;
    foreach (QFileInfo a, mdir.drives()){
        ui->comboBox1->addItem(a.absoluteFilePath());
        ui->comboBox2->addItem(a.absoluteFilePath());
    }

    connect(ui->comboBox1, SIGNAL(activated(int)), this, SLOT(comboBoxActivated1(int)));
    connect(ui->comboBox2, SIGNAL(activated(int)), this, SLOT(comboBoxActivated2(int)));
    comboBoxActivated1(0);
    comboBoxActivated2(0);

    ui->treeView1->setDragEnabled(true);
    ui->treeView1->setAcceptDrops(true);
    ui->treeView1->setDropIndicatorShown(true);
    ui->treeView1->setDragDropMode(QAbstractItemView::InternalMove);

    ui->treeView2->setDragEnabled(true);
    ui->treeView2->setAcceptDrops(true);
    ui->treeView2->setDropIndicatorShown(true);
    ui->treeView2->setDragDropMode(QAbstractItemView::InternalMove);

    ui->comboBox1->setToolTip("Update");


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::comboBoxActivated1(int index)
{
    QString dir =  ui->comboBox1->itemData(index,Qt::DisplayRole).toString();
    qDebug() <<dir;

    model1.setRootPath(dir);

    ui->treeView1->setModel(&model1);

    ui->treeView1->setRootIndex(model1.index(dir));

    QStorageInfo storage(dir);
    qDebug() << "Name:" << storage.name();
    qDebug() << "Display name:" << storage.displayName();
    qDebug() << "File system type:" << storage.fileSystemType();
    qDebug() << "Is read-only:" << storage.isReadOnly();
    qDebug() << "Is root:" << storage.isRoot();
    qDebug() << "Size:" << storage.bytesTotal()/1000000 << "MB";
    qDebug() << "Free space:" << storage.bytesFree()/1000000 << "MB";
    qDebug() << "Available space:" << storage.bytesAvailable()/1000000 << "MB";

    qDebug() << "name:" << storage.name();
    qDebug() << "fileSystemType:" << storage.fileSystemType();
    qDebug() << "size:" << storage.bytesTotal()/1024/1024/1024 << "MB";
    qDebug() << "availableSize:" << storage.bytesAvailable()/1024/1024/1024 << "MB";
        qDebug() << "Free space:" << storage.bytesFree()/1024/1024/1024 << "MB";

}

void MainWindow::comboBoxActivated2(int index)
{
    QString dir =  ui->comboBox2->itemData(index,Qt::DisplayRole).toString();
    qDebug() <<dir;

    model2.setRootPath(dir);

    ui->treeView2->setModel(&model2);

    ui->treeView2->setRootIndex(model2.index(dir));
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{



}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{

}

void MainWindow::dropEvent(QDropEvent *event)
{

}
