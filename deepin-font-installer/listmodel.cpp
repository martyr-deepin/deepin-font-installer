/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "listmodel.h"

ListModel::ListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_fontInfo(new DFontInfo)
{
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_fontList.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    const int i = index.row();
    const DFontData *data = m_fontList.at(i);

    switch (role) {
    case FontNameRole:
        return data->familyName;
    case FontStyleRole:
        return data->styleName;
    case FontTypeRole:
        return data->type;
    case FontVersionRole:
        return data->version;
    case FontCopyrightRole:
        return data->copyright;
    case FontDescription:
        return data->description;
    }

    return QVariant();
}

void ListModel::appendPath(const QString &filePath)
{
    DFontData *newData = new DFontData;
    newData->filePath = filePath;
    m_fontInfo->getFontInfo(newData);
    m_fontList << newData;
}
