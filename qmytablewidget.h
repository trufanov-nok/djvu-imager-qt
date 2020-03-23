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
    void displayTableItems(const QStringList& files, bool clear = true, const QString &regexp = QString(".*?([0-9]+).*?"), int group = 1);
    void clearHighlight();
    void highlightSelectedItems(bool set = true);
    void setPage( int val, int row, int col = 1);
    int getPage(int row, int col = 1) const;
    int getCurrentPage() const;
    QMap<int, QString> pagesToFilePathsData() const;
    QMap<int, QString> currentToFilePathsData() const;
    QStringList filePathsData() const;
public slots:
    void verifyPageChange();
signals:
    void signalRowCountChanged(int newCount);
    void pageWasChanged(int old_page, int new_page);
private:
    int m_oldPage;
};

class MyIntRangeValidator : public QIntValidator
{
public:
    MyIntRangeValidator(int bottom, int top, QObject *parent);
    QValidator::State validate(QString &s, int &) const override;
};


#endif // QMYTABLEWIDGET_H
