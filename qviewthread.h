#ifndef QVIEWTHREAD_H
#define QVIEWTHREAD_H

#include <QThread>
#include <QMap>

class QViewThread : public QThread
{
    Q_OBJECT
public:
    QViewThread(QObject *parent, const QMap<int, QString> filesToPages, const QString &out_path);
private:
    void run() override;
private:
    const QMap<int, QString> m_filesToPages;
    const QString m_out_path;
};

#endif // QVIEWTHREAD_H
