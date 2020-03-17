#include "qmytablewidget.h"

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
        int page = item(r,1)->text().toInt();
        res[page] = item(r,0)->data(0).toString();
    }
    return res;
}

QMap<int, QString> QMyTableWidget::currentToFilePathsData() const
{
    QMap<int, QString> res;
    int r = currentRow();
    if (r >= 0) {
        int page = item(r,1)->text().toInt();
        res[page] = item(r,0)->data(0).toString();
    }

    return res;
}

QStringList QMyTableWidget::filePathsData() const
{
    QStringList res;
    for (int r = 0; r < rowCount(); r++) {
        res.append(item(r,0)->data(0).toString());
    }
    return res;
}
