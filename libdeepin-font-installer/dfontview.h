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

private:
    QVBoxLayout *m_mainLayout;
};

#endif // DFONTVIEW_H
