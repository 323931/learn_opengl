#include "glwindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QMainWindow window;
    window.setWindowTitle("OpenGL Episode 5 - Hello Triangle");
    window.resize(800, 600);

    auto* glWidget = new GLWindow(&window);
    window.setCentralWidget(glWidget);

    window.show();
    return app.exec();
}
