/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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
#include <QLabel>
#include <QPushButton>
#include "dfontinfomanager.h"
#include "dfontmanager.h"
#include "listview.h"
#include "listitem.h"
#include "progress.h"

class MultiFilePage : public QWidget
{
    Q_OBJECT

public:
    MultiFilePage(QWidget *parent = 0);
    ~MultiFilePage();

    void addItems(const QStringList &paths);
    QList<DFontInfo *> getInfoList() { return m_fontInfoList; };

signals:
    void countChanged();

private slots:
    void refreshList();
    void batchInstallation();
    void onProgressChanged(const QString &filePath, const float &percent);
    void onWorkerFinished();
    void onViewFileBtnClicked();

private:
    DFontInfoManager *m_fontInfoManager;
    DFontManager *m_fontManager;
    ListView *m_listView;
    QLabel *m_tipsLabel;
    QPushButton *m_installBtn;
    QPushButton *m_viewFileBtn;
    Progress *m_progress;

    QList<DFontInfo *> m_fontInfoList;
    QMap<QString, ListItem *> m_listItems;
};

#endif
