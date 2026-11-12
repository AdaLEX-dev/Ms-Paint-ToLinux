#pragma once

#include <QWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QMenu>
#include "HomeTab.h"
#include "ViewTab.h"

class RibbonBar : public QWidget {
    Q_OBJECT

public:
    explicit RibbonBar(QWidget* parent = nullptr);

    HomeTab* homeTab() const { return m_homeTab; }
    ViewTab* viewTab() const { return m_viewTab; }
    QPushButton* fileButton() const { return m_fileButton; }

signals:
    void newTriggered();
    void openTriggered();
    void saveTriggered();
    void saveAsTriggered(const QString& format);
    void propertiesTriggered();
    void aboutTriggered();
    void exitTriggered();

private:
    void setupFileMenu();

private:
    QPushButton* m_fileButton = nullptr;
    QTabWidget* m_tabWidget = nullptr;
    HomeTab* m_homeTab = nullptr;
    ViewTab* m_viewTab = nullptr;
};
