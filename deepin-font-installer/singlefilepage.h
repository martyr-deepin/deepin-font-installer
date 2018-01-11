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

#ifndef SINGLEFILEPAGE_H
#define SINGLEFILEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "dfontinfo.h"

class SingleFilePage : public QWidget
{
    Q_OBJECT

public:
    SingleFilePage(QWidget *parent = nullptr);
    ~SingleFilePage();

    void updateInfo(DFontData *data);
    void showInstalled();

private slots:
    void handleInstall();
    void handleRemove();
    void viewFilePath();

private:
    DFontData *m_data;
    DFontInfo *m_info;
    QVBoxLayout *m_layout;
    QLabel *m_nameLabel;
    QLabel *m_styleLabel;
    QLabel *m_typeLabel;
    QLabel *m_versionLabel;
    QLabel *m_copyrightLabel;
    QLabel *m_descriptionLabel;
    QLabel *m_statusLabel;
    QPushButton *m_installBtn;
    QPushButton *m_removeBtn;
    QPushButton *m_reinstallBtn;
    QPushButton *m_viewFileBtn;
    QPushButton *m_closeBtn;
};

#endif
