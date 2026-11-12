#pragma once

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>
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

public slots:
    void selectHomeTab();
    void selectViewTab();

signals:
    void newTriggered();
    void openTriggered();
    void saveTriggered();
    void saveAsTriggered(const QString& format);
    void undoTriggered();
    void redoTriggered();
    void propertiesTriggered();
    void aboutTriggered();
    void exitTriggered();

private:
    void setupUI();
    void setupFileMenu();
    void updateTabStyles();

private:
    QPushButton* m_fileButton = nullptr;
    QPushButton* m_homeTabBtn = nullptr;
    QPushButton* m_viewTabBtn = nullptr;

    QStackedWidget* m_stack = nullptr;
    HomeTab* m_homeTab = nullptr;
    ViewTab* m_viewTab = nullptr;

    bool m_isHomeActive = true;
};
