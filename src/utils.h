#ifndef UTILS_H
#define UTILS_H
#include <QStringList>

namespace Utils {

QString findExecutable(const QString& name);
bool execute_with_work_dir_hack(const QString& cmd, const QString& work_dir);
bool execute(const QString& cmd);
bool execute(const QString& cmd, QString& stdOut);
bool execute(const QString& cmd, int& result);
bool execute(const QString& cmd, int& width, int& height);
bool execute(const QString& cmd, QStringList& list);
}
#endif // UTILS_H
