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

    // Modern clean styling
    app.setStyle(QStyleFactory::create("Fusion"));

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
