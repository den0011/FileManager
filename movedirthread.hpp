#include <QThread>
#include <QDir>

class MoveDirThread : public QThread
{
    Q_OBJECT

public:
    MoveDirThread(QObject *parent = nullptr)
        : QThread(parent)
    {
        m_cancelled = false;
    }

    void setSourceDir(const QString &dirPath)
    {
        m_sourceDir = dirPath;
    }

    void setDestDir(const QString &dirPath)
    {
        m_destDir = dirPath;
    }

    void cancel()
    {
        m_cancelled = true;
    }

signals:
    void finished();
    void error(const QString &message);

protected:
    void run() override
    {
        if (m_sourceDir.isEmpty() || m_destDir.isEmpty()) {
            emit error("Source or destination directory is not set");
            return;
        }

        QDir sourceDir(m_sourceDir);
        if (!sourceDir.exists()) {
            emit error("Source directory does not exist");
            return;
        }

        QDir destDir(m_destDir);
        if (!destDir.exists()) {
            if (!destDir.mkpath(".")) {
                emit error("Failed to create destination directory");
                return;
            }
        }

        QStringList files = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &file : files) {
            if (m_cancelled) {
                break;
            }

            QString srcFilePath = m_sourceDir + QDir::separator() + file;
            QString destFilePath = m_destDir + QDir::separator() + file;

            if (QFileInfo(srcFilePath).isDir()) {
                MoveDirThread *subThread = new MoveDirThread(this);
                subThread->setSourceDir(srcFilePath);
                subThread->setDestDir(destFilePath);
                connect(subThread, &MoveDirThread::finished, subThread, &MoveDirThread::deleteLater);
                connect(subThread, &MoveDirThread::error, this, &MoveDirThread::error);
                subThread->start();

                while (!subThread->isFinished()) {
                    msleep(100); // добавляем задержку в 100 мс между копированием директорий
                    if (m_cancelled) {
                        subThread->cancel();
                        break;
                    }
                }
            } else {
                if (!QFile::rename(srcFilePath, destFilePath)) {
                    emit error("Failed to move file " + file);
                }
            }
        }

        if (!m_cancelled) {
            if (!sourceDir.rmdir(".")) {
                emit error("Failed to remove source directory");
            }
        }

        emit finished();
    }

private:
    QString m_sourceDir;
    QString m_destDir;
    bool m_cancelled;
};
