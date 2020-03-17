#ifndef QCONVERTTHREAD_H
#define QCONVERTTHREAD_H

#include <QThread>
#include <QMap>
#include <QSettings>
#include "pageparam.h"

class QConvertThread : public QThread
{
    Q_OBJECT
public:
    QConvertThread(QObject *parent, const PageSetting& defaultPageSetting,
                   const QMap<int, PageSetting>& customPageSettings,
                   const QMap<int, QString>& filesToPages,
                   const QSettings* settings,
                   const QString& out_path);
    void setCurrentConversion(bool val = true) { m_curConv = val; }
private:
    void run() override;
signals:
    void progress(int page);
private:
    const PageSetting& m_defaultPageSetting;
    const QMap<int, PageSetting>& m_customPageSettings;
    const QMap<int, QString> m_filesToPages;
    const QSettings* m_settings;
    const QString& m_out_path;
    bool m_curConv = m_curConv;
};

#endif // QCONVERTTHREAD_H
