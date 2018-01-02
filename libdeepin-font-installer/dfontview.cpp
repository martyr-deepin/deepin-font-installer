#include "dfontview.h"
#include <QFontDatabase>

static QString content = "abcdefghijklmnopqrstuvwxyz\n"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                         "0123456789.:,;(*!?')\n";

DFontView::DFontView(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(new QVBoxLayout(this)),
      m_topLabel(new QLabel)
{
    m_mainLayout->addWidget(m_topLabel, 0, Qt::AlignTop);
}

void DFontView::setFileUrl(const QString &url)
{
    const int id = QFontDatabase::addApplicationFont(url);
    const QString family = QFontDatabase::applicationFontFamilies(id).first();
    QFont font(family);
    font.setPointSize(28);
    m_topLabel->setFont(font);
}
