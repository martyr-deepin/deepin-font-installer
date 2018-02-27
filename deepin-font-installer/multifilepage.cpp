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
#include "dsvgrenderer.h"
#include "listitem.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QProcess>
#include <QTimer>
#include <QDebug>
#include <QUrl>
#include "dsimplelistitem.h"

DWIDGET_USE_NAMESPACE

MultiFilePage::MultiFilePage(QWidget *parent)
    : QWidget(parent),
      m_fontInfoManager(DFontInfoManager::instance()),
      m_fontManager(DFontManager::instance()),
      m_listView(new ListView),
      m_tipsLabel(new QLabel(tr("Installed successfully"))),
      m_installBtn(new QPushButton),
      m_viewFileBtn(new QPushButton),
      m_progress(new Progress),
      m_iconPixmap(new QPixmap),
      m_closeNormalPixmap(new QPixmap),
      m_closePressPixmap(new QPixmap),
      m_closeHoverPixmap(new QPixmap),
      m_refreshThread(new RefreshThread),
      m_bottomLayout(new QStackedLayout),
      m_spinner(new DSpinner)
{
    const auto ratio = qApp->devicePixelRatio();
    *m_iconPixmap = DSvgRenderer::render(":/images/font-x-generic.svg", QSize(32, 32) * ratio);
    m_iconPixmap->setDevicePixelRatio(ratio);

    *m_closeNormalPixmap = DSvgRenderer::render(":/images/close_normal.svg", QSize(16, 16) * ratio);
    m_closeNormalPixmap->setDevicePixelRatio(ratio);

    *m_closeHoverPixmap = DSvgRenderer::render(":/images/close_hover.svg", QSize(16, 16) * ratio);
    m_closeHoverPixmap->setDevicePixelRatio(ratio);

    *m_closePressPixmap = DSvgRenderer::render(":/images/close_press.svg", QSize(16, 16) * ratio);
    m_closePressPixmap->setDevicePixelRatio(ratio);

    // list widget.
    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->addSpacing(15);
    contentLayout->addWidget(m_listView);
    contentLayout->addSpacing(15);

    // init buttons page.
    QWidget *btnsPage = new QWidget;
    QVBoxLayout *bmainLayout = new QVBoxLayout(btnsPage);
    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(m_installBtn, 0, Qt::AlignHCenter);
    btnsLayout->addWidget(m_viewFileBtn, 0, Qt::AlignHCenter);
    bmainLayout->addWidget(m_tipsLabel, 0, Qt::AlignHCenter);
    bmainLayout->addSpacing(8);
    bmainLayout->addLayout(btnsLayout);

    // init progress page.
    QWidget *progPage = new QWidget;
    QVBoxLayout *progLayout = new QVBoxLayout(progPage);
    progLayout->addWidget(m_progress, 0, Qt::AlignCenter);

    // init refresh page.
    QWidget *refPage = new QWidget;
    QVBoxLayout *refLayout = new QVBoxLayout(refPage);
    QLabel *tipsLabel = new QLabel(tr("Refreshing font cache, please wait..."));
    tipsLabel->setStyleSheet("QLabel { color: #606060; font-size: 14px; }");
    refLayout->addWidget(m_spinner, 0, Qt::AlignHCenter);
    refLayout->addSpacing(5);
    refLayout->addWidget(tipsLabel, 0, Qt::AlignHCenter);
    m_spinner->setFixedSize(30, 30);

    m_bottomLayout->addWidget(btnsPage);
    m_bottomLayout->addWidget(progPage);
    m_bottomLayout->addWidget(refPage);

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

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(m_bottomLayout);

    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(10, 10, 10, 0);

    connect(m_installBtn, &QPushButton::clicked, this, &MultiFilePage::batchInstallation);
    connect(m_viewFileBtn, &QPushButton::clicked, this, &MultiFilePage::onViewFileBtnClicked);
    connect(m_fontManager, &DFontManager::batchInstall, this, &MultiFilePage::onProgressChanged);
    connect(m_refreshThread, &RefreshThread::refreshFinished, this, &MultiFilePage::onWorkerFinished);
    connect(m_listView, &ListView::closeBtnClicked, this, &MultiFilePage::handleCloseBtnClicked);
}

MultiFilePage::~MultiFilePage()
{
    delete m_iconPixmap;
    delete m_closeNormalPixmap;
    delete m_closePressPixmap;
    delete m_closeHoverPixmap;
}

void MultiFilePage::addItems(const QStringList &paths)
{
    QList<DSimpleListItem *> listItems;

    for (const QString &path : paths) {
        if (!m_listItems.contains(path)) {
            DFontInfo *fontInfo = m_fontInfoManager->getFontInfo(path);

            // if can not open the font.
            if (fontInfo->isError) {
                delete fontInfo;
                continue;
            }

            ListItem *fileItem = new ListItem(fontInfo, m_iconPixmap, m_closeNormalPixmap, m_closeHoverPixmap, m_closePressPixmap);

            if (fontInfo->copyright.isEmpty()) {
                fontInfo->copyright = tr("Unknown");
            }
            if (fontInfo->description.isEmpty()) {
                fontInfo->description = tr("Unknown");
            }
            if (fontInfo->version.isEmpty()) {
                fontInfo->version = tr("Unknown");
            }
            if (fontInfo->sysVersion.isEmpty()) {
                fontInfo->sysVersion = tr("Unknown");
            }

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
        // the function called above modifies sysVersion.
        item->isInstalled = m_fontInfoManager->isFontInstalled(item);

        if (item->sysVersion.isEmpty())
            item->sysVersion = tr("Unknown");
    }
}

void MultiFilePage::batchInstallation()
{
    QStringList filePaths;

    for (auto const &item : m_fontInfoList) {
        filePaths << item->filePath;
    }

    qDebug() << "Install List: " << filePaths;

    m_fontManager->setType(DFontManager::Install);
    m_fontManager->setInstallFileList(filePaths);
    m_fontManager->start();
}

void MultiFilePage::onProgressChanged(const QString &filePath, const double &percent)
{
    m_bottomLayout->setCurrentIndex(1);
    m_installBtn->setVisible(false);
    m_viewFileBtn->setVisible(false);
    m_progress->setVisible(true);
    m_progress->setValue(static_cast<int>(percent));

    ListItem *item = m_listItems.find(filePath).value();
    item->setStatus(ListItem::Installed);
    m_listView->update();

    emit installing();

    if (percent == 100) {
        qDebug() << "Install finished";

        m_refreshThread->start();
        m_spinner->start();
        m_bottomLayout->setCurrentIndex(2);
    }
}

void MultiFilePage::onWorkerFinished()
{
    m_spinner->stop();
    m_bottomLayout->setCurrentIndex(0);
    m_tipsLabel->setVisible(true);
    m_installBtn->setVisible(false);
    m_viewFileBtn->setVisible(true);
    m_progress->setVisible(false);
    m_progress->setValue(0);

    refreshList();
    emit installFinished();
}

void MultiFilePage::onViewFileBtnClicked()
{
    QUrl url = QUrl::fromLocalFile("/usr/share/fonts/deepin-font-install");
    QProcess::startDetached("dde-file-manager", QStringList() << url.toString());
}

void MultiFilePage::handleCloseBtnClicked(DSimpleListItem *item)
{
    ListItem *listItem = static_cast<ListItem *>(item);
    DFontInfo *fontInfo = listItem->getFontInfo();

    m_listItems.remove(fontInfo->filePath);
    m_fontInfoList.removeOne(fontInfo);
    m_listView->removeItem(item);

    emit countChanged();
}
