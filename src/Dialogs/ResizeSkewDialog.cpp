#include "ResizeSkewDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

ResizeSkewDialog::ResizeSkewDialog(int curW, int curH, QWidget* parent)
    : QDialog(parent), m_origW(curW), m_origH(curH)
{
    setWindowTitle("Resize and Skew");
    setFixedSize(300, 320);

    QVBoxLayout* root = new QVBoxLayout(this);

    // Resize Group
    QGroupBox* grpResize = new QGroupBox("Resize", this);
    QVBoxLayout* resLayout = new QVBoxLayout(grpResize);

    QHBoxLayout* modeLayout = new QHBoxLayout();
    m_radioPercent = new QRadioButton("Percentage", grpResize);
    m_radioPixels = new QRadioButton("Pixels", grpResize);
    m_radioPercent->setChecked(true);
    modeLayout->addWidget(m_radioPercent);
    modeLayout->addWidget(m_radioPixels);
    resLayout->addLayout(modeLayout);

    QGridLayout* spinGrid = new QGridLayout();
    spinGrid->addWidget(new QLabel("Horizontal:", grpResize), 0, 0);
    m_resizeH = new QSpinBox(grpResize);
    m_resizeH->setRange(1, 500);
    m_resizeH->setValue(100);
    spinGrid->addWidget(m_resizeH, 0, 1);

    spinGrid->addWidget(new QLabel("Vertical:", grpResize), 1, 0);
    m_resizeV = new QSpinBox(grpResize);
    m_resizeV->setRange(1, 500);
    m_resizeV->setValue(100);
    spinGrid->addWidget(m_resizeV, 1, 1);

    resLayout->addLayout(spinGrid);

    m_aspectRatio = new QCheckBox("Maintain aspect ratio", grpResize);
    m_aspectRatio->setChecked(true);
    resLayout->addWidget(m_aspectRatio);

    connect(m_resizeH, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int val) {
        if (m_aspectRatio->isChecked()) {
            m_resizeV->blockSignals(true);
            m_resizeV->setValue(val);
            m_resizeV->blockSignals(false);
        }
    });

    connect(m_radioPixels, &QRadioButton::toggled, this, [this](bool px) {
        if (px) {
            m_resizeH->setRange(1, 10000);
            m_resizeV->setRange(1, 10000);
            m_resizeH->setValue(m_origW);
            m_resizeV->setValue(m_origH);
        } else {
            m_resizeH->setRange(1, 500);
            m_resizeV->setRange(1, 500);
            m_resizeH->setValue(100);
            m_resizeV->setValue(100);
        }
    });

    root->addWidget(grpResize);

    // Skew Group
    QGroupBox* grpSkew = new QGroupBox("Skew (degrees)", this);
    QGridLayout* skewGrid = new QGridLayout(grpSkew);

    skewGrid->addWidget(new QLabel("Horizontal:", grpSkew), 0, 0);
    m_skewH = new QDoubleSpinBox(grpSkew);
    m_skewH->setRange(-89.0, 89.0);
    m_skewH->setValue(0.0);
    skewGrid->addWidget(m_skewH, 0, 1);

    skewGrid->addWidget(new QLabel("Vertical:", grpSkew), 1, 0);
    m_skewV = new QDoubleSpinBox(grpSkew);
    m_skewV->setRange(-89.0, 89.0);
    m_skewV->setValue(0.0);
    skewGrid->addWidget(m_skewV, 1, 1);

    root->addWidget(grpSkew);

    // Dialog buttons
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

double ResizeSkewDialog::scaleX() const {
    if (m_radioPixels->isChecked()) {
        return double(m_resizeH->value()) / m_origW;
    }
    return m_resizeH->value() / 100.0;
}

double ResizeSkewDialog::scaleY() const {
    if (m_radioPixels->isChecked()) {
        return double(m_resizeV->value()) / m_origH;
    }
    return m_resizeV->value() / 100.0;
}
