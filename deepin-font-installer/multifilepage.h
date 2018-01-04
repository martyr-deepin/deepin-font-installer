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

#ifndef MULTIFILEPAGE_H
#define MULTIFILEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "listview.h"
#include "dfontinfo.h"

class MultiFilePage : public QWidget
{
    Q_OBJECT

public:
    MultiFilePage(QWidget *parent = 0);
    ~MultiFilePage();

    void addItem(const QString &path);

signals:
    void countChanged();

private slots:
    void handleDelete(DFontData *);
    void refreshList();
    void refreshPage();
    void batchInstallation();

private:
    QVBoxLayout *m_layout;
    ListView *m_listView;
    QPushButton *m_installBtn;
    QPushButton *m_closeBtn;
    DFontInfo *m_fontInfo;

public:
    QList<DFontData *> dataList;
};

#endif
