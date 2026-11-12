#pragma once

#include <QWidget>
#include <QColor>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QToolButton>

class ColorPalette : public QWidget {
    Q_OBJECT

public:
    explicit ColorPalette(QWidget* parent = nullptr);

    QColor color1() const { return m_color1; }
    QColor color2() const { return m_color2; }
    bool isEditingColor1() const { return m_editingColor1; }

public slots:
    void setColor1(const QColor& c);
    void setColor2(const QColor& c);
    void selectColor1Mode();
    void selectColor2Mode();

signals:
    void color1Changed(const QColor& c);
    void color2Changed(const QColor& c);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onEditColorsClicked();

private:
    void setupUI();
    void updatePreviews();
    void updateCustomPalette();

private:
    QColor m_color1 = Qt::black;
    QColor m_color2 = Qt::white;
    bool m_editingColor1 = true;

    QWidget* m_card1 = nullptr;
    QWidget* m_card2 = nullptr;
    QLabel* m_swatchPreview1 = nullptr;
    QLabel* m_swatchPreview2 = nullptr;

    QVector<QColor> m_row1;
    QVector<QColor> m_row2;
    QVector<QColor> m_row3;
    QVector<QToolButton*> m_customButtons;

    QToolButton* m_editColorsBtn = nullptr;
};
