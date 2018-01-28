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

#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QListWidgetItem>
#include <QLabel>
#include "dimagebutton.h"
#include "dfontinfomanager.h"

DWIDGET_USE_NAMESPACE

class ListItem : public QWidget
{
    Q_OBJECT

public:

    enum Status {
        None,
        Installed,
        Installing,
        Waiting
    };

    ListItem(QWidget *parent = nullptr);

    QListWidgetItem *getItem() { return m_item; };
    DFontInfo *getFontInfo() { return m_fontInfo; };
    void updateInfo(DFontInfo *);
    void setStatus(Status status);

signals:
    void closeBtnClicked(QListWidgetItem *item);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QListWidgetItem *m_item;
    DFontInfo *m_fontInfo;
    QLabel *m_nameLabel;
    QLabel *m_styleLabel;
    QLabel *m_infoLabel;
    QLabel *m_statusLabel;
    DImageButton *m_closeBtn;
};

#endif // LISTITEM_H
