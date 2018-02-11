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

#include "multifilepage.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QProcess>
#include <QTimer>
#include <QUrl>
#include "listitem.h"

MultiFilePage::MultiFilePage(QWidget *parent)
    : QWidget(parent),
      m_fontInfoManager(DFontInfoManager::instance()),
      m_fontManager(DFontManager::instance()),
      m_listView(new ListView),
      m_tipsLabel(new QLabel(tr("Installed successfully"))),
      m_installBtn(new QPushButton),
      m_viewFileBtn(new QPushButton),
      m_progress(new Progress)
{
    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->addSpacing(15);
    contentLayout->addWidget(m_listView);
    contentLayout->addSpacing(15);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(m_installBtn, 0, Qt::AlignHCenter);
    btnsLayout->addWidget(m_viewFileBtn, 0, Qt::AlignHCenter);

    m_tipsLabel->setStyleSheet("QLabel { color: #47790c; font-size: 14px; }");
    m_tipsLabel->setVisible(false);

    m_installBtn->setText(tr("Install"));
    m_installBtn->setFocusPolicy(Qt::NoFocus);
    m_installBtn->setFixedSize(160, 36);
    m_installBtn->setObjectName("BlueButton");
    m_installBtn->setVisible(false);

    m_viewFileBtn->setObjectName("BlueButton");
    m_viewFileBtn->setText(tr("View font directory"));
    m_viewFileBtn->setFocusPolicy(Qt::NoFocus);
    m_viewFileBtn->setFixedSize(180, 36);
    m_viewFileBtn->setVisible(false);
    m_viewFileBtn->setVisible(false);
    m_progress->setVisible(false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(m_tipsLabel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(m_progress, 0, Qt::AlignHCenter);
    mainLayout->addLayout(btnsLayout);
    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(10, 10, 10, 0);

    connect(m_installBtn, &QPushButton::clicked, this, &MultiFilePage::batchInstallation);
    connect(m_viewFileBtn, &QPushButton::clicked, this, &MultiFilePage::onViewFileBtnClicked);
    connect(m_fontManager, &DFontManager::installing, this, &MultiFilePage::onProgressChanged);
}

MultiFilePage::~MultiFilePage()
{
}

void MultiFilePage::addItems(const QStringList &paths)
{
    QList<DSimpleListItem *> listItems;

    for (const QString &path : paths) {
        if (!m_listItems.contains(path)) {
            DFontInfo *fontInfo = m_fontInfoManager->getFontInfo(path);

            // if can not open the font.
            if (fontInfo->familyName.isEmpty() &&
                fontInfo->styleName.isEmpty()) {
                delete fontInfo;
                continue;
            }

            ListItem *fileItem = new ListItem(fontInfo);
            listItems << fileItem;
            m_fontInfoList << fontInfo;
            m_listItems.insert(path, fileItem);
        }
    }

    // add items to listview.
    m_listView->addItems(listItems);

    m_tipsLabel->setVisible(false);
    m_installBtn->setVisible(true);
    m_viewFileBtn->setVisible(false);
    m_progress->setValue(0);
    refreshList();
}

void MultiFilePage::refreshList()
{
    m_fontInfoManager->refreshList();

    for (auto *item : m_fontInfoList) {
        item->isInstalled = m_fontInfoManager->isFontInstalled(item);
    }
}

void MultiFilePage::batchInstallation()
{
    QStringList filePaths;

    for (auto const &item : m_fontInfoList) {
        filePaths << item->filePath;
    }

    m_fontManager->setType(DFontManager::Install);
    m_fontManager->setInstallFileList(filePaths);
    m_fontManager->start();
}

void MultiFilePage::onProgressChanged(const QString &filePath, const float &percent)
{
    m_installBtn->setVisible(false);
    m_viewFileBtn->setVisible(false);
    m_progress->setVisible(true);
    m_progress->setValue(static_cast<int>(percent));

    ListItem *item = m_listItems.find(filePath).value();
    item->setStatus(ListItem::Installed);
    m_listView->update();

    // int nextIndex = m_listWidget->row(item->getItem()) + 1;
    // if (nextIndex < m_listWidget->count()) {
    //     QListWidgetItem *item = m_listWidget->item(nextIndex);
    //     ListItem *nextItem = qobject_cast<ListItem *>(m_listWidget->itemWidget(item));

    //     nextItem->setStatus(ListItem::Installing);
    //     m_listWidget->scrollToItem(nextItem->getItem());
    // }

    if (percent == 100) {
        onWorkerFinished();
    }
}

void MultiFilePage::onWorkerFinished()
{
    m_tipsLabel->setVisible(true);
    m_installBtn->setVisible(false);
    m_viewFileBtn->setVisible(true);
    m_progress->setVisible(false);
    m_progress->setValue(0);
    refreshList();
}

void MultiFilePage::onViewFileBtnClicked()
{
    QUrl url = QUrl::fromLocalFile("/usr/share/fonts/deepin-font-install");
    QProcess::startDetached("dde-file-manager", QStringList() << url.toString());
}
