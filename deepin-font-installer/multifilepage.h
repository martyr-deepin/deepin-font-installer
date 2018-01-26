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

#include "dfontinfomanager.h"
#include "dfontinstall.h"
#include "dspinner.h"
#include "listwidget.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>

DWIDGET_USE_NAMESPACE

class MultiFilePage : public QWidget
{
    Q_OBJECT

public:
    MultiFilePage(QWidget *parent = 0);
    ~MultiFilePage();

    void addItems(const QStringList &paths);
    QList<DFontInfo *> getInfoList() { return m_infoList; };

signals:
    void countChanged();

private slots:
    void handleDelete(DFontInfo *);
    void refreshList();
    void refreshPage();
    void batchInstallation();

private:
    DFontInfoManager *m_fontInfo;
    DFontInstall *m_fontInstall;
    ListWidget *m_listWidget;
    QPushButton *m_installBtn;
    QPushButton *m_closeBtn;
    DSpinner *m_spinner;
    QList<DFontInfo *> m_infoList;
};

#endif
