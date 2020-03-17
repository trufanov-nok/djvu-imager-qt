#include "qmytablewidget.h"
#include <QLineEdit>
#include <QFileInfo>

QMyTableWidget::QMyTableWidget(QWidget *parent): QTableWidget(parent)
{
}

void QMyTableWidget::rowCountChanged(int oldCount, int newCount)
{
    QTableWidget::rowCountChanged(oldCount, newCount);
    emit signalRowCountChanged(newCount);
}

void QMyTableWidget::clearHighlight()
{
    for (int i = 0; i < rowCount(); i++) {
        for (int j = 0; j < columnCount(); j++) {
            QTableWidgetItem* it = item(i,j);
            QFont f = it->font();
            f.setBold(false);
            it->setFont(f);
        }
    }
}

void QMyTableWidget::highlightSelectedItems(bool set)
{
    for (QTableWidgetItem* it: selectedItems()) {
        QFont f = it->font();
        if (f.bold() != set) {
            f.setBold(set);
            it->setFont(f);
        }
    }
}

QMap<int, QString> QMyTableWidget::pagesToFilePathsData() const
{
    QMap<int, QString> res;
    for (int r = 0; r < rowCount(); r++) {
        int page = qobject_cast<QLineEdit*>(cellWidget(r,1))->text().toInt();
        res[page] = item(r,0)->data(Qt::UserRole).toString();
    }
    return res;
}

QMap<int, QString> QMyTableWidget::currentToFilePathsData() const
{
    QMap<int, QString> res;
    int r = currentRow();
    if (r >= 0) {
        int page = qobject_cast<QLineEdit*>(cellWidget(r,1))->text().toInt();
        res[page] = item(r,0)->data(Qt::UserRole).toString();
    }

    return res;
}

QStringList QMyTableWidget::filePathsData() const
{
    QStringList res;
    for (int r = 0; r < rowCount(); r++) {
        res.append(item(r,0)->data(Qt::UserRole).toString());
    }
    return res;
}

void QMyTableWidget::displayTableItems(const QStringList& files, bool clear)
{
    blockSignals(true);
    setSortingEnabled(false);
    if (clear) {
        setRowCount(0);
    }
    QFileInfo fi;
    QRegularExpression re(".*?([0-9]+).*?\\.sep\\..*", QRegularExpression::CaseInsensitiveOption);


    for (int i = 0; i < files.count(); i++) {
        fi.setFile(files[i]);
        const QString filename = fi.fileName();
        const QRegularExpressionMatch m = re.match(filename);
        if (m.hasMatch() && m.matchType() == QRegularExpression::NormalMatch) {
            bool isOk= false;
            QString col2 = QString::number(m.captured(1).toInt(&isOk));
            if (col2.isNull()) col2 = QString::number(i);
            const int new_row = rowCount();
            setRowCount(new_row+1);
            QTableWidgetItem* item = new QTableWidgetItem( filename );
            item->setData(Qt::UserRole, fi.absoluteFilePath());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            setItem(new_row, 0, item);

            QLineEdit * tableline = new QLineEdit;
            tableline->setFrame(false);
            tableline->setText(col2);
            tableline->setValidator( new MyIntRangeValidator(1, 999999, this) );
//            connect(tableline, &QLineEdit::editingFinished)
//            setItem(new_row, 1, new QTableWidgetItem( col2 ));
            setCellWidget(new_row, 1, tableline);
        }
    }

    setHorizontalHeaderLabels(QStringList() << tr("Filename") << tr("#"));
    setSortingEnabled(true);
    blockSignals(false);
    signalRowCountChanged(rowCount());
}

MyIntRangeValidator::MyIntRangeValidator(int bottom, int top, QObject *parent) :
    QIntValidator(bottom, top, parent)
{
}

QValidator::State MyIntRangeValidator::validate(QString &s, int &) const
{
    if (s.isEmpty()) {
        return QValidator::Intermediate;
    }

    bool ok;
    int d = s.toInt(&ok);
    if (ok && d >= bottom() && d <= top() ) {
        return QValidator::Acceptable;
    } else {
        return QValidator::Invalid;
    }
}


