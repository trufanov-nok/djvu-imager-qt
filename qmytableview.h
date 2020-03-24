#ifndef QMYTABLEWIDGET_H
#define QMYTABLEWIDGET_H

#include <QTableView>
#include <QIntValidator>
#include <QItemDelegate>


struct ItemData
{
    QString filename;
    QString display_name;
    int page;
    bool marked;
};

class QMyTableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    QMyTableModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QList<ItemData>& getData() { return m_data; }
    void beginResetModel() { QAbstractTableModel::beginResetModel(); }
    void endResetModel() { QAbstractTableModel::endResetModel(); }
private:
    QList<ItemData> m_data;  //holds text entered into QTableView
};

class QPageNoDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QPageNoDelegate (QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
signals:
     void pageWasChanged(int old_page, int new_page) const;
};

class QMyTableView : public QTableView
{
    Q_OBJECT
protected:
    void rowCountChanged(int,int);
public:
    QMyTableView(QWidget *parent = nullptr);
    void displayTableItems(const QStringList& files, bool clear = true, const QString &regexp = QString(".*?([0-9]+).*?"), int group = 1);
    void clearHighlight();
    void highlightSelectedItems(bool set = true);
    void removeRows(QList<int> &rows);
    void setPage( int val, int row, int col = 1);
    int getPage(int row, int col = 1) const;
    int getCurrentPage() const;
    QMap<int, QString> pagesToFilePathsData() const;
    QMap<int, QString> currentToFilePathsData() const;
    QStringList filePathsData() const;
    QModelIndexList selectedIndexes() const override { return QTableView::selectedIndexes(); }
public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;
signals:
    void currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void signalRowCountChanged(int newCount);
    void pageWasChanged(int old_page, int new_page);
};

class MyIntRangeValidator : public QIntValidator
{
public:
    MyIntRangeValidator(int bottom, int top, QObject *parent);
    QValidator::State validate(QString &s, int &) const override;
};


#endif // QMYTABLEWIDGET_H
