#ifndef QMYTABLEWIDGET_H
#define QMYTABLEWIDGET_H

#include <QTableWidget>
#include <QIntValidator>

class QMyTableWidget : public QTableWidget
{
    Q_OBJECT
protected:
    void rowCountChanged(int,int);
public:
    QMyTableWidget(QWidget *parent = nullptr);
    void displayTableItems(const QStringList& files, bool clear = true);
    void clearHighlight();
    void highlightSelectedItems(bool set = true);
    QMap<int, QString> pagesToFilePathsData() const;
    QMap<int, QString> currentToFilePathsData() const;
    QStringList filePathsData() const;

signals:
    void signalRowCountChanged(int newCount);
};

class MyIntRangeValidator : public QIntValidator
{
public:
    MyIntRangeValidator(int bottom, int top, QObject *parent);
    QValidator::State validate(QString &s, int &) const override;
};


#endif // QMYTABLEWIDGET_H
