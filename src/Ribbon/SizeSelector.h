#pragma once

#include <QWidget>
#include <QToolButton>
#include <QMenu>

class SizeSelector : public QWidget {
    Q_OBJECT

public:
    explicit SizeSelector(QWidget* parent = nullptr);

    int selectedSize() const { return m_size; }

signals:
    void sizeChanged(int width);

private:
    int m_size = 1;
    QToolButton* m_button = nullptr;
    QMenu* m_menu = nullptr;
};
