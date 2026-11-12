#pragma once

#include <QWidget>
#include <QToolButton>
#include <QCheckBox>

class ViewTab : public QWidget {
    Q_OBJECT

public:
    explicit ViewTab(QWidget* parent = nullptr);

signals:
    void zoomInTriggered();
    void zoomOutTriggered();
    void zoom100Triggered();
    void gridlinesToggled(bool show);
    void rulersToggled(bool show);
    void statusBarToggled(bool show);
    void fullScreenTriggered();

private:
    QCheckBox* m_checkGrid = nullptr;
    QCheckBox* m_checkRulers = nullptr;
    QCheckBox* m_checkStatus = nullptr;
};
