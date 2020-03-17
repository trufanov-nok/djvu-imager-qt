#ifndef QMYTABLEWIDGET_H
#define QMYTABLEWIDGET_H

#include <QTableWidget>

class QMyTableWidget : public QTableWidget
{
    Q_OBJECT
protected:
    void rowCountChanged(int,int);
public:
    QMyTableWidget(QWidget *parent = nullptr);
    void clearHighlight();
    void highlightSelectedItems(bool set = true);
    QMap<int, QString> pagesToFilePathsData() const;
    QMap<int, QString> currentToFilePathsData() const;
    QStringList filePathsData() const;
signals:
    void signalRowCountChanged(int newCount);
};

#endif // QMYTABLEWIDGET_H
