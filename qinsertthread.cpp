#include "qinsertthread.h"
#include <QStandardPaths>
#include <QApplication>
#include <QMessageBox>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDesktopServices>
#include <QUrl>


#include "utils.h"
#include "mainwindow.h"

QInsertThread::QInsertThread(QObject *parent, const QMap<int, QString> &filesToPages, const QString& tmpImagesDir,
                             const QString& tmpFilesDir,
                             const QString& sourceDjVu,
                             const QString& destDjVu,
                             bool openAfterInsert)
    : QThread(parent),
      m_filesToPages(filesToPages),
      m_tmpImagesDir(tmpImagesDir),
      m_tmpFilesDir(tmpFilesDir),
      m_sourceDjVu(sourceDjVu),
      m_destDjVu(destDjVu),
      m_openAfterInsert(openAfterInsert)
{
}

bool CheckSubSizes(int w, int h, int sw, int sh)
{
    // GP<GPixmap> read_background(BufferByteStream &bs, int w, int h, int &bgred)
    // from csepdjvu.cpp
    int bgred;

    for (bgred=1; bgred<=12; bgred++)
    {
        int subw = (w + bgred - 1) / bgred;
        int subh = (h + bgred - 1) / bgred;
        if (subh == sh && subw == sw)
            // Found reduction factor
            //return pix;
            return true;
    }
    // Failure
    //G_THROW("Background pixmap size does not match foreground");
    return false;
}


void QInsertThread::run()
{

    ///////////////////////////////////////////////////////////
    //
    // dumping the multi-page DjVu document structure:

    QString djvused = Utils::findExecutable("djvused");
    if (djvused.isEmpty()) {
        return;
    }

    QString command_line = QString("\"%1\" -e \"dump\" \"%2\"").arg(djvused)
            .arg(m_sourceDjVu);

    QStringList dump_list;
    if (!Utils::execute(command_line, dump_list)) {
        return;
    }

    ///////////////////////////////////////////////////////////
    //
    //  converting multi-page DjVu document to the indirect multi-page format:

    QString djvmcvt = Utils::findExecutable("djvmcvt");
    if (djvmcvt.isEmpty()) {
        return;
    }

    command_line = QString("\"%1\" -i \"%2\" \"%3\" index.djvu").arg(djvmcvt)
            .arg(m_sourceDjVu)
            .arg(m_tmpFilesDir);


    if (!Utils::execute(command_line)) {
        return;
    }

    QString djvumake = Utils::findExecutable("djvumake");
    if (djvumake.isEmpty()) {
        return;
    }

    QString djvuextract = Utils::findExecutable("djvuextract");
    if (djvuextract.isEmpty()) {
        return;
    }

    int max_pages = 0;
    command_line = QString("\"%1\" -e \"n\" %2").arg(djvused).arg(m_sourceDjVu);
    if (!Utils::execute(command_line, max_pages)) {
        return;
    }

    if (max_pages <= 0) {
        emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                              QObject::tr("The file \"%1\" contains insorrect number of pages: %2.")
                              .arg(m_sourceDjVu).arg(max_pages));
        return;
    }

    QRegularExpression rex;
    QFileInfo fi;
    int pos = 1;
    for (QMap<int, QString>::const_iterator it = m_filesToPages.cbegin();
         it != m_filesToPages.cend(); ++it, ++pos) {

        int page_num = it.key();

        if (page_num > max_pages) {
            emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                  QObject::tr("The number of the illustration \"%1\" is \"%2\" which is not within the pages range (1-%3)\n of the processed pasted-on DjVu-file.\n\nProcessing aborted.")
                                  .arg(pos).arg(page_num).arg(max_pages));
            return;
        }

        fi.setFile(*it);
        QString tmpImage = m_tmpImagesDir + fi.completeBaseName() + ".djv";
        if (!QFile::exists(tmpImage)) {
            emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                  QObject::tr("The file \"%1\" is not found.")
                                  .arg(tmpImage));
            return;
        }

        bool is_fg44 = false, is_fgbz = false, is_sjbz = false;
        int djvu_image_width = 0, djvu_image_height = 0;

        command_line = QString("\"%1\" -e \"size\" \"%2\"").arg(djvused).arg(tmpImage);
        if (!Utils::execute(command_line, djvu_image_width, djvu_image_height)) {
            return;
        }

        ////////////////////////////
        //
        // Extract BG44 chunk from the current DjVu-pic:
        command_line = QString("\"%1\" \"%2\" BG44=\"%3\"").arg(djvuextract).arg(tmpImage).arg(m_tmpFilesDir + "BG44.cnk");
        if (!Utils::execute(command_line)) {
            return;
        }

        rex.setPattern(QString(".*\\[P%1\\]$").arg(page_num));
        int idx = dump_list.indexOf(rex);
        if (idx <= -1) {
            emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                  QObject::tr("The page \"%1\" is not found in the dumped DjVu structure.")
                                  .arg(page_num));
            return;
        }
        rex.setPattern(".*FORM:DJV[UI].*");
        int idx_next = dump_list.indexOf(rex, idx+1);
        if (idx_next == -1) {
            idx_next = dump_list.count();
        }

        QString src_width; QString src_height; QString src_dpi;
        QString incl_chunk; QString temp_djvu;
        QString command_line2;

        for (int i = idx; i < idx_next; i++) {

            if (dump_list[i].contains("FORM:DJVU")) {
                // FORM:DJVU [10653] {0165.djvu} [P165]
                if (max_pages == 1) {
                    fi.setFile(m_sourceDjVu);
                    temp_djvu += m_tmpFilesDir + fi.fileName();
                } else {
                    rex.setPattern(".*{(.*)}.*");
                    QRegularExpressionMatch m = rex.match(dump_list[i]);
                    temp_djvu = m_tmpFilesDir + m.captured(1);
                }
                continue;
            }

            if (dump_list[i].contains("INFO [")) {
                //INFO [10]         DjVu 2561x3927, v25, 600 dpi, gamma=2.2
                rex.setPattern(".*DjVu ([0-9]+)x([0-9]+).*?([0-9]+) dpi.*");
                QRegularExpressionMatch m = rex.match(dump_list[i]);
                src_width = m.captured(1);
                src_height = m.captured(2);
                src_dpi = m.captured(3);

                if (!CheckSubSizes(src_width.toInt(), src_height.toInt(),
                                   djvu_image_width, djvu_image_height)) {
                    emit MainWindow::_top_widget_->error(QObject::tr("Error"),
                                          QObject::tr("The size of the illustration #%1 (%2x%3) isn't proportional to the size "
                                                      "of the corresponding pasted-on DjVu-page (%4x%5).\n\nProcessing aborted.")
                                          .arg(pos).arg(src_width.toInt()).arg(src_height.toInt())
                                          .arg(djvu_image_width).arg(djvu_image_height));
                    return;
                }
                continue;
            }

            if (dump_list[i].contains("BG44 [")) {
                continue;
            }

            if (dump_list[i].contains("FG44 [")) {
                is_fg44 = true;
            }

            if (dump_list[i].contains("FGbz [")) {
                is_fgbz = true;
            }

            if (dump_list[i].contains("INCL [")) {
                //INCL [9]          Indirection chunk --> {0170.djbz}
                rex.setPattern(".*{(.*)}.*");
                QRegularExpressionMatch m = rex.match(dump_list[i]);
                incl_chunk = m.captured(1);
            } else {
                ///////////////////////////////////
                // djvuextract [-page=pagenum] djvufile [chkid=filename]...
                if (dump_list[i].contains("Sjbz [")) {
                    is_sjbz = true;
                }
                rex.setPattern("[ ]*([a-zA-Z]+) .*");
                QRegularExpressionMatch m = rex.match(dump_list[i]);
                const QString chunk = m.captured(1);
                command_line2 += QString("\"%1=%2%3.cnk\" ")
                        .arg(chunk).arg(m_tmpFilesDir).arg(chunk);
            }
        }

        ////////////////////////////
        //
        // djvuextact main file page:

        if (!command_line2.isEmpty()) {
            command_line = QString("\"%1\" \"%2\" %3")
                    .arg(djvuextract)
                    .arg(temp_djvu)
                    .arg(command_line2);

            if (!Utils::execute(command_line)) {
                return;
            }
        }

        ////////////////////////////
        //
        // djvumake it back

        command_line = QString("\"%1\" \"%2\" ")
                .arg(djvumake)
                .arg(temp_djvu);

        if (!is_sjbz) {// the illustration is pasted on an empty page
            // fixing the bug with different DPI's
            QString cmd = QString("\"%1\" -e \"set-dpi %2\" -s \"%3\" ").arg(djvused).arg(src_dpi).arg(tmpImage);
            if (!Utils::execute(cmd)) {
                return;
            }

            cmd = QString("\"%1\" \"%2\" BG44=\"%3\"").arg(djvuextract).arg(tmpImage).arg(m_tmpFilesDir + "BG44.cnk");
            if (!Utils::execute(cmd)) {
                return;
            }
            command_line += "\"INFO=" + src_width + "," + src_height + "," + src_dpi +"\" ";
        } else {
            command_line += "\"INFO=,," + src_dpi + "\" ";
        }

        if(!incl_chunk.isEmpty()) {
            command_line += "\"INCL=" /*+ m_tmpFilesDir*/ + incl_chunk + "\" ";
        }

        if(!is_fgbz && is_sjbz) {
            command_line += "\"FGbz=#black\" ";
        }

        command_line += command_line2;
        command_line += "\"BG44="+m_tmpFilesDir+"BG44.cnk\"";

        if (!Utils::execute_with_work_dir_hack(command_line, m_tmpFilesDir)) {
            return;
        }

        if (pos < max_pages) {
            emit progress(pos);
        }

    }

    ///////////////////////////////////////////////////////////
    //
    //  converting multi-page DjVu document to the bundled multi-page format:

    command_line = QString("\"%1\" -b \"%2\" \"%3\"")
            .arg(djvmcvt)
            .arg(m_tmpFilesDir+"index.djvu")
            .arg(m_destDjVu);
    if (!Utils::execute(command_line)) {
        return;
    }
    emit progress(max_pages);

    if (m_openAfterInsert) {
        if (!QDesktopServices::openUrl(QUrl(m_destDjVu))) {
            emit MainWindow::_top_widget_->error(tr("Error"), tr("Can't find an application to open %1").arg(m_destDjVu));
        }
    }

}
