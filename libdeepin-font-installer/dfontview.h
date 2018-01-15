#ifndef DFONTVIEW_H
#define DFONTVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFontDatabase>

#include <ft2build.h>
#include FT_FREETYPE_H

class DFontView : public QWidget
{
    Q_OBJECT

public:
    DFontView(QWidget *parent = nullptr);
    ~DFontView();

    void setFileUrl(const QString &url);

protected:
    void paintEvent(QPaintEvent *);

private:
    void initContents();
    QString getSampleString();
    QString getLanguageSampleString(const QString &language);
    bool checkFontContainText(const QString &text);
    QString buildCharlistForFace(int length);

private:
    FT_Library m_library;
    FT_Face m_face;
    QFontDatabase *m_fontDatabase;
};

#endif  // DFONTVIEW_H
