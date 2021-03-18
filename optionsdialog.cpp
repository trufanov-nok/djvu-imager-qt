#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include "config.h"
#include "utils.h"
#include <QDir>
#include <QScrollBar>

AppSettings::AppSettings():
    changeDPI(false), DPI(100),
    openAfterConv(false), openAfterIns(false),
    outputToDesktop(false), outputFileSuffix("out"),
    language("en"),
    customRegexp(false),
    regexp(".*?([0-9]+).*?"),
    regexpGroup(1)
{
}

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
    #if defined(Q_OS_WIN)
    , m_settings(QApplication::applicationDirPath() + "/settings.ini")
    #else
    , m_settings("DjVu Imager") // use default folder to store ini file
    #endif
{
    ui->setupUi(this);

    connect(ui->cbDPI, &QCheckBox::stateChanged, ui->comboDPI, &QComboBox::setEnabled);

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
    emit ui->cbDPI->stateChanged(m_appSettings.changeDPI);
    ui->comboDPI->setCurrentIndex(
                ui->comboDPI->findText( QString::number(m_appSettings.DPI) )
                );
    ui->cbOpenAfterConv->setChecked(m_appSettings.openAfterConv);
    ui->cbOpenAfterIns->setChecked(m_appSettings.openAfterIns);
    ui->cbOutpDesktop->setChecked(m_appSettings.outputToDesktop);
    ui->edOutputFileSuffix->setText(m_appSettings.outputFileSuffix);
    initLanguageList(m_appSettings.language);

    ui->gbRegExp->setChecked(m_appSettings.customRegexp);
    ui->edRegExp->setText(m_appSettings.regexp);
    ui->sbGroup->setValue(m_appSettings.regexpGroup);
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
    m_appSettings.customRegexp = ui->gbRegExp->isChecked();
    m_appSettings.regexp = ui->edRegExp->text();
    m_appSettings.regexpGroup = ui->sbGroup->value();
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
    m_settings.setValue("customRegexp", m_appSettings.customRegexp);
    m_settings.setValue("regexp", m_appSettings.regexp);
    m_settings.setValue("regexpGroup", m_appSettings.regexpGroup);
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
    m_appSettings.customRegexp = m_settings.value("customRegexp", false).toBool();
    m_appSettings.regexp = m_settings.value("regexp", ".*?([0-9]+).*?").toString();
    m_appSettings.regexpGroup = m_settings.value("regexpGroup", 1).toInt();
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

void OptionsDialog::on_btnTest_clicked()
{
    int grp = ui->sbGroup->value();
    ui->teDemo->clear();
    QRegularExpression rex(ui->edRegExp->text());
    const QStringList flist = ui->teDemoSrc->toPlainText().split("\n");
    for(const QString& fname: flist) {
        const QRegularExpressionMatch match = rex.match(fname);
        bool ok = false; int res = 0;
        if (match.isValid() && match.lastCapturedIndex() > 0) {
            res = match.captured(grp).toInt(&ok);
        }

        if (ok) {
            ui->teDemo->appendPlainText(tr("page #: %1").arg(res));
        } else {
            ui->teDemo->appendPlainText(tr("<error>"));
        }
    }
    ui->teDemo->verticalScrollBar()->setValue(0);
}

void OptionsDialog::on_btnResetRegExp_clicked()
{
    ui->edRegExp->setText(".*?([0-9]+).*?");
    ui->sbGroup->setValue(1);
}
