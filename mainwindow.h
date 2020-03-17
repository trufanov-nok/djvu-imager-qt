#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QSettings>
#include <QTranslator>

#include "pageparam.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

    void displayTableItems(const QStringList&, bool clear = true);
    void addOrOpenFiles(bool isOpen = true);

    void loadDefaultPageSetting();
    void saveDefaultPageSetting();
    void displayPageSetting(PageSetting &val);
    void displayDefaultPageSetting();
    void propagetePageSettingsChanges();
    void updateTmpFolderSize();
private:
    bool loadLanguage(const QString& dir, const QString& lang);
    void changeLanguage(QString lang, bool dont_store = false);
    void updateWindowTitle();
    void changeEvent(QEvent* event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void error(const QString &title, const QString& text);
public slots:
    void displayError(const QString &title, const QString& text);

private slots:
    void on_btnHelp_clicked();

    void on_btnOpenFile_clicked();

    void on_btnOpenFolder_clicked();

    void on_btnAddFiles_clicked();

    void on_btnRemoveFiles_clicked();

    void on_btnSourceDjVu_clicked();

    void on_btnDestDjVu_clicked();

    void on_btnOptions_clicked();

    void on_rbAll_clicked();

    void on_rbCurrent_clicked();

    void on_btnClear_clicked();

    void on_tblFiles_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnConvert_clicked();

    void on_btnView_clicked();

    void on_btnCurrent_clicked();

    void on_btnInsert_clicked();
public:
    static MainWindow* _top_widget_;
private:
    Ui::MainWindow *ui;
    QSettings m_settings;
    PageSetting m_defaultPageSetting;
    PageSetting* m_currentPageSetting;
    QMap<int, PageSetting> m_customPageSettings;
    QString m_lastOpenPath;
    QString m_tmpImagesFolder;
    QString m_tmpFileFolder;

    //Language
    QString m_current_lang;
    QTranslator m_translator;
    QTranslator m_qt_translator;
};
#endif // MAINWINDOW_H
