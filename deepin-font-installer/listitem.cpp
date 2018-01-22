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
#include <QVBoxLayout>
#include <QPainter>
#include <QSvgWidget>

ListItem::ListItem(QWidget *parent)
    : QWidget(parent),
      m_item(new QListWidgetItem),
      m_fontData(nullptr),
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

    m_nameLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: 500; color: #000000; }");
    m_styleLabel->setStyleSheet("QLabel { font-size: 13px; color: #000000; }");
    m_infoLabel->setStyleSheet("QLabel { color: #5A5A5A; }");

    m_closeBtn->setFixedSize(24, 24);
    m_closeBtn->hide();

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_nameLabel);
    topLayout->addSpacing(10);
    topLayout->addWidget(m_styleLabel);
    topLayout->addStretch();
    topLayout->setSpacing(true);

    QVBoxLayout *contentLayout = new QVBoxLayout;
    contentLayout->addStretch();
    contentLayout->addLayout(topLayout);
    contentLayout->addWidget(m_infoLabel);
    contentLayout->addStretch();
    contentLayout->setSpacing(0);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(iconWidget);
    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(m_closeBtn);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addSpacing(5);
    mainLayout->setMargin(0);

    connect(m_closeBtn, &DImageButton::clicked, this, [=] {
        emit closeBtnClicked(m_item);
    });
}

void ListItem::setFontInfo(DFontInfo *p)
{
    const QFontMetrics fm = m_infoLabel->fontMetrics();
    m_fontData = p;

    updateStatus();
    m_nameLabel->setText(m_fontData->familyName);
    m_styleLabel->setText(m_fontData->styleName);

    if (m_fontData->description.isEmpty()) {
        m_infoLabel->setText(tr("Unknown"));
    } else {
        m_infoLabel->setText(fm.elidedText(m_fontData->description,
                                           Qt::ElideRight,
                                           this->width() / 1.9));
    }
}

void ListItem::updateStatus()
{
    if (m_fontData->isInstalled) {
        m_statusLabel->setStyleSheet("QLabel { color: #528315; }");
        m_statusLabel->setText(tr("Installed"));
    } else {
        m_statusLabel->setText("");
    }
}

void ListItem::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    if (m_item->listWidget()->count() >= 1 &&
        m_item == m_item->listWidget()->item(m_item->listWidget()->count() - 1)) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QColor(151, 151, 151, 255 * 0.1));
    painter.drawLine(QPoint(60, rect().height() - 1),
                     QPoint(rect().width() - 5, rect().height() - 1));
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
