#include "dfontview.h"
#include <QFontDatabase>

static const QString lowerTextStock = "abcdefghijklmnopqrstuvwxyz";
static const QString upperTextStock = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const QString punctuationTextStock = "0123456789.:,;(*!?')";

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

bool DFontView::checkFontContainText(FT_Face face, const QString &text)
{
    bool retval = true;

    FT_Select_Charmap(face, FT_ENCODING_UNICODE);

    for (auto ch : text) {
        if (!FT_Get_Char_Index(face, ch.unicode())) {
            retval = false;
            break;
        }
    }

    return retval;
}
