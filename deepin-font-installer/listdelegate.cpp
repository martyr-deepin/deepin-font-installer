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

#include "listdelegate.h"
#include "listmodel.h"
#include <QPainter>
#include <QApplication>
#include <DSvgRenderer>

DWIDGET_USE_NAMESPACE

ListDelegate::ListDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
    m_icon = DSvgRenderer::render(":/images/font-x-generic.svg", QSize(16, 16) * qApp->devicePixelRatio());
}

void ListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int contentX = 60;
    painter->setRenderHint(QPainter::Antialiasing, true);

    // draw separator line.
    if (index.row()) {
        const QPoint startPos(contentX, option.rect.top());
        const QPoint endPos(option.rect.right() - 10,
                            option.rect.top());
        painter->setPen(QColor(0, 0, 0, 255 * 0.1));
        painter->drawLine(startPos, endPos);
    }

    const QRect rect = option.rect;
    const QString familyName = index.data(ListModel::FontNameRole).toString();
    const QString styleName = index.data(ListModel::FontStyleRole).toString();
    const QString typeName = index.data(ListModel::FontTypeRole).toString();
    const QString version = index.data(ListModel::FontVersionRole).toString();
    const QString copyright = index.data(ListModel::FontCopyrightRole).toString();
    const QString description = index.data(ListModel::FontDescription).toString();

    painter->setPen(Qt::black);
    painter->drawText(rect, Qt::AlignCenter, familyName);
}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 60);
}
