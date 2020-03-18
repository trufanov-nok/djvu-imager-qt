#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

#include "optionsdialog.h"
#include "progressdialog.h"
#include "qconvertthread.h"
#include "qviewthread.h"
#include "qinsertthread.h"

#include "config.h"
#include "utils.h"

MainWindow* MainWindow::_top_widget_ = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    #if defined(Q_OS_WIN)
    , m_settings(QApplication::applicationDirPath() + "/settings.ini")
    #else
    , m_settings("DjVu Imager") // use default folder to store ini file
    #endif
    , m_currentPageSetting(&m_defaultPageSetting)
{
    MainWindow::_top_widget_ = this;
    connect(this, &MainWindow::error, this, &MainWindow::displayError);

    ui->setupUi(this);

    updateWindowTitle();

    connect(ui->cbBSF, &QCheckBox::stateChanged, [=](int val) {
        ui->comboBSF->setEnabled(val);
        m_currentPageSetting->reqBSF = val;
        propagetePageSettingsChanges();
    });

    connect(ui->cbBkgQuality, &QCheckBox::stateChanged, [=](int val) {
        ui->valBkgQuality->setEnabled(val);
        ui->lblBkgQuality->setEnabled(val);
        m_currentPageSetting->reqBackg = val;
        propagetePageSettingsChanges();
    });

    connect(ui->valBkgQuality, &QSlider::valueChanged, [=](int val) {
        ui->lblBkgQuality->setText(QString::number(val));
        m_currentPageSetting->valBackg = val;
        propagetePageSettingsChanges();
    });

    connect(ui->comboBSF, &QComboBox::currentTextChanged, [=](const QString &val) {
        m_currentPageSetting->valBSF = val.toInt();
        propagetePageSettingsChanges();
    });

    connect(ui->tblFiles, &QMyTableWidget::signalRowCountChanged, [=](int cnt) {
        ui->btnRemoveFiles->setEnabled(cnt);
        ui->gbPaths->setEnabled(cnt);
        ui->gbParameters->setEnabled(cnt);
        ui->gbApplyTo->setEnabled(cnt);
        ui->gbButtons->setEnabled(cnt);
        ui->btnInsert->setEnabled(cnt);
    });


    ui->tblFiles->signalRowCountChanged(0);

    ui->comboBSF->blockSignals(true);
    for (int i = 2; i <= 12; i++) {
        ui->comboBSF->addItem(QString::number(i), i);
    }
    ui->comboBSF->blockSignals(false);

    loadDefaultPageSetting();
    displayDefaultPageSetting();

    m_lastOpenPath = m_settings.value("lastPath", "").toString();
    ui->edSourceDjVu->setText(m_settings.value("lastSrcPath", "").toString());
    ui->edDestDjVu->setText(m_settings.value("lastDestPath", "").toString());
    ui->tblFiles->horizontalHeader()->resizeSection(1, 50);

    m_tmpImagesFolder = QApplication::applicationDirPath() + "/tmp/images/";
    m_tmpFileFolder = QApplication::applicationDirPath() + "/tmp/files/";

#ifdef __linux__
    QStringList tmp_paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
    if (!tmp_paths.isEmpty()) {
        m_tmpImagesFolder = tmp_paths[0] + "/djvu_imager_tmp/"; // overwrite with "/tmp"
        m_tmpFileFolder= tmp_paths[0] + "/djvu_imager_tmp_files/";
    }
#endif

    QString default_lang = QLocale::system().name().toLower();
    default_lang.truncate(default_lang.lastIndexOf('_'));
    QString lang = m_settings.value("language", default_lang).toString();
    changeLanguage(lang, true);
}

MainWindow::~MainWindow()
{
    saveDefaultPageSetting();
    m_settings.setValue("lastPath", m_lastOpenPath);
    m_settings.setValue("lastSrcPath", ui->edSourceDjVu->text());
    m_settings.setValue("lastDestPath", ui->edDestDjVu->text());

    delete ui;
}


void MainWindow::on_btnHelp_clicked()
{
    const QString fname = tr("eng.htm");
    QString main_help_file = qApp->applicationDirPath() + "/help/" + fname;

    bool found = QFile::exists(main_help_file);
    if (!found) {
        main_help_file = QString::fromUtf8(HELP_DIR_ABS) + "/" + fname;
        found = QFile::exists(main_help_file);
        if (!found) {
            main_help_file = QString::fromUtf8(HELP_DIR_REL) + "/" + fname;
            found = QFile::exists(main_help_file);
        }
    }

    if (!found) {
        QMessageBox::critical(this, tr("Error"), tr("Help file not found!"));
    } else if (!QDesktopServices::openUrl(QUrl(main_help_file))) {
        QMessageBox::critical(this, tr("Error"), tr("Can't find an application to open %1").arg(main_help_file));
    }
}

void MainWindow::addOrOpenFiles(bool isOpen)
{
    if (m_lastOpenPath.isEmpty()) {
        m_lastOpenPath = QApplication::applicationDirPath();
    }

    const QStringList files = QFileDialog::getOpenFileNames(
                this, QString(), m_lastOpenPath,
                tr("SK v5.91 background subscans") + " (*.sep.bmp *.sep.jpeg *.sep.jpg *.sep.gif *.sep.tiff *.sep.tif *.sep.pnm *.sep.ppm *.sep.pgm *.sep.pbm *.sep.png);;" +
                tr("All images") + " (*.bmp *.jpeg *.jpg *.gif *.tiff *.tif *.pnm *.ppm *.pgm *.pbm *.png);;" +
                tr("All files") + " (*.*)"
                );

    if (!files.isEmpty()) {
        if (m_settings.value("customRegexp", false).toBool()) {
            ui->tblFiles->displayTableItems(files, isOpen, m_settings.value("regexp", ".*?([0-9]+).*?").toString(),
                                            m_settings.value("regexpGroup", 1).toInt());
        } else {
            ui->tblFiles->displayTableItems(files, isOpen);
        }
        m_lastOpenPath = QDir(files[0]).path();
    }
}

void MainWindow::on_btnOpenFile_clicked()
{
    addOrOpenFiles();
}

void MainWindow::on_btnOpenFolder_clicked()
{
    if (m_lastOpenPath.isEmpty()) {
        m_lastOpenPath = QApplication::applicationDirPath();
    }

    const QString selected_dir = QFileDialog::getExistingDirectory(
                this, QString(), m_lastOpenPath);
    if (!selected_dir.isEmpty()) {
        QDir dir(selected_dir);

        QStringList files = dir.entryList(QDir::Files);
        if (!files.isEmpty()) {
            if (m_settings.value("customRegexp", false).toBool()) {
                ui->tblFiles->displayTableItems(files, true, m_settings.value("regexp", ".*?([0-9]+).*?").toString(),
                                                m_settings.value("regexpGroup", 1).toInt());
            } else {
                ui->tblFiles->displayTableItems(files);
            }
        }
        m_lastOpenPath = selected_dir;
    }
}

void MainWindow::on_btnAddFiles_clicked()
{
    addOrOpenFiles(false);
}

void MainWindow::on_btnRemoveFiles_clicked()
{
    // only 1st column items are reported as cellWidget is in 2nd one
    const QList<QTableWidgetItem*> list = ui->tblFiles->selectedItems();
    const int row_cnt = list.count();

    if (!row_cnt) {
        QMessageBox::information(this, tr("File removal", nullptr, 1), tr("Please select a file or files to remove from the list.\nThe files on drive will be kept."));
        return;
    } else if (row_cnt == 1){
        if (QMessageBox::question(this, tr("File removal", nullptr, 1),
                                  tr("Remove the file \"%1\" from the list?").arg(list[0]->text()),
                                  QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel) {
            return;
        }
    } else {

        if (QMessageBox::question(this, tr("File removal", nullptr, row_cnt),
                                  tr("Remove %1 file(s) from the list?", nullptr, row_cnt).arg(row_cnt),
                                  QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel) {
            return;
        }
    }

    QList<int> rows;
    for (const QTableWidgetItem* item: list) {
        if (item->column() == 0) { // just to make sure
            rows.append(item->row());
        }
    }

    qSort(rows.begin(), rows.end(),  qGreater<int>());
    for (int row: rows) {
        ui->tblFiles->removeRow(row);
    }

}

void MainWindow::on_btnSourceDjVu_clicked()
{
    QString dir = ui->edSourceDjVu->text();
    if (dir.isEmpty()) {
        dir = QApplication::applicationDirPath();
    }

    const QString file = QFileDialog::getOpenFileName(
                this, QString(), dir,
                tr("DjVu files") + " (*.djvu *.djv)"
                );

    if (!file.isEmpty()) {
        ui->edSourceDjVu->setText(file);
        if (ui->edDestDjVu->text().isEmpty()) {
            QString new_suffix = m_settings.value("outputFileSuffix", "out").toString();

            {
                QFileInfo fi(file);
                QString dest_path = fi.path(); //dest_path == source path
                if (m_settings.value("outputToDesktop", false).toBool()) {
                    QStringList desktop_paths = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
                    if (!desktop_paths.isEmpty()) {
                        dest_path = desktop_paths[0];
                    }
                }

                QString new_dest =
                        dest_path + QDir::separator() + fi.baseName()
                        + QString(".%1.").arg(new_suffix)
                        + fi.completeSuffix();
                if (new_dest != ui->edSourceDjVu->text()) { //might be the same if suffix is empty and folders match
                    ui->edDestDjVu->setText(new_dest);
                }
            }
        }
    }
}

void MainWindow::on_btnDestDjVu_clicked()
{
    QString dir = ui->edDestDjVu->text();
    if (dir.isEmpty()) {
        dir = QApplication::applicationDirPath();
    }

    const QString selected_dir = QFileDialog::getExistingDirectory(
                this, QString(), dir);
    if (!selected_dir.isEmpty()) {
        ui->edDestDjVu->setText(selected_dir);
    }
}

void MainWindow::on_btnOptions_clicked()
{
    OptionsDialog options;
    connect(&options, &OptionsDialog::accepted, [=]() {
        changeLanguage(m_settings.value("language", "en").toString(), true);
    });
    options.exec();
}

void MainWindow::loadDefaultPageSetting()
{
    m_defaultPageSetting.reqBSF = m_settings.value("all_reqBSF", true).toBool();
    m_defaultPageSetting.valBSF = m_settings.value("all_valBSF", 2).toInt();
    m_defaultPageSetting.reqBackg = m_settings.value("all_reqBackg", true).toBool();
    m_defaultPageSetting.valBackg = m_settings.value("all_valBackg", 8).toInt();
}

void MainWindow::saveDefaultPageSetting()
{
    m_settings.setValue("all_reqBSF", m_defaultPageSetting.reqBSF);
    m_settings.setValue("all_valBSF", m_defaultPageSetting.valBSF);
    m_settings.setValue("all_reqBackg", m_defaultPageSetting.reqBackg);
    m_settings.setValue("all_valBackg", m_defaultPageSetting.valBackg);
}

void MainWindow::displayPageSetting(PageSetting& val)
{
    m_currentPageSetting = &val;
    ui->cbBSF->setChecked(val.reqBSF);
    ui->comboBSF->setCurrentIndex(
                ui->comboBSF->findText( QString::number(val.valBSF) )
                );
    ui->cbBkgQuality->setChecked(val.reqBackg);
    ui->valBkgQuality->setValue(val.valBackg);
}

void MainWindow::displayDefaultPageSetting()
{
    displayPageSetting(m_defaultPageSetting);
}

void MainWindow::on_rbAll_clicked()
{
    displayDefaultPageSetting();
    for (QTableWidgetItem* it: ui->tblFiles->selectedItems()) {
        if (it->column() == 1) {
            int page = it->text().toInt();
            m_customPageSettings.remove(page);
        }
    }

    ui->tblFiles->highlightSelectedItems(false);
}

void MainWindow::on_rbCurrent_clicked()
{
    PageSetting* new_current = nullptr;
    int current_row = ui->tblFiles->currentRow();
    for (QTableWidgetItem* it: ui->tblFiles->selectedItems()) {
        if (it->column() == 1) {
            int page = it->text().toInt();
            if (!m_customPageSettings.contains(page)) {
                m_customPageSettings[page] = *m_currentPageSetting;
            }
            if (it->row() == current_row) {
                new_current = &m_customPageSettings[page];
            }
        }
    }

    if (new_current) {
        m_currentPageSetting = new_current;
    }

    ui->tblFiles->highlightSelectedItems();
}

void MainWindow::on_btnClear_clicked()
{
    m_customPageSettings.clear();
    ui->tblFiles->clearHighlight();
}

void MainWindow::propagetePageSettingsChanges()
{
    if (m_currentPageSetting != &m_defaultPageSetting) { //rbCustom is on
        for (QTableWidgetItem* it: ui->tblFiles->selectedItems()) {
            if (it->column() == 1) {
                int page = it->text().toInt();
                if (m_customPageSettings.contains(page)) {
                    m_customPageSettings[page] = *m_currentPageSetting;
                }
            }
        }
    }
}

void MainWindow::on_tblFiles_currentCellChanged(int currentRow, int /*currentColumn*/, int /*previousRow*/, int /*previousColumn*/)
{
    QTableWidgetItem* it = ui->tblFiles->item(currentRow, 1);
    if (it) {
        int page = it->text().toInt();
        if (m_customPageSettings.contains(page)) {
            displayPageSetting(m_customPageSettings[page]);
        } else {
            displayDefaultPageSetting();
        }
    }
}

void MainWindow::on_btnConvert_clicked()
{
    const QString title = tr("Conversion");
    QDir dir(m_tmpImagesFolder);

    if (!dir.removeRecursively()) {
        QMessageBox::critical(this, title, tr("Can't remove folder %1").arg(m_tmpImagesFolder));
        return;
    }

    if (!dir.mkpath(".")) {
        QMessageBox::critical(this, title, tr("Can't create folder %1").arg(m_tmpImagesFolder));
        return;
    }

    ProgressDialog dlg(this);
    dlg.setMaximum(ui->tblFiles->rowCount());

    const QMap<int, QString> filesToPages = ui->tblFiles->pagesToFilePathsData();
    QConvertThread thread(this, m_defaultPageSetting, m_customPageSettings,
                          filesToPages, &m_settings, m_tmpImagesFolder);
    connect(&thread, &QConvertThread::progress, &dlg, &ProgressDialog::setProgress);
    connect(&dlg, &ProgressDialog::rejected, &thread, &QConvertThread::terminate);
    thread.start();
    dlg.exec();

    thread.wait();

    updateTmpFolderSize();
}

void MainWindow::updateTmpFolderSize()
{
    QDir dir(m_tmpImagesFolder);
    QFileInfo fi;
    double fsize = 0;
    for(QString filePath : dir.entryList(QDir::Files)) {
        fi.setFile(dir, filePath);
        fsize += fi.size();
    }

    fsize /= 1024;
    if (fsize < 1024.) {
        ui->lblFilesize->setText(tr("%1 KiB").arg(QString::number(fsize, 'f', 2)));
    } else {
        ui->lblFilesize->setText(tr("%1 MiB").arg(QString::number(fsize/1024, 'f', 2)));
    }
}

void MainWindow::on_btnView_clicked()
{
    const QMap<int, QString> filesToPages = ui->tblFiles->pagesToFilePathsData();
    QViewThread* thread = new QViewThread(this, filesToPages, m_tmpImagesFolder);
    connect(thread, &QViewThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void MainWindow::on_btnCurrent_clicked()
{
    const QMap<int, QString> filesToPages = ui->tblFiles->currentToFilePathsData();
    QConvertThread* thread = new QConvertThread(this, m_defaultPageSetting, m_customPageSettings,
                          filesToPages, &m_settings, m_tmpImagesFolder);
    thread->setCurrentConversion(true);
    connect(thread, &QConvertThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void MainWindow::on_btnInsert_clicked()
{
    const QString title = tr("Insertion");
    QFileInfo fi(ui->edSourceDjVu->text());
    QString suffix = fi.suffix().toLower();
    if (!(suffix == "djv" || suffix == "djvu")) {
        QMessageBox::critical(this, title, tr("No DjVu file is chosen or wrong DjVu filename.").arg(m_tmpImagesFolder));
        return;
    }
    if (!fi.exists()) {
        QMessageBox::critical(this, title, tr("The file \"%1\" is not found.").arg(fi.absoluteFilePath()));
        return;
    }

    fi.setFile(ui->edDestDjVu->text());
    suffix = fi.suffix().toLower();
    if (!(suffix == "djv" || suffix == "djvu")) {
        QMessageBox::critical(this, title, tr("Output DjVu file has wrong filename."));
        return;
    }

    QDir dir(m_tmpFileFolder);

    if (!dir.removeRecursively()) {
        QMessageBox::critical(this, title, tr("Can't remove folder %1").arg(m_tmpImagesFolder));
        return;
    }

    if (!dir.mkpath(".")) {
        QMessageBox::critical(this, title, tr("Can't create folder %1").arg(m_tmpImagesFolder));
        return;
    }

    dir = fi.absoluteDir(); // output dir
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::critical(this, title, tr("Can't create folder %1").arg(m_tmpImagesFolder));
            return;
        }
    }


    ProgressDialog dlg(this);
    dlg.setMaximum(ui->tblFiles->rowCount());
    dlg.setInsertion(true);

    const QMap<int, QString> filesToPages = ui->tblFiles->pagesToFilePathsData();
    QInsertThread thread(this, filesToPages, m_tmpImagesFolder, m_tmpFileFolder,
                         ui->edSourceDjVu->text(), ui->edDestDjVu->text(),
                         m_settings.value("openAfterIns", false).toBool());
    connect(&thread, &QInsertThread::progress, &dlg, &ProgressDialog::setProgress);
    connect(&dlg, &ProgressDialog::rejected, &thread, &QInsertThread::terminate);
    thread.start();
    dlg.exec();

    thread.wait();
}

void MainWindow::displayError(const QString& title, const QString& text) {
    // required to display errors from non-GUI threads
    QMessageBox::critical(this, title, text);
}


/// l10n funcs adapted from ScanTailor Universal

bool
MainWindow::loadLanguage(const QString& dir, const QString& lang)
{
    const QString translation("DjVu_Imager-Qt_" + lang);
    bool loaded = m_translator.load(dir + translation);

#if defined(unix) || defined(__unix__) || defined(__unix)
    // will load it from /usr/share later
#else
    // we distribute all Qt Frameworks files for Win
    const QString qt_translation("qtbase_" + lang);
    if (loaded) {
        m_qt_translator.load(dir + qt_translation);
    }
#endif
    return loaded;
}

void
MainWindow::changeLanguage(QString lang, bool dont_store)
{
    lang = lang.toLower();
    if (lang == m_current_lang)
        return;



    bool loaded = loadLanguage("", lang);
    if (!loaded) {
        loaded = loadLanguage(qApp->applicationDirPath() + "/languages/", lang);
        if (!loaded) {
            loaded = loadLanguage(QString::fromUtf8(TRANSLATIONS_DIR_ABS) + "/", lang);
            if (!loaded) {
                loaded = loadLanguage(QString::fromUtf8(TRANSLATIONS_DIR_REL) + "/", lang);
            }
        }
    }

    if (loaded || lang == "en")
    {
        qApp->removeTranslator(&m_translator);
        qApp->installTranslator(&m_translator);
        if (!dont_store) {
            m_settings.setValue("language", lang);
        }
        m_current_lang = lang;

        // additionally load Qt's own localization to translate QDialogButtonBox buttons etc.
        if (!m_qt_translator.isEmpty()) {
            qApp->removeTranslator(&m_qt_translator);
        }

        if (lang != "en") { // Qt's "en" is built in

#if defined(unix) || defined(__unix__) || defined(__unix)
            if (m_qt_translator.isEmpty()) {
                m_qt_translator.load(QString("qt_%1").arg(lang), "/usr/share/qt5/translations/");
            }
#endif
            if (!m_qt_translator.isEmpty()) {
                qApp->installTranslator(&m_qt_translator);
            }
        }

    } else {
        changeLanguage("en"); // fallback to EN
    }
}

void
MainWindow::updateWindowTitle()
{
    setWindowTitle(tr("DjVu Imager"));
}

void
MainWindow::changeEvent(QEvent* event)
{
    if (nullptr != event) {
        switch(event->type()) {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            updateWindowTitle();
            break;

            // this event is send, if the system, language changes
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            changeLanguage(locale);
        }
            break;
        default:
            break;
        }
    }
    QMainWindow::changeEvent(event);
}
