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
#include <QSvgWidget>

ListItem::ListItem(QWidget *parent)
    : QWidget(parent),
      m_item(new QListWidgetItem),
      m_fontData(nullptr),
      m_mainLayout(new QHBoxLayout(this)),
      m_infoLayout(new QVBoxLayout),
      m_nameLabel(new QLabel),
      m_styleLabel(new QLabel),
      m_infoLabel(new QLabel),
      m_statusLabel(new QLabel),
      m_closeBtn(new DImageButton (":/images/close_normal.svg",
                                   ":/images/close_hover.svg",
                                   ":/images/close_press.svg"))
{
    QSvgWidget *iconWidget = new QSvgWidget(":/images/font-x-generic.svg");
    iconWidget->setFixedSize(50, 50);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(m_nameLabel);
    nameLayout->addSpacing(5);
    nameLayout->addWidget(m_styleLabel);
    nameLayout->addStretch();

    m_infoLabel->setStyleSheet("QLabel { color: #444444; }");
    m_styleLabel->setStyleSheet("QLabel { color: #909090; }");

    m_closeBtn->setFixedSize(24, 24);
    m_closeBtn->hide();

    m_infoLayout->addLayout(nameLayout);
    m_infoLayout->addWidget(m_infoLabel);

    m_mainLayout->addWidget(iconWidget);
    m_mainLayout->addLayout(m_infoLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_closeBtn);
    m_mainLayout->addWidget(m_statusLabel);

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
    const QFontMetrics fm = m_infoLabel->fontMetrics();

    if (p->isInstalled) {
        m_statusLabel->setStyleSheet("QLabel { color: #528315; }");
        m_statusLabel->setText(tr("Installed"));
    }

    m_fontData = p;
    m_nameLabel->setText(m_fontData->familyName);
    m_styleLabel->setText(m_fontData->styleName);

    m_infoLabel->setText(fm.elidedText(m_fontData->description,
                                       Qt::ElideRight,
                                       this->width() / 1.8));
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
    painter.drawRect(QRect(10, rect().height() - 1, rect().width() - 20, 1));
}

void ListItem::enterEvent(QEvent *e)
{
    m_closeBtn->show();
    m_closeBtn->raise();
    m_statusLabel->hide();

    QWidget::enterEvent(e);
}

void ListItem::leaveEvent(QEvent *e)
{
    m_closeBtn->hide();
    m_statusLabel->show();

    QWidget::leaveEvent(e);
}
