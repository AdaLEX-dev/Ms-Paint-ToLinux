#include <QApplication>
#include <QStyleFactory>
#include <QFont>
#include "MainWindow.h"
#include "Ribbon/PaintIcons.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Paint");
    app.setApplicationDisplayName("Paint");
    app.setOrganizationName("Microsoft");
    app.setWindowIcon(PaintIcons::appIcon(64));

    // Modern clean light Windows 10 styling - overrides any system dark mode
    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette lightPal;
    lightPal.setColor(QPalette::Window, QColor(245, 246, 247));
    lightPal.setColor(QPalette::WindowText, QColor(0, 0, 0));
    lightPal.setColor(QPalette::Base, QColor(255, 255, 255));
    lightPal.setColor(QPalette::AlternateBase, QColor(245, 246, 247));
    lightPal.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
    lightPal.setColor(QPalette::ToolTipText, QColor(0, 0, 0));
    lightPal.setColor(QPalette::Text, QColor(0, 0, 0));
    lightPal.setColor(QPalette::Button, QColor(245, 246, 247));
    lightPal.setColor(QPalette::ButtonText, QColor(0, 0, 0));
    lightPal.setColor(QPalette::BrightText, QColor(255, 0, 0));
    lightPal.setColor(QPalette::Link, QColor(0, 120, 215));
    lightPal.setColor(QPalette::Highlight, QColor(0, 120, 215));
    lightPal.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    app.setPalette(lightPal);

    app.setStyleSheet(
        "QWidget { color: #000000; font-family: 'Segoe UI', Arial, sans-serif; }"
        "QToolButton { color: #000000; }"
        "QPushButton { color: #000000; }"
        "QLabel { color: #000000; }"
        "QMenu { background: #ffffff; color: #000000; border: 1px solid #c0c0c0; }"
        "QMenu::item { color: #000000; padding: 4px 24px; }"
        "QMenu::item:selected { background: #e5f1fb; color: #000000; }"
        "QCheckBox { color: #000000; }"
        "QStatusBar { color: #000000; }"
        "QStatusBar QLabel { color: #000000; }"
        "QLineEdit { color: #000000; background: #ffffff; }"
        "QTextEdit { color: #000000; background: #ffffff; }"
        "QDialog { color: #000000; background: #f5f6f7; }"
    );

    QFont font("Segoe UI", 9);
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);

    MainWindow window;
    window.show();

    if (argc > 1) {
        QString fileArg = QString::fromLocal8Bit(argv[1]);
        if (!fileArg.startsWith("-")) {
            window.loadFile(fileArg);
        }
    }

    return app.exec();
}
