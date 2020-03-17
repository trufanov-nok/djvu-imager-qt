#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

#include <QSettings>

namespace Ui {
class OptionsDialog;
}

struct AppSettings
{
    bool changeDPI;
    int DPI;
    bool openAfterConv;
    bool openAfterIns;
    bool outputToDesktop;
    QString outputFileSuffix;
    QString language;
    bool customRegexp;
    QString regexp;
    int regexpGroup;
    AppSettings();
};

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();
private slots:
    void on_OptionsDialog_accepted();
    void on_btnTest_clicked();
    void on_btnResetRegExp_clicked();
private:
    void displayAppSettings();
    void storeAppSettings();
    void saveAppSettings();
    void loadAppSettings();

    void initLanguageList(QString cur_lang);
private:
    Ui::OptionsDialog *ui;
    QSettings m_settings;
    AppSettings m_appSettings;
};

#endif // OPTIONSDIALOG_H
