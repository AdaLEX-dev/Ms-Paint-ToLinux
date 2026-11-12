#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QCheckBox>

class ResizeSkewDialog : public QDialog {
    Q_OBJECT

public:
    explicit ResizeSkewDialog(int curW, int curH, QWidget* parent = nullptr);

    double scaleX() const;
    double scaleY() const;
    double skewXDeg() const { return m_skewH->value(); }
    double skewYDeg() const { return m_skewV->value(); }

private:
    int m_origW;
    int m_origH;

    QRadioButton* m_radioPercent;
    QRadioButton* m_radioPixels;

    QSpinBox* m_resizeH;
    QSpinBox* m_resizeV;
    QCheckBox* m_aspectRatio;

    QDoubleSpinBox* m_skewH;
    QDoubleSpinBox* m_skewV;
};
