#include "qmytablewidget.h"
#include <QLineEdit>
#include <QFileInfo>
#include <QMessageBox>

QMyTableWidget::QMyTableWidget(QWidget *parent): QTableWidget(parent)
{
    connect(this, &QMyTableWidget::currentCellChanged, [=](){
        m_oldPage = getCurrentPage();
    });
}

void QMyTableWidget::rowCountChanged(int oldCount, int newCount)
{
    QTableWidget::rowCountChanged(oldCount, newCount);
    emit signalRowCountChanged(newCount);
}

void QMyTableWidget::clearHighlight()
{
    for (int i = 0; i < rowCount(); i++) {
        QTableWidgetItem* it = item(i,0);
        QFont f = it->font();
        f.setBold(false);
        it->setFont(f);
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
        int page = getPage(r);
        res[page] = item(r,0)->data(Qt::UserRole).toString();
    }
    return res;
}

QMap<int, QString> QMyTableWidget::currentToFilePathsData() const
{
    QMap<int, QString> res;
    int r = currentRow();
    if (r >= 0) {
        int page = getPage(r);
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

void QMyTableWidget::displayTableItems(const QStringList& files, bool clear, const QString& regexp, int group)
{
    blockSignals(true);
    setSortingEnabled(false);
    if (clear) {
        setRowCount(0);
    }
    QFileInfo fi;
    QRegularExpression re(regexp, QRegularExpression::CaseInsensitiveOption);


    for (int i = 0; i < files.count(); i++) {
        fi.setFile(files[i]);
        const QString filename = fi.fileName();
        const QRegularExpressionMatch m = re.match(filename);
        if (m.hasMatch() && m.matchType() == QRegularExpression::NormalMatch) {
            bool isOk= false;
            QString col2 = QString::number(m.captured(group).toInt(&isOk));
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
            connect(tableline, &QLineEdit::editingFinished, this, &QMyTableWidget::verifyPageChange);

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

int QMyTableWidget::getPage(int row, int col) const
{
    return qobject_cast<QLineEdit*>(cellWidget(row, col))->text().toInt();
}

void QMyTableWidget::setPage(int val, int row, int col)
{
    qobject_cast<QLineEdit*>(cellWidget(row, col))->setText(QString::number(val));
}

int QMyTableWidget::getCurrentPage() const
{
    int row = currentRow();
    if (row > 0) {
        return getPage(row);
    }
    return -1;
}

void QMyTableWidget::verifyPageChange()
{
    QLineEdit* editor = qobject_cast<QLineEdit*>(sender());
    if (!editor || !editor->isModified()) { // in some cases editingFinished may be fired twice
        return;
    } else {
        editor->setModified(false);
    }

    int new_page = getCurrentPage();
    for (int i = 0; i < rowCount(); i++) {
        if (i != currentRow() && new_page == getPage(i)) {
            QMessageBox::critical(this, tr("Error"), tr("The page %1 already exists in the list").arg(new_page));
            setPage(m_oldPage, currentRow());
            return;
        }
    }
    emit pageWasChanged(m_oldPage, new_page);
    m_oldPage = new_page;
}
