#ifndef QINSERTTHREAD_H
#define QINSERTTHREAD_H

#include <QThread>
#include <QMap>

class QInsertThread : public QThread
{
    Q_OBJECT
public:
    QInsertThread(QObject *parent,
                   const QMap<int, QString>& filesToPages,
                   const QString& tmpImagesDir,
                   const QString& tmpFilesDir,
                   const QString& sourceDjVu,
                   const QString& destDjVu, bool openAfterInsert);
private:
    void run() override;
signals:
    void progress(int page);
private:
    const QMap<int, QString> m_filesToPages;
    const QString m_tmpImagesDir;
    const QString m_tmpFilesDir;
    const QString m_sourceDjVu;
    const QString m_destDjVu;
    bool m_openAfterInsert;
};

#endif // QINSERTTHREAD_H
