#include "picturemodel.h"

typedef QList<QVariant> VariantList;

PictureModel::PictureModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    // root is empty
    root = new TreeItem(VariantList());
}

PictureModel::~PictureModel()
{
    delete root;
}

QModelIndex PictureModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem* parent_item = get_item(parent);
    TreeItem *child = parent_item->child(row);
    if (child)
        return createIndex(row, column, child);
    else
        return QModelIndex();
}

QModelIndex PictureModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *child = get_item(index);
    TreeItem *parent = child->parentItem();

    if (parent == root)
        return QModelIndex();

    // By common convention, only items in the first column have children.
    // Thus the column of the returned QModelIndex is 0.
    return createIndex(parent->childCount(), 0, parent);
}

int PictureModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    TreeItem* parent_item = get_item(parent);
    return parent_item->childCount();
}

int PictureModel::columnCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    return 12;
}

QVariant PictureModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::SizeHintRole)
        return QSize(50, 50);

    TreeItem* item = get_item(index);

    if (role == Qt::DecorationRole)
    {
        return item->data(index.column());
    }
    else if (role == Qt::ToolTipRole)
    {
        QVariant data = item->data(index.column());
        QColor c = data.value<QColor>();
        return QString("Hue: " + QString::number(c.hue()) + " Sat: " + QString::number(c.saturation()) + " Val: " + QString::number(c.value()));
    }

    return QVariant();
}

TreeItem* PictureModel::get_item(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return root;
}

void PictureModel::add_colors(ColorVec colors)
{
    beginInsertRows(QModelIndex(), root->childCount(), root->childCount() + 1);
    QVariant parent_color = colors[0];
    TreeItem *parent = new TreeItem(VariantList() << parent_color, root);
    root->appendChild(parent);

    VariantList child_data;
    // add in a blank variant to move out from the parent column
    child_data << QVariant();
    for (int i=1; i < colors.size(); i++)
    {
        QVariant c = colors[i];
        child_data.append(c);
    }

    TreeItem *child = new TreeItem(child_data, parent);
    parent->appendChild(child);

    endInsertRows();
}

Qt::ItemFlags PictureModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled;
}
