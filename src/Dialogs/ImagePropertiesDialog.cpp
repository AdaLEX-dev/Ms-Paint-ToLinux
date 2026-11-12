#include "ImagePropertiesDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

ImagePropertiesDialog::ImagePropertiesDialog(int w, int h, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Image Properties");
    setFixedSize(320, 260);

    QVBoxLayout* root = new QVBoxLayout(this);

    // Units Group
    QGroupBox* grpUnits = new QGroupBox("Units", this);
    QHBoxLayout* unitLayout = new QHBoxLayout(grpUnits);
    m_unitInches = new QRadioButton("Inches", grpUnits);
    m_unitCm = new QRadioButton("Centimeters", grpUnits);
    m_unitPixels = new QRadioButton("Pixels", grpUnits);
    m_unitPixels->setChecked(true);
    unitLayout->addWidget(m_unitInches);
    unitLayout->addWidget(m_unitCm);
    unitLayout->addWidget(m_unitPixels);
    root->addWidget(grpUnits);

    // Dimensions Group
    QGroupBox* grpDim = new QGroupBox("Dimensions", this);
    QGridLayout* dimGrid = new QGridLayout(grpDim);

    dimGrid->addWidget(new QLabel("Width:", grpDim), 0, 0);
    m_spinW = new QSpinBox(grpDim);
    m_spinW->setRange(1, 10000);
    m_spinW->setValue(w);
    dimGrid->addWidget(m_spinW, 0, 1);

    dimGrid->addWidget(new QLabel("Height:", grpDim), 1, 0);
    m_spinH = new QSpinBox(grpDim);
    m_spinH->setRange(1, 10000);
    m_spinH->setValue(h);
    dimGrid->addWidget(m_spinH, 1, 1);

    root->addWidget(grpDim);

    // Buttons
    QHBoxLayout* btns = new QHBoxLayout();
    btns->addStretch(1);
    QPushButton* btnOk = new QPushButton("OK", this);
    QPushButton* btnCancel = new QPushButton("Cancel", this);
    connect(btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    btns->addWidget(btnOk);
    btns->addWidget(btnCancel);
    root->addLayout(btns);
}

int ImagePropertiesDialog::canvasWidth() const {
    return m_spinW->value();
}

int ImagePropertiesDialog::canvasHeight() const {
    return m_spinH->value();
}
