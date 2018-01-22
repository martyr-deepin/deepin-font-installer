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

#include "listwidget.h"
#include <QListWidgetItem>

ListWidget::ListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setVerticalScrollMode(ScrollPerPixel);
    setAutoScroll(false);
    setSelectionMode(QAbstractItemView::NoSelection);
    setStyleSheet("QListView {"
                  "border: 1px solid rgba(151, 151, 151, 0.1);"
                  "border-radius: 4px;"
                  "}");
}

ListWidget::~ListWidget()
{
}

void ListWidget::addListItem(DFontInfo *info)
{
    ListItem *fileItem = new ListItem;

    addItem(fileItem->getItem());
    fileItem->setFontInfo(info);
    fileItem->getItem()->setSizeHint(QSize(100, 63));
    setItemWidget(fileItem->getItem(), fileItem);

    connect(fileItem, &ListItem::closeBtnClicked, this, &ListWidget::handleClose);
}

void ListWidget::handleClose(QListWidgetItem *item)
{
    ListItem *fileItem = static_cast<ListItem *>(itemWidget(item));
    emit deleteItem(fileItem->getFontInfo());
    delete takeItem(row(fileItem->getItem()));
}
