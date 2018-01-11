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

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include "dfontinfo.h"

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ListModel(QObject *parent = nullptr);

    enum FontRole
    {
        FontNameRole = Qt::DisplayRole,
        UnusedRole = Qt::UserRole,
        FontStyleRole,
        FontTypeRole,
        FontVersionRole,
        FontCopyrightRole,
        FontDescription,
    };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void appendPath(const QString &filePath);

    const QList<DFontData *> fontList() const { return m_fontList; };

private:
    QList<DFontData *> m_fontList;
    DFontInfo *m_fontInfo;
};

#endif // LISTMODEL_H
