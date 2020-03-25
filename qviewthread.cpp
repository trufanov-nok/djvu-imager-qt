#include "qviewthread.h"
#include <QStandardPaths>
#include <QApplication>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

#include "utils.h"
#include "mainwindow.h"

QViewThread::QViewThread(QObject *parent, const QMap<int, QString> filesToPages, const QString &out_path)
    : QThread(parent),
      m_filesToPages(filesToPages),
      m_out_path(out_path)
{
}

void QViewThread::run()
{

    QString djvm = Utils::findExecutable("djvm");
    if (djvm.isEmpty()) {
        return;
    }

    const QString view_file = m_out_path + "view.djvu";
    if (QFile::exists(view_file)) {
        if (!QFile::remove(view_file)) {
            emit MainWindow::_top_widget_->error(tr("Error"),
                                  tr("Can't remove %1").arg(view_file));
            return;
        }
    }

    QFileInfo fi;
    QStringList files;
    for(QMap<int, QString>::const_iterator it = m_filesToPages.constBegin();
        it != m_filesToPages.constEnd(); ++it) {
        fi.setFile(*it);
        QString fname = m_out_path + fi.completeBaseName() + ".djv";
        if (!QFile::exists(fname)) {
            emit MainWindow::_top_widget_->error(tr("Error"),
                                  tr("The file %1 is not found.").arg(fname));
            return;
        }
        files.append("\"" + fname + "\"");
    }

    QString command_line = QString("%1 -c %2 %3").arg(djvm).arg(view_file).arg(files.join(" "));
    if (!Utils::execute(command_line)) {
        return;
    }

    if (!QDesktopServices::openUrl(QUrl(view_file))) {
        emit MainWindow::_top_widget_->error(tr("Error"), tr("Can't find an application to open %1").arg(view_file));
    }

}
