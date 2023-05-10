#include <QThread>
#include <QProgressDialog>
#include <QDir>
#include <QDebug>
#include <QFileInfo>

class DirectoryRemover : public QThread {
    Q_OBJECT
public:
    explicit DirectoryRemover(QObject *parent = nullptr) : QThread(parent) {}

    void setDirectory(const QString& dir) {
        m_dir = dir;
    }

signals:
    void finished();
    void canceled();
    void progressData(int value);

protected:
    void run() override {
        QDir dir(m_dir);
        qint64 totalSize = calculateSize(dir);

        QProgressDialog progress("Removing directory...", "Cancel", 0, 100, qobject_cast<QWidget*>(parent()));
        progress.setWindowModality(Qt::WindowModal);

        connect(&progress, &QProgressDialog::canceled, this, &DirectoryRemover::cancel);

        int deletedFilesCount = removeDirectory(dir, totalSize, &progress);

        progress.setValue(100);
        progress.hide();

        emit finished();

        QString message = QString("Deleted %1 files from %2").arg(deletedFilesCount).arg(m_dir);
        qDebug() << message;
    }

private:
    QString m_dir;

    qint64 calculateSize(const QDir& dir) {
        qint64 size = 0;
        QFileInfoList fileList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::System | QDir::Hidden);
        foreach (QFileInfo fileInfo, fileList) {
            size += fileInfo.size();
        }
        QFileInfoList folderList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::System | QDir::Hidden);
        foreach (QFileInfo folderInfo, folderList) {
            size += calculateSize(QDir(folderInfo.absoluteFilePath()));
        }
        return size;
    }

    int removeDirectory(QDir dir, qint64 totalSize, QProgressDialog* progress) {
        int deletedFilesCount = 0;
        QFileInfoList fileList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::System | QDir::Hidden);
        foreach (QFileInfo fileInfo, fileList) {
            if (progress->wasCanceled()) {
                emit canceled();
                return deletedFilesCount;
            }
            if (QFile::remove(fileInfo.absoluteFilePath())) {
                ++deletedFilesCount;
            }
            qint64 currentSize = fileInfo.size();
            totalSize -= currentSize;
            int currentProgress = (totalSize > 0) ? (100 * (totalSize - dir.count()) / totalSize) : 100;
            progress->setValue(currentProgress);
            emit progressData (currentProgress);
            msleep(10);
        }
        QFileInfoList folderList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::System | QDir::Hidden | QDir::NoSymLinks);
        foreach (QFileInfo folderInfo, folderList) {
            QString folderName = folderInfo.absoluteFilePath();
            int subFilesCount = removeDirectory(QDir(folderName), totalSize, progress);
            deletedFilesCount += subFilesCount;
            if (!QDir().rmdir(folderName)) {
                emit canceled();
                return deletedFilesCount;
            }
            progress->setValue(100 * (totalSize - dir.count()) / totalSize);
            emit progressData (progress->value());
            msleep(10);
        }
        return deletedFilesCount;
    }

    void cancel() {
        qDebug() << "cansel";
        emit canceled();
    }
};
