#include "dfontview.h"

static QString lowerCaseText = "abcdefghijklmnopqrstuvwxyz";
static QString upperCaseText = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static QString punctuationText = "0123456789.:,;(*!?')";

DFontView::DFontView(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(new QVBoxLayout(this))
{
}
