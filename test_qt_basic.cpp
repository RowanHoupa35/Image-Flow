#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    // Force system libraries
    setenv("LD_LIBRARY_PATH", "/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu", 1);
    
    QApplication app(argc, argv);
    QPushButton btn("Click to test - should work now!");
    QObject::connect(&btn, &QPushButton::clicked, &app, &QApplication::quit);
    btn.show();
    return app.exec();
}