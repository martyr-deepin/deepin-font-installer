#include "dfontview.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QDebug>

static const QString lowerTextStock = "abcdefghijklmnopqrstuvwxyz";
static const QString upperTextStock = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const QString punctuationTextStock = "0123456789.:,;(*!?')";
static QString sampleString = nullptr;
static QString styleName = nullptr;
static QHash<QString, QString> contents = {};

DFontView::DFontView(QWidget *parent)
    : QWidget(parent),
      m_library(0),
      m_face(0),
      m_fontDatabase(new QFontDatabase)
{
    initContents();

    setFixedSize(800, 550);
}

DFontView::~DFontView()
{
}

void DFontView::setFileUrl(const QString &url)
{
    m_fontDatabase->removeAllApplicationFonts();
    m_fontDatabase->addApplicationFont(url);

    FT_Init_FreeType(&m_library);
    FT_New_Face(m_library, url.toUtf8().constData(), 0, &m_face);

    sampleString = getSampleString().simplified();
    styleName = (char *) m_face->style_name;

    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);

    repaint();
}

void DFontView::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QFont font(m_fontDatabase->applicationFontFamilies(0).first());
    painter.setPen(Qt::black);

    if (styleName.contains("Italic")) {
        font.setItalic(true);
    }

    if (styleName.contains("Regular")) {
        font.setWeight(QFont::Normal);
    } else if (styleName.contains("Bold")) {
        font.setWeight(QFont::Bold);
    } else if (styleName.contains("Light")) {
        font.setWeight(QFont::Light);
    } else if (styleName.contains("Thin")) {
        font.setWeight(QFont::Thin);
    } else if (styleName.contains("ExtraLight")) {
        font.setWeight(QFont::ExtraLight);
    } else if (styleName.contains("ExtraBold")) {
        font.setWeight(QFont::ExtraBold);
    } else if (styleName.contains("Medium")) {
        font.setWeight(QFont::Medium);
    } else if (styleName.contains("DemiBold")) {
        font.setWeight(QFont::DemiBold);
    } else if (styleName.contains("Black")) {
        font.setWeight(QFont::Black);
    }

    const int padding = 20;
    int y = 10;
    int fontSize = 25;

    font.setPointSize(fontSize);
    painter.setFont(font);

    const QFontMetrics metrics(font);
    const int lowerWidth = metrics.width(lowerTextStock);
    const int lowerHeight = metrics.height();
    painter.drawText(QRect(20, y + padding, lowerWidth, lowerHeight), Qt::AlignLeft, lowerTextStock);
    y += lowerHeight;

    const int upperWidth = metrics.width(upperTextStock);
    const int upperHeight = metrics.height();
    painter.drawText(QRect(20, y + padding, upperWidth, upperHeight), Qt::AlignLeft, upperTextStock);
    y += upperHeight;

    const int punWidth = metrics.width(punctuationTextStock);
    int punHeight = metrics.height();
    painter.drawText(QRect(20, y + padding, punWidth, punHeight), Qt::AlignLeft, punctuationTextStock);
    y += punHeight;

    for (int i = 0; i < 5; ++i) {
        fontSize += 3;
        font.setPointSize(fontSize);
        painter.setFont(font);

        QFontMetrics met(font);
        int sampleWidth = met.width(sampleString);
        int sampleHeight = met.height();

        if (y + sampleHeight >= rect().height() - padding * 2)
            break;

        painter.drawText(QRect(20, y + padding * 2, sampleWidth , sampleHeight), Qt::AlignLeft, sampleString);
        y += sampleHeight + padding;
    }

    QWidget::paintEvent(e);
}

void DFontView::initContents()
{
    QFile file(":/CONTENTS.txt");

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray content = file.readAll();
    QTextStream stream(&content, QIODevice::ReadOnly);

    file.close();

    while (!stream.atEnd()) {
        const QString line = stream.readLine();
        const QStringList items = line.split(QChar(':'));

        contents.insert(items.at(0), items.at(1));
    }
}

QString DFontView::getSampleString()
{
    QString sampleString = nullptr;
    bool isAvailable = false;

    // check the current system language sample string.
    sampleString = getLanguageSampleString(QLocale::system().name());
    if (checkFontContainText(sampleString) && !sampleString.isEmpty()) {
        isAvailable = true;
    }

    // check english sample string.
    if (!isAvailable) {
        sampleString = getLanguageSampleString("en");
        if (checkFontContainText(sampleString)) {
            isAvailable = true;
        }
    }

    // random string from available chars.
    if (!isAvailable) {
        sampleString = buildCharlistForFace(36);
    }

    return sampleString;
}

QString DFontView::getLanguageSampleString(const QString &language)
{
    QString result = nullptr;
    QString key = nullptr;

    if (contents.contains(language)) {
        key = language;
    } else {
        const QStringList parseList = language.split("_", QString::SkipEmptyParts);
        if (parseList.length() > 0 &&
            contents.contains(parseList.first())) {
            key = parseList.first();
        }
    }

    auto findResult = contents.find(key);
    result.append(findResult.value());

    return result;
}

bool DFontView::checkFontContainText(const QString &text)
{
    bool retval = true;

    FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);

    for (auto ch : text) {
        if (!FT_Get_Char_Index(m_face, ch.unicode())) {
            retval = false;
            break;
        }
    }

    return retval;
}

QString DFontView::buildCharlistForFace(int length)
{
    unsigned int glyph;
    unsigned long ch;
    int totalChars = 0;
    QString retval;

    ch = FT_Get_First_Char(m_face, &glyph);

    while (glyph != 0) {
        retval.append(QChar((int) ch));
        ch = FT_Get_Next_Char(m_face, ch, &glyph);
        totalChars++;

        if (retval.count() == length)
            break;
    }

    return retval;
}
