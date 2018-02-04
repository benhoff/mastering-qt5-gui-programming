#include "picturemodel.h"

PictureModel::PictureModel()
{
    setup_data();
}

void PictureModel::setup_data()
{
}


QModelIndex PictureModel::parent(const QModelIndex &child) const override
{
}

QVariant PictureModel::data(const QModelIndex &index, int role) const override
{
}

int PictureModel::rowCount(const QModelIndex &parent) const override
{
}

int PictureModel::columnCount(const QModelIndex &parent) const override
{
}

QModelIndex PictureModel::index(int row, int column, const QModelIndex &parent) const override
{
}
