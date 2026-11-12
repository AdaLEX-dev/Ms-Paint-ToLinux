#pragma once

#include <QWidget>
#include <QColor>
#include <QVector>
#include <QPushButton>

class ColorPalette : public QWidget {
    Q_OBJECT

public:
    explicit ColorPalette(QWidget* parent = nullptr);

    QColor color1() const { return m_color1; }
    QColor color2() const { return m_color2; }

public slots:
    void setColor1(const QColor& c);
    void setColor2(const QColor& c);

signals:
    void color1Changed(const QColor& c);
    void color2Changed(const QColor& c);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void onEditColorsClicked();
    QRect getColor1Rect() const;
    QRect getColor2Rect() const;
    QRect getSwatchRect(int row, int col) const;

private:
    QColor m_color1 = Qt::black;
    QColor m_color2 = Qt::white;
    bool m_editingColor1 = true;

    QVector<QColor> m_row1;
    QVector<QColor> m_row2;
    QVector<QColor> m_row3; // Custom colors

    QPushButton* m_editColorsBtn = nullptr;
};
