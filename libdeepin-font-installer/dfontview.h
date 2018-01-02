#ifndef DFONTVIEW_H
#define DFONTVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class DFontView : public QWidget
{
    Q_OBJECT

public:
    DFontView(QWidget *parent = nullptr);

    void setFileUrl(const QString &url);

private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;
};

#endif // DFONTVIEW_H
