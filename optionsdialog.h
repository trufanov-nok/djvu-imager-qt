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
    bool customFilename;
    QString fnAddon;
    int addonState;
    bool recognizeST;
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
