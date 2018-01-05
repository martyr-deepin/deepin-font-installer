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

#include "multifilepage.h"
#include <QDebug>
#include <QApplication>

MultiFilePage::MultiFilePage(QWidget *parent)
    : QWidget(parent),
      m_fontInfo(new DFontInfo),
      m_layout(new QVBoxLayout(this)),
      m_listView(new ListView),
      m_installBtn(new QPushButton(tr("Install"))),
      m_closeBtn(new QPushButton(tr("OK")))
{
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_installBtn, 0, Qt::AlignHCenter);
    bottomLayout->addWidget(m_closeBtn, 0, Qt::AlignHCenter);

    m_installBtn->setObjectName("BlueButton");
    m_closeBtn->setObjectName("BlueButton");

    m_installBtn->hide();
    m_closeBtn->hide();

    m_layout->addWidget(m_listView);
    m_layout->addSpacing(15);
    m_layout->addLayout(bottomLayout);
    m_layout->addSpacing(10);

    connect(m_listView, &ListView::deleteItem, this, &MultiFilePage::handleDelete);
    connect(m_installBtn, &QPushButton::clicked, this, &MultiFilePage::batchInstallation);
    connect(m_closeBtn, &QPushButton::clicked, this, &QApplication::quit);
}

MultiFilePage::~MultiFilePage()
{
}

void MultiFilePage::addItem(const QString &path)
{
    // whether the same path.
    bool isExist = false;
    for (const auto *d : dataList) {
        if (d->filePath == path) {
            isExist = true;
            break;
        }
    }

    // add to dataList and listView if it does not exist.
    if (!isExist) {
        DFontData *data = new DFontData;

        data->filePath = path;
        dataList << data;
        m_fontInfo->getFontInfo(data);
        m_listView->addListItem(data);

        refreshPage();
    }
}

void MultiFilePage::handleDelete(DFontData *p)
{
    dataList.removeAt(dataList.indexOf(p));

    if (p) {
        delete p;
    }

    refreshPage();

    emit countChanged();
}

void MultiFilePage::refreshList()
{
    for (auto *item : dataList) {
        if (!item->isInstalled) {
            item->isInstalled = m_fontInfo->isFontInstalled(item);
        }
    }
}

void MultiFilePage::refreshPage()
{
    bool isAllInstalled = true;

    for (const auto *item : dataList) {
        if (!item->isInstalled) {
            isAllInstalled = false;
            break;
        }
    }

    if (isAllInstalled) {
        m_installBtn->hide();
        m_closeBtn->show();
    } else {
        m_installBtn->show();
        m_closeBtn->hide();
    }
}

void MultiFilePage::batchInstallation()
{
    QStringList filePaths;

    for (auto const &item : dataList) {
        if (!item->isInstalled) {
            filePaths << item->filePath;
        }
    }

    if (filePaths.count() != 0) {
        if (m_fontInfo->fontsInstall(filePaths)) {
            refreshList();
            refreshPage();
        }
    }
}
