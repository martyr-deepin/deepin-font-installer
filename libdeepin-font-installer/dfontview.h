#ifndef DFONTVIEW_H
#define DFONTVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include <ft2build.h>
#include FT_FREETYPE_H

class DFontView : public QWidget
{
    Q_OBJECT

public:
    DFontView(QWidget *parent = nullptr);

    void setFileUrl(const QString &url);

private:
    bool checkFontContainText(FT_Face face, const QString &text);
    QString buildCharlistForFace(FT_Face face, int length);

private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;
};

#endif // DFONTVIEW_H
