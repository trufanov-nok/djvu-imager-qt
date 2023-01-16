#include "utils.h"

#include <QApplication>
#include <QStandardPaths>
#include <QMessageBox>
#include <QProcess>
#include <QRegularExpression>

#include "mainwindow.h"

namespace Utils {

QString findExecutable(const QString& name)
{
    QString exec = QStandardPaths::findExecutable(name, QStringList() << QApplication::applicationDirPath()+"/tools");
    if (exec.isEmpty()) {
        exec = QStandardPaths::findExecutable(name); // search in system paths
        if (exec.isEmpty()) {
            emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                                 name != "c44-fi" ? QObject::tr("Can't find %1 executable. Please install DjVuLibre.").arg(name) :
                                                                    QObject::tr("Can't find c44-fi encoder."));
        }
    }
    return exec;
}


bool execute_with_work_dir_hack(const QString& cmd, const QString& work_dir)
{
    // bcs of https://sourceforge.net/p/djvu/bugs/316/
    QProcess proc;
    int res = -1;
    proc.setWorkingDirectory(work_dir);
    proc.setProcessChannelMode(QProcess::ProcessChannelMode::SeparateChannels);
    proc.start(cmd);
    proc.waitForFinished(-1);
    if (proc.exitStatus() == QProcess::NormalExit) {
        res = proc.exitCode();
    }
    if (res != 0) {
        emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                             QObject::tr("Command execution:\n%1\nreturn %2")
                                             .arg(cmd)
                                             .arg(res));
        return false;
    }
    return true;
}

bool execute(const QString& cmd)
{
    QProcess proc;
    int res = -1;
    proc.setProcessChannelMode(QProcess::ProcessChannelMode::SeparateChannels);
    proc.start(cmd);
    proc.waitForFinished(-1);
    if (proc.exitStatus() == QProcess::NormalExit) {
        res = proc.exitCode();
    }
    if (res != 0) {
        emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                             QObject::tr("Command execution:\n%1\nreturn %2")
                                             .arg(cmd)
                                             .arg(res));
    }


    return res == 0;
}

bool execute(const QString& cmd, QString& stdOut)
{
    QProcess proc;
    int res = -1;
    proc.setProcessChannelMode(QProcess::ProcessChannelMode::SeparateChannels);
    proc.start(cmd);
    proc.waitForFinished(-1);
    if (proc.exitStatus() == QProcess::NormalExit) {
        res = proc.exitCode();
    }
    if (res != 0) {
        emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                             QObject::tr("Command execution:\n%1\nreturn %2")
                                             .arg(cmd)
                                             .arg(res));
        return false;
    }

    stdOut = QString::fromStdString(proc.readAllStandardOutput().toStdString());
    return true;
}

bool execute(const QString& cmd, QStringList& list)
{
    QString tmp;
    if (execute(cmd, tmp)) {
        list = tmp.trimmed().split('\n');
        return true;
    }
    return false;
}

bool execute(const QString& cmd, int& result)
{
    QString stdOut;
    if (execute(cmd, stdOut)) {
        bool is_ok(true);
        result = stdOut.trimmed().toInt(&is_ok);
        if (!is_ok) {
            emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                                 QObject::tr("Incorrect value in stdout for command:\n%1\nValue: %2")
                                                 .arg(cmd, stdOut));
        }
        return is_ok;
    }
    return false;
}

bool execute(const QString& cmd, int& width, int& height)
{
    QString stdOut;
    bool ok = false;
    if (execute(cmd, stdOut)) {
        QStringList sl  = stdOut.trimmed().split('\n');
        if (!sl.isEmpty()) {
            QString val = sl[0];
            QRegularExpression rex("width=[ ]*([0-9]+)", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = rex.match(val);
            if (match.isValid() && !match.captured(1).isEmpty()) {
                width = match.captured(1).toInt(&ok);
            }
            if (ok) {
                rex.setPattern("height=[ ]*([0-9]+)");
                QRegularExpressionMatch match = rex.match(val);
                if (match.isValid() && !match.captured(1).isEmpty()) {
                    height = match.captured(1).toInt(&ok);
                }
            }
        }

        if (!ok) {
            emit MainWindow::_top_widget_->error(QObject::tr("Error"), QObject::tr("Incorrect value in stdout for command:\n%1\nValue: %2")
                                                 .arg(cmd, stdOut));
        }
        return ok;
    }
    return false;
}

}
