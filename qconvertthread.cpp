#include "qconvertthread.h"
#include <QStandardPaths>
#include <QApplication>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

#include "utils.h"
#include "mainwindow.h"

// The first chunk of 4:
#define BASE_CHUNK 70

QConvertThread::QConvertThread(QObject *parent, const PageSetting& defaultPageSetting,
                               const QMap<int, PageSetting>& customPageSettings, const QMap<int, QString> &filesToPages, const QSettings *settings, const QString &out_path)
    : QThread(parent),
      m_defaultPageSetting(defaultPageSetting),
      m_customPageSettings(customPageSettings),
      m_filesToPages(filesToPages),
      m_settings(settings),
      m_out_path(out_path)
{
    m_curConv = false;
}

void QConvertThread::run()
{

    const QString c44_fi = Utils::findExecutable("c44-fi");
    if (c44_fi.isEmpty()) {
        return;
    }

    int dpi = 0;
    if (m_settings->value("changeDPI", false).toBool()) {
        dpi = m_settings->value("DPI", 600).toInt();
    }


    PageSetting curSetting;
    QFileInfo fi;
    int cnt = 0;
    for(QMap<int, QString>::const_iterator it = m_filesToPages.constBegin();
        it != m_filesToPages.constEnd(); ++it) {

        QString command_line = c44_fi;

        const int page = it.key();
        curSetting = m_customPageSettings.contains(page) ?
                    m_customPageSettings[page] : m_defaultPageSetting;

        if (curSetting.reqBackg) {
            command_line += QString(" -slice %1 ").arg(BASE_CHUNK + curSetting.valBackg);
        }

        if (curSetting.reqBSF) {
            command_line += QString(" -bsf %1 ").arg(curSetting.valBSF);
        }

        if (dpi) {
            command_line += QString(" -dpi %1 ").arg(dpi);
        }

        fi.setFile(*it);
        const QString dest_file = m_out_path + fi.completeBaseName() + ".djv";
        command_line += QString(" \"%1\" \"%2\" ")
                .arg(*it, dest_file);

        if (!Utils::execute(command_line)) {
            return;
        }

        emit progress(++cnt);

        if (m_curConv || m_settings->value("openAfterConv", false).toBool()) {
            if (!QDesktopServices::openUrl(QUrl(dest_file))) {
                emit MainWindow::_top_widget_->error(tr("Error"), tr("Can't find an application to open %1").arg(dest_file));
            }
        }
    }
}
