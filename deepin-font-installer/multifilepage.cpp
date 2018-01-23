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
#include <QListWidgetItem>
#include "listitem.h"

MultiFilePage::MultiFilePage(QWidget *parent)
    : QWidget(parent),
      m_fontInfo(new DFontInfoManager),
      m_layout(new QVBoxLayout(this)),
      m_listWidget(new ListWidget),
      m_installBtn(new QPushButton(tr("Install"))),
      m_closeBtn(new QPushButton(tr("Done")))
{
    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->addSpacing(15);
    contentLayout->addWidget(m_listWidget);
    contentLayout->addSpacing(15);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_installBtn, 0, Qt::AlignHCenter);
    bottomLayout->addWidget(m_closeBtn, 0, Qt::AlignHCenter);

    m_installBtn->setObjectName("BlueButton");
    m_closeBtn->setObjectName("BlueButton");

    m_installBtn->hide();
    m_closeBtn->hide();

    m_layout->addLayout(contentLayout);
    m_layout->addSpacing(10);
    m_layout->addLayout(bottomLayout);
    m_layout->addSpacing(10);

    connect(m_listWidget, &ListWidget::deleteItem, this, &MultiFilePage::handleDelete);
    connect(m_installBtn, &QPushButton::clicked, this, &MultiFilePage::batchInstallation);
    connect(m_closeBtn, &QPushButton::clicked, this, &QApplication::quit);
}

MultiFilePage::~MultiFilePage()
{
}

void MultiFilePage::addItems(const QStringList &paths)
{
    for (const QString &path : paths) {
        bool isExist = false;
        // whether the same path;
        for (const auto *d : m_infoList) {
            if (d->filePath == path) {
                isExist = true;
                break;
            }
        }

        // add to listView if it does not exist.
        if (isExist) {
            continue;
        } else {
            DFontInfo *data = new DFontInfo;
            data->filePath = path;
            m_infoList.append(data);
            m_fontInfo->getFontInfo(data);
            m_listWidget->addListItem(data);
        }
    }

    refreshList();
    refreshPage();
}

void MultiFilePage::handleDelete(DFontInfo *p)
{
    m_infoList.removeAt(m_infoList.indexOf(p));

    if (p) {
        delete p;
    }

    refreshPage();

    emit countChanged();
}

void MultiFilePage::refreshList()
{
    m_fontInfo->refreshList();

    for (auto *item : m_infoList) {
        item->isInstalled = m_fontInfo->isFontInstalled(item);
    }
}

void MultiFilePage::refreshPage()
{
    bool isAllInstalled = true;

    for (const auto *item : m_infoList) {
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

    for (int i = 0; i < m_listWidget->count(); ++i) {
        QListWidgetItem *item = m_listWidget->item(i);
        ListItem *itemWidget = qobject_cast<ListItem *>(m_listWidget->itemWidget(item));
        itemWidget->updateStatus();
    }
}

void MultiFilePage::batchInstallation()
{
    QStringList filePaths;

    for (auto const &item : m_infoList) {
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
