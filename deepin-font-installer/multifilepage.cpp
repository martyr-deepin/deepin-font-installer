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
#include <QListWidgetItem>
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
      m_listWidget(new ListWidget),
      m_tipsLabel(new QLabel(tr("Installed successfully"))),
      m_installBtn(new DSuggestButton),
      m_viewFileBtn(new DSuggestButton),
      m_progress(new Progress),
      m_animation(new QPropertyAnimation(m_progress, "value", this))
{
    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->addSpacing(15);
    contentLayout->addWidget(m_listWidget);
    contentLayout->addSpacing(15);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(m_installBtn, 0, Qt::AlignHCenter);
    btnsLayout->addWidget(m_viewFileBtn, 0, Qt::AlignHCenter);

    m_tipsLabel->setStyleSheet("QLabel { color: #47790c; font-size: 14px; }");
    m_tipsLabel->setVisible(false);

    m_installBtn->setText(tr("Install"));
    m_installBtn->setFocusPolicy(Qt::NoFocus);
    m_installBtn->setObjectName("BlueButton");
    m_installBtn->setFixedSize(160, 36);
    m_installBtn->setVisible(false);

    m_viewFileBtn->setStyleSheet("QPushButton { font-size: 14px; }");
    m_viewFileBtn->setText(tr("View font directory"));
    m_viewFileBtn->setFocusPolicy(Qt::NoFocus);
    m_viewFileBtn->setObjectName("BlueButton");
    m_viewFileBtn->setFixedSize(160, 36);
    m_viewFileBtn->setVisible(false);
    m_viewFileBtn->setVisible(false);
    m_progress->setVisible(false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(m_tipsLabel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(5);
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
    for (const QString &path : paths) {
        if (!m_listItems.contains(path)) {
            DFontInfo *fontInfo = m_fontInfoManager->getFontInfo(path);
            m_infoList.append(fontInfo);

            ListItem *fileItem = new ListItem;
            m_listWidget->addItem(fileItem->getItem());
            fileItem->updateInfo(fontInfo);
            fileItem->getItem()->setSizeHint(QSize(100, 48));
            m_listWidget->setItemWidget(fileItem->getItem(), fileItem);
            connect(fileItem, &ListItem::closeBtnClicked, this, &MultiFilePage::handleClose);

            m_listItems.insert(path, fileItem);
        }
    }

    m_tipsLabel->setVisible(false);
    m_installBtn->setVisible(true);
    m_viewFileBtn->setVisible(false);
    m_progress->setValue(0);

    refreshList();
}

void MultiFilePage::handleClose(QListWidgetItem *item)
{
    ListItem *fileItem = static_cast<ListItem *>(m_listWidget->itemWidget(item));
    delete m_listWidget->takeItem(m_listWidget->row(fileItem->getItem()));
    fileItem->deleteLater();

    m_infoList.removeAt(m_infoList.indexOf(fileItem->getFontInfo()));
    m_listItems.remove(fileItem->getFontInfo()->filePath);

    emit countChanged();
}

void MultiFilePage::refreshList()
{
    m_fontInfoManager->refreshList();

    for (auto *item : m_infoList) {
        item->isInstalled = m_fontInfoManager->isFontInstalled(item);
    }

    // for (int i = 0; i < m_listWidget->count(); ++i) {
    //     QListWidgetItem *item = m_listWidget->item(i);
    //     ListItem *itemWidget = qobject_cast<ListItem *>(m_listWidget->itemWidget(item));
    //     itemWidget->updateStatus();
    // }
}

void MultiFilePage::batchInstallation()
{
    QStringList filePaths;

    for (auto const &item : m_infoList) {
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

    m_animation->setStartValue(m_progress->value());
    m_animation->setEndValue((int) percent);
    m_animation->start();

    ListItem *item = m_listItems.find(filePath).value();
    item->setStatus(ListItem::Installed);

    int nextIndex = m_listWidget->row(item->getItem()) + 1;
    if (nextIndex < m_listWidget->count()) {
        QListWidgetItem *item = m_listWidget->item(nextIndex);
        ListItem *nextItem = qobject_cast<ListItem *>(m_listWidget->itemWidget(item));

        nextItem->setStatus(ListItem::Installing);
        m_listWidget->scrollToItem(nextItem->getItem());
    }

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
