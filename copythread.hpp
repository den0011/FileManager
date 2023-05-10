#include <QThread>
#include <QFileInfo>
#include <QDir>

class CopyThread : public QThread {
    Q_OBJECT

signals:
    void updateProgress(qint64 current, qint64 total);
    void copyFinished(bool success);
    void copyCancelled();

public:
    CopyThread(QString source, QString destination) :
        m_source(source), m_destination(destination) {}

    void cancelCopy() {
        m_cancelled = true;
    }

protected:
    void run() {
        QFileInfo sourceFileInfo(m_source);
        if (sourceFileInfo.isDir()) {
            QDir destinationDir(m_destination);
            destinationDir.mkdir(sourceFileInfo.fileName());

            QDir sourceDir(m_source);
            QStringList fileList = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

            for (int i = 0; i < fileList.size(); i++) {
                QString fileName = fileList.at(i);
                QString sourceFilePath = m_source + "/" + fileName;
                QString destinationFilePath = m_destination + "/" + sourceFileInfo.fileName() + "/" + fileName;

                QFileInfo fileInfo(sourceFilePath);
                if (fileInfo.isDir()) {
                    CopyThread copyThread(sourceFilePath, destinationFilePath);
                    connect(&copyThread, SIGNAL(updateProgress(qint64, qint64)), this, SIGNAL(updateProgress(qint64, qint64)));
                    connect(&copyThread, SIGNAL(copyFinished(bool)), this, SIGNAL(copyFinished(bool)));
                    connect(this, SIGNAL(copyCancelled()), &copyThread, SLOT(cancelCopy()));
                    copyThread.start();
                    copyThread.wait();
                } else {
                    if (m_cancelled) {
                        emit copyCancelled();
                        return;
                    }

                    if (!QFile::copy(sourceFilePath, destinationFilePath)) {
                        emit copyFinished(false);
                        return;
                    }

                    emit updateProgress(sourceFileInfo.size(), sourceFileInfo.size());
                }
            }
        } else {
            if (!QFile::copy(m_source, m_destination)) {
                emit copyFinished(false);
                return;
            }

            emit updateProgress(sourceFileInfo.size(), sourceFileInfo.size());
        }

        emit copyFinished(true);
    }

private:
    QString m_source;
    QString m_destination;
    bool m_cancelled = false;
};
