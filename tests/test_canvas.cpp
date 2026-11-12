#include <QApplication>
#include <cassert>
#include <iostream>
#include "../src/Canvas/PaintCanvas.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::cout << "[Test] Initializing PaintCanvas..." << std::endl;
    PaintCanvas canvas;
    canvas.newImage(800, 600, Qt::white);
    assert(canvas.imageSize() == QSize(800, 600));

    std::cout << "[Test] Testing Color & Tool Selection..." << std::endl;
    canvas.setColor1(Qt::red);
    canvas.setColor2(Qt::blue);
    assert(canvas.color1() == Qt::red);
    assert(canvas.color2() == Qt::blue);

    canvas.setActiveTool(ActiveTool::Pencil);
    assert(canvas.activeTool() == ActiveTool::Pencil);

    canvas.setActiveTool(ActiveTool::Shape);
    canvas.setShapeType(ShapeType::Oval);
    assert(canvas.shapeType() == ShapeType::Oval);

    std::cout << "[Test] Testing Transformations..." << std::endl;
    canvas.rotate90CW();
    assert(canvas.imageSize() == QSize(600, 800));

    canvas.rotate90CCW();
    assert(canvas.imageSize() == QSize(800, 600));

    canvas.flipHorizontal();
    canvas.flipVertical();
    canvas.invertColors();

    std::cout << "[Test] Testing Undo/Redo..." << std::endl;
    assert(canvas.canUndo());
    canvas.undo();
    assert(canvas.canRedo());
    canvas.redo();

    std::cout << "[Test] Testing Save/Load..." << std::endl;
    bool saved = canvas.saveImage("test_sample.png", "PNG");
    assert(saved);

    PaintCanvas loader;
    bool loaded = loader.openImage("test_sample.png");
    assert(loaded);
    assert(loader.imageSize() == canvas.imageSize());

    std::cout << "[Test] Testing ColorPicker & Text Tools..." << std::endl;
    // Draw a green patch
    canvas.newImage(200, 200, Qt::white);
    QPainter p(&canvas.image());
    p.fillRect(10, 10, 20, 20, Qt::green);
    p.end();

    // Select color picker and pick green
    canvas.setActiveTool(ActiveTool::Pencil);
    canvas.setActiveTool(ActiveTool::ColorPicker);
    assert(canvas.activeTool() == ActiveTool::ColorPicker);

    // Simulate mouse click on the green patch
    QMouseEvent pressEv(QEvent::MouseButtonPress, QPointF(15, 15), QPointF(15, 15), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QMouseEvent releaseEv(QEvent::MouseButtonRelease, QPointF(15, 15), QPointF(15, 15), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(&canvas, &pressEv);
    QApplication::sendEvent(&canvas, &releaseEv);

    // Assert picked color is green and tool returned to Pencil
    assert(canvas.color1() == Qt::green);
    assert(canvas.activeTool() == ActiveTool::Pencil);

    // Test text tool initialization
    canvas.setActiveTool(ActiveTool::Text);
    assert(canvas.color1() != Qt::white); // Must not be white on white

    std::cout << "[Test] ALL TESTS PASSED SUCCESSFULLY!" << std::endl;
    return 0;
}
