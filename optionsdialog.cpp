#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include "utils.h"
#include <QDir>

AppSettings::AppSettings():
    changeDPI(false), DPI(100),
    openAfterConv(false), openAfterIns(false),
    outputToDesktop(false), outputFileSuffix("out"),
    language("en"),
    customFilename(false),
    fnAddon("b"), addonState(0), recognizeST(true)
{
}

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog),
    m_settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)
{
    ui->setupUi(this);

    connect(ui->cbDPI, &QCheckBox::stateChanged, ui->comboDPI, &QComboBox::setEnabled);
    connect(ui->cbCustomFilename, &QCheckBox::stateChanged, ui->gbBackground, &QGroupBox::setEnabled);

    loadAppSettings();
    displayAppSettings();
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::displayAppSettings()
{
    ui->cbDPI->setChecked(m_appSettings.changeDPI);
    ui->cbDPI->stateChanged(m_appSettings.changeDPI);
    ui->comboDPI->setCurrentIndex(
                ui->comboDPI->findText( QString::number(m_appSettings.DPI) )
                );
    ui->cbOpenAfterConv->setChecked(m_appSettings.openAfterConv);
    ui->cbOpenAfterIns->setChecked(m_appSettings.openAfterIns);
    ui->cbOutpDesktop->setChecked(m_appSettings.outputToDesktop);
    ui->edOutputFileSuffix->setText(m_appSettings.outputFileSuffix);
    initLanguageList(m_appSettings.language);

    ui->cbCustomFilename->setChecked(m_appSettings.customFilename);
    ui->cbCustomFilename->stateChanged(m_appSettings.customFilename);

    ui->edAddon->setText(m_appSettings.fnAddon);
    switch (m_appSettings.addonState) {
    case 0: ui->rbNone->setChecked(true);
        break;
    case 1: ui->rbSuffix->setChecked(true);
        break;
    default:
        ui->rbPrefix->setChecked(true);
    }

    ui->cbRecognizeSTFilename->setChecked(m_appSettings.recognizeST);

}

void OptionsDialog::storeAppSettings()
{
    m_appSettings.changeDPI = ui->cbDPI->isChecked();
    m_appSettings.DPI = ui->comboDPI->currentText().toInt();
    m_appSettings.openAfterConv = ui->cbOpenAfterConv->isChecked();
    m_appSettings.openAfterIns = ui->cbOpenAfterIns->isChecked();
    m_appSettings.outputToDesktop = ui->cbOutpDesktop->isChecked();
    m_appSettings.outputFileSuffix = ui->edOutputFileSuffix->text();
    m_appSettings.language =  ui->comboLang->itemData(ui->comboLang->currentIndex()).toString();
    m_appSettings.customFilename = ui->cbCustomFilename->isChecked();
    m_appSettings.fnAddon = ui->edAddon->text();
    m_appSettings.addonState = 0;
    if (ui->rbSuffix->isChecked()) {
        m_appSettings.addonState = 2;
    } else if (ui->rbPrefix->isChecked()) {
        m_appSettings.addonState = 3;
    }
    m_appSettings.recognizeST = ui->cbRecognizeSTFilename->isChecked();
}


void OptionsDialog::saveAppSettings()
{
    m_settings.setValue("changeDPI", m_appSettings.changeDPI);
    m_settings.setValue("DPI", m_appSettings.DPI);
    m_settings.setValue("openAfterConv", m_appSettings.openAfterConv);
    m_settings.setValue("openAfterIns", m_appSettings.openAfterIns);
    m_settings.setValue("outputToDesktop", m_appSettings.outputToDesktop);
    m_settings.setValue("outputFileSuffix", m_appSettings.outputFileSuffix);
    m_settings.setValue("language", m_appSettings.language);
    m_settings.setValue("customFilename", m_appSettings.customFilename);
    m_settings.setValue("fnAddon", m_appSettings.fnAddon);
    m_settings.setValue("addonState", m_appSettings.addonState);
    m_settings.setValue("recognizeST", m_appSettings.recognizeST);
}

void OptionsDialog::loadAppSettings()
{
    m_appSettings.changeDPI = m_settings.value("changeDPI", false).toBool();
    m_appSettings.DPI = m_settings.value("DPI", 600).toInt();
    m_appSettings.openAfterConv = m_settings.value("openAfterConv", false).toBool();
    m_appSettings.openAfterIns = m_settings.value("openAfterIns", false).toBool();
    m_appSettings.outputToDesktop = m_settings.value("outputToDesktop", false).toBool();
    m_appSettings.outputFileSuffix = m_settings.value("outputFileSuffix", "out").toString();
    m_appSettings.language = m_settings.value("language", "en").toString();
    m_appSettings.customFilename = m_settings.value("customFilename", false).toBool();
    m_appSettings.fnAddon = m_settings.value("fnAddon", "b").toString();
    m_appSettings.addonState = m_settings.value("addonState", 0).toInt();
    m_appSettings.recognizeST = m_settings.value("recognizeST", true).toBool();
}

void OptionsDialog::on_OptionsDialog_accepted()
{
    storeAppSettings();
    saveAppSettings();
}


void OptionsDialog::initLanguageList(QString cur_lang)
{
    ui->comboLang->blockSignals(true);
    ui->comboLang->clear();
    ui->comboLang->addItem("English", "en");

    const QStringList language_file_filter("DjVu_Imager-Qt_*.qm");
    QStringList fileNames = QDir().entryList(language_file_filter);

    if (fileNames.isEmpty()) {
        fileNames = QDir(QApplication::applicationDirPath() + "/languages/").entryList(language_file_filter);
        if (fileNames.isEmpty()) {
            fileNames = QDir(QString::fromUtf8(TRANSLATIONS_DIR_ABS)).entryList(language_file_filter);
            if (fileNames.isEmpty()) {
                fileNames = QDir(QString::fromUtf8(TRANSLATIONS_DIR_REL)).entryList(language_file_filter);
            }
        }
    }

    fileNames.sort();

    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.lastIndexOf('_') + 1);

        QString lang = QLocale::languageToString(QLocale(locale).language());
        ui->comboLang->addItem(lang, locale);
        if (locale == cur_lang) {
            ui->comboLang->setCurrentIndex(i+1);
        }
    }

    ui->comboLang->blockSignals(false);
}
