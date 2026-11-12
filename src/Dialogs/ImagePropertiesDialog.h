#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QRadioButton>

class ImagePropertiesDialog : public QDialog {
    Q_OBJECT

public:
    explicit ImagePropertiesDialog(int w, int h, QWidget* parent = nullptr);

    int canvasWidth() const;
    int canvasHeight() const;

private:
    QSpinBox* m_spinW;
    QSpinBox* m_spinH;
    QRadioButton* m_unitPixels;
    QRadioButton* m_unitInches;
    QRadioButton* m_unitCm;
};
