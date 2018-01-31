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
#include <QFormLayout>
#include <QSvgWidget>

ListItem::ListItem(QWidget *parent)
    : QWidget(parent),
      m_item(new QListWidgetItem),
      m_fontInfo(nullptr),
      m_nameLabel(new QLabel),
      m_styleLabel(new QLabel),
      m_infoLabel(new QLabel),
      m_statusLabel(new QLabel),
      m_closeBtn(new DImageButton (":/images/close_normal.svg",
                                   ":/images/close_hover.svg",
                                   ":/images/close_press.svg"))
{
    QSvgWidget *iconWidget = new QSvgWidget(":/images/font-x-generic.svg");
    iconWidget->setFixedSize(47, 45);

    m_nameLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: 510; color: #000000; }");
    m_styleLabel->setStyleSheet("QLabel { font-size: 12px; color: #000000; }");

    m_closeBtn->setFixedSize(24, 24);
    m_closeBtn->hide();

    QWidget *formWidget = new QWidget;
    QFormLayout *formLayout = new QFormLayout(formWidget);
    formLayout->addRow(m_nameLabel, m_styleLabel);
    formLayout->addRow(m_infoLabel);
    formLayout->setHorizontalSpacing(8);
    formLayout->setVerticalSpacing(0);
    formLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(iconWidget);
    mainLayout->addWidget(formWidget, 0, Qt::AlignVCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(m_closeBtn);
    mainLayout->addSpacing(5);
    mainLayout->setMargin(0);

    connect(m_closeBtn, &DImageButton::clicked, this, [=] {
       emit closeBtnClicked(m_item);
    });
}

void ListItem::updateInfo(DFontInfo *p)
{
    m_fontInfo = p;

    const bool isInstalled = m_fontInfo->isInstalled;
    const bool isSampleVersion = m_fontInfo->sysVersion == m_fontInfo->version;

    m_nameLabel->setText(m_fontInfo->familyName);
    m_styleLabel->setText(m_styleLabel->fontMetrics().elidedText(m_fontInfo->styleName,
                                                                 Qt::ElideRight, 180));

    m_infoLabel->setStyleSheet("QLabel { color: #5A5A5A; font-size: 14px; }");
    if (isInstalled) {
        m_infoLabel->setStyleSheet("QLabel { color: #47790C; font-size: 14px; }");

        if (isSampleVersion) {
            m_infoLabel->setText(tr("Same version installed"));
        } else {
            m_infoLabel->setText(QString(tr("Other version installed: %1")).arg(m_fontInfo->sysVersion));
        }
    } else {
        m_infoLabel->setStyleSheet("QLabel { color: #5A5A5A; font-size: 14px; }");
        if (m_fontInfo->description.isEmpty()) {
            m_infoLabel->setText(tr("Unknown"));
        } else {
            m_infoLabel->setText(m_infoLabel->fontMetrics().elidedText(m_fontInfo->description,
                                                                       Qt::ElideRight,
                                                                       m_infoLabel->width() / 1.9));
        }
    }
}

void ListItem::setStatus(Status status)
{
    m_closeBtn->hide();
    m_statusLabel->setStyleSheet("QLabel { color: #7C7C7C; }");

    switch (status) {
    case None:
        m_statusLabel->setText("");
        break;
    case Installed:
        m_status = Installed;
        m_statusLabel->setStyleSheet("QLabel { color: #528315; }");
        m_statusLabel->setText(tr("Installed"));
        break;
    case Installing:
        m_status = Installing;
        m_statusLabel->setText(tr("Installing"));
    break;
    case Waiting:
        m_statusLabel->setText("Waiting");
        break;
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
    if (m_status == Installed) {
        return;
    }

    m_closeBtn->show();
    m_closeBtn->raise();
    m_statusLabel->hide();

    QWidget::enterEvent(e);
}

void ListItem::leaveEvent(QEvent *e)
{
    if (m_status == Installed) {
        return;
    }

    m_closeBtn->hide();
    m_statusLabel->show();

    QWidget::leaveEvent(e);
}
