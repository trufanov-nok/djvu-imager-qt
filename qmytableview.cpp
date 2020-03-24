#include "qmytableview.h"
#include <QHeaderView>
#include <QStringListModel>
#include <QLineEdit>
#include <QFileInfo>
#include <QMessageBox>




/// MODEL




QMyTableModel::QMyTableModel(QObject *parent):
    QAbstractTableModel(parent)
{
}

int QMyTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_data.size();
}

int QMyTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant QMyTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return m_data[index.row()].display_name;
        } else if (index.column() == 1) {
            return m_data[index.row()].page;
        }
    } else if (role == Qt::FontRole) {
        if (m_data[index.row()].marked) {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
    }
    return QVariant();
}

QVariant QMyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (Qt::Orientation::Horizontal == orientation) {
            return section == 0 ? tr("Filename") : tr("#");
        } else {
            return section;
        }
    }
    return QVariant();
}

Qt::ItemFlags QMyTableModel::flags(const QModelIndex & index) const
{
    if (index.column() == 1) {
        return Qt::ItemFlag::ItemIsSelectable |  Qt::ItemFlag::ItemIsEnabled |
                 Qt::ItemFlag::ItemIsEditable;
    }
    return Qt::ItemFlag::ItemIsSelectable |  Qt::ItemFlag::ItemIsEnabled;
}

bool QMyTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.column() == 1 && role == Qt::EditRole) {
        m_data[index.row()].page = value.toString().toInt();
        return true;
    }

    return false;
}




/// VIEW


QMyTableView::QMyTableView(QWidget *parent): QTableView(parent)
{
    QPageNoDelegate* dlg = new QPageNoDelegate(this);
    setItemDelegateForColumn(1, dlg);

    connect(dlg, &QPageNoDelegate::pageWasChanged, this, &QMyTableView::pageWasChanged);

    setModel(new QMyTableModel(this));
}

void QMyTableView::rowCountChanged(int oldCount, int newCount)
{
    QTableView::rowCountChanged(oldCount, newCount);
    emit signalRowCountChanged(newCount);
}

void QMyTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
    emit currentCellChanged(current.row(), current.column(), previous.row(), previous.column());
}

void QMyTableView::clearHighlight()
{
    QMyTableModel* m = qobject_cast<QMyTableModel*> (model());
    for (ItemData& d: m->getData()) {
        d.marked = false;
    }
    emit m->dataChanged(m->index(0,0), m->index(m->rowCount(), m->columnCount()), QVector<int>() << Qt::DisplayRole);
}

void QMyTableView::highlightSelectedItems(bool set)
{
    QMyTableModel* m = qobject_cast<QMyTableModel*> (model());

    QList<ItemData>& data = m->getData();
    for (const QModelIndex i: selectionModel()->selectedRows()) {
        data[i.row()].marked = set;
    }

    emit m->dataChanged(selectionModel()->selectedIndexes().first(),
                        selectionModel()->selectedIndexes().last(),
                        QVector<int>() << Qt::DisplayRole);
}

QMap<int, QString> QMyTableView::pagesToFilePathsData() const
{
    QMap<int, QString> res;

    for (const ItemData d: qobject_cast<QMyTableModel*> (model())->getData()) {
        res[d.page] = d.filename;
    }

    return res;
}

QMap<int, QString> QMyTableView::currentToFilePathsData() const
{
    QMap<int, QString> res;
    if (!currentIndex().isValid()) return res;
    const QList<ItemData>& data = qobject_cast<QMyTableModel*> (model())->getData();
    ItemData d = data[currentIndex().row()];
    res[d.page] = d.filename;
    return res;
}

QStringList QMyTableView::filePathsData() const
{
    QStringList res;
    for (const ItemData d: qobject_cast<QMyTableModel*> (model())->getData()) {
        res.append(d.filename);
    }
    return res;
}

void QMyTableView::displayTableItems(const QStringList& files, bool clear, const QString& regexp, int group)
{

    QMyTableModel* m = qobject_cast<QMyTableModel*> (model());
    QList<ItemData>& data = m->getData();
    m->beginResetModel();
    if (clear) {
        data.clear();
    }

    QFileInfo fi;
    QRegularExpression re(regexp, QRegularExpression::CaseInsensitiveOption);


    for (int i = 0; i < files.count(); i++) {
        fi.setFile(files[i]);
        const QString filename = fi.fileName();
        const QRegularExpressionMatch m = re.match(filename);
        if (m.hasMatch() && m.matchType() == QRegularExpression::NormalMatch) {
            bool isOk = false;
            int page = m.captured(group).toInt(&isOk);
            if (!isOk) page = i;

            ItemData d;
            d.display_name = filename;
            d.filename = fi.absoluteFilePath();
            d.page = page;
            d.marked = false;

            data.append(d);
        }
    }

    m->endResetModel();

    signalRowCountChanged(model()->rowCount());
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

void QMyTableView::removeRows(QList<int>& rows)
{
    qSort(rows.begin(), rows.end(),  qGreater<int>());
    QList<ItemData>& data = qobject_cast<QMyTableModel*> (model())->getData();
    for (int row: rows) {
        data.removeAt(row);
    }
}

int QMyTableView::getPage(int row, int col) const
{
    QModelIndex m = model()->index(row, col);
    if (!m.isValid()) return -1;
    return model()->data(m).toInt();
}

void QMyTableView::setPage(int val, int row, int col)
{
    QModelIndex m = model()->index(row, col);
    if (!m.isValid()) return;
    model()->setData(m, val);
}

int QMyTableView::getCurrentPage() const
{
    int row = currentIndex().row();
    if (row > 0) {
        return getPage(row);
    }
    return -1;
}


//// DELEGATE


QPageNoDelegate::QPageNoDelegate (QObject *parent): QItemDelegate(parent)
{
}

QWidget * QPageNoDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem &/*option*/,
                          const QModelIndex &/*index*/) const
{
    QLineEdit * tableline = new QLineEdit(parent);
    tableline->setFrame(false);
    tableline->setValidator( new MyIntRangeValidator(1, 999999, tableline) );
    return tableline;
}

void QPageNoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit * ed = qobject_cast<QLineEdit*>(editor);
    ed->setText(index.model()->data(index).toString());
}

void QPageNoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    QLineEdit * ed = qobject_cast<QLineEdit*>(editor);
    QMyTableModel* m = qobject_cast<QMyTableModel*> (model);
    const QList<ItemData>& data = m->getData();

    int current_row = index.row();
    int old_page = data[current_row].page;
    int new_page = ed->text().toInt();



    for (int i = 0; i < data.size(); i++) {
        if (i != current_row && new_page == data[i].page) {
            QMessageBox::critical(editor, tr("Error"), tr("The page %1 already exists in the list").arg(new_page));
            ed->setText(QString::number(old_page));
            return;
        }
    }

    emit pageWasChanged(old_page, new_page);

    model->setData(index, new_page);
}

void QPageNoDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
