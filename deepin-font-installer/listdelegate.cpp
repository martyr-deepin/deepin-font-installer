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
    const auto ratio = qApp->devicePixelRatio();
    m_fontIcon = DSvgRenderer::render(":/images/font-x-generic.svg", QSize(42, 42) * ratio);
    m_removeIcon = DSvgRenderer::render(":/images/close_normal.svg", QSize(16, 16) * ratio);
    m_fontIcon.setDevicePixelRatio(ratio);
    m_removeIcon.setDevicePixelRatio(ratio);
}

void ListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int contentX = 60;
    const QRect rect = option.rect;
    const QString familyName = index.data(ListModel::FontNameRole).toString();
    const QString styleName = index.data(ListModel::FontStyleRole).toString();
    const QString typeName = index.data(ListModel::FontTypeRole).toString();
    const QString version = index.data(ListModel::FontVersionRole).toString();
    const QString copyright = index.data(ListModel::FontCopyrightRole).toString();
    const QString description = index.data(ListModel::FontDescription).toString();

    painter->setRenderHint(QPainter::Antialiasing, true);

    // draw separator line.
    if (index.row()) {
        const QPoint startPos(contentX, rect.y());
        const QPoint endPos(rect.right() - 10, rect.y());
        painter->setPen(QColor(0, 0, 0, 255 * 0.1));
        painter->drawLine(startPos, endPos);
    }

    // draw icon.
    const int iconX = 5;
    const int iconY = rect.y() + (rect.height() - m_fontIcon.height() / m_fontIcon.devicePixelRatio()) / 2;
    painter->drawPixmap(iconX, iconY, m_fontIcon);

    // draw font family name.
    QRect nameRect = rect;
    nameRect.setLeft(contentX);
    nameRect.setHeight(nameRect.height() / 2);
    const QRectF nameBoundingRect = painter->boundingRect(nameRect, familyName,
                                                          Qt::AlignLeft | Qt::AlignBottom);
    painter->setPen(Qt::black);
    painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignBottom, familyName);

    // draw stlye name.
    const int styleX = nameBoundingRect.right() + 15;
    QRect styleRect = nameRect;
    styleRect.setLeft(styleX);
    styleRect.setRight(rect.right() - 85);
    painter->setPen(QColor(0, 0, 0, 255 * 0.5));
    painter->drawText(styleRect, Qt::AlignLeft | Qt::AlignBottom, styleName);

    // draw remove icon.
    const int x = rect.right() - m_removeIcon.width() / m_removeIcon.devicePixelRatio() - 10;
    const int y = rect.y() + (rect.height() - m_removeIcon.height() / m_removeIcon.devicePixelRatio()) / 2;
    painter->drawPixmap(x, y, m_removeIcon);
}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 60);
}
