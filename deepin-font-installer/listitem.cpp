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

#include "listitem.h"
#include "utils.h"
#include <QPainter>

ListItem::ListItem(QWidget *parent)
    : QWidget(parent),
      m_item(new QListWidgetItem),
      m_fontData(nullptr),
      m_layout(new QHBoxLayout(this)),
      m_infoLayout(new QVBoxLayout),
      m_nameLabel(new QLabel),
      m_styleLabel(new QLabel),
      m_infoLabel(new QLabel("不知道显示什么")),
      m_statusLabel(new QLabel),
      m_closeBtn(new DImageButton (":/images/close_normal.svg",
                                   ":/images/close_hover.svg",
                                   ":/images/close_press.svg"))
{
    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(m_nameLabel);
    nameLayout->addWidget(m_styleLabel);

    m_styleLabel->setStyleSheet("QLabel { color: #909090; }");

    m_closeBtn->setFixedSize(24, 24);
    m_closeBtn->setVisible(false);

    m_infoLayout->addLayout(nameLayout);
    m_infoLayout->addWidget(m_infoLabel);

    m_layout->addLayout(m_infoLayout);
    m_layout->addStretch();
    m_layout->addWidget(m_closeBtn);
    m_layout->addWidget(m_statusLabel);

    connect(m_closeBtn, &DImageButton::clicked, this, [=] {
        emit closeBtnClicked(m_item);
    });
}

QListWidgetItem *ListItem::getItem()
{
    return m_item;
}

QString ListItem::getFilePath() const
{
    return m_filePath;
}

void ListItem::setFontData(DFontData *p)
{
    if (DFontInfo::isFontInstalled(p)) {
        m_statusLabel->setText(tr("Installed"));
    }

    m_fontData = p;
    m_nameLabel->setText(m_fontData->familyName);
    m_styleLabel->setText(m_fontData->styleName);
}

DFontData *ListItem::getFontData()
{
    return m_fontData;
}

void ListItem::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#e4e4e4"));
    painter.drawRect(QRect(0, rect().height() - 1, rect().width(), 1));
}

void ListItem::enterEvent(QEvent *e)
{
    m_closeBtn->setVisible(true);
    m_statusLabel->setVisible(false);

    QWidget::enterEvent(e);
}

void ListItem::leaveEvent(QEvent *e)
{
    m_closeBtn->setVisible(false);
    m_statusLabel->setVisible(true);

    QWidget::leaveEvent(e);
}
