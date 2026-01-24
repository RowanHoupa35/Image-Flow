/**
 * @file test_qt_basic.cpp
 * @brief Basic Qt5 installation and functionality test
 *
 * Minimal Qt application to verify that Qt5 is properly installed
 * and can create/display GUI widgets.
 *
 * @details
 * Test: Creates a simple button that quits the application when clicked.
 *
 * What it Verifies:
 * - Qt5 libraries are found and linked correctly
 * - QApplication can be instantiated
 * - QPushButton can be created and displayed
 * - Signal/slot connections work (clicked → quit)
 *
 * Library Path:
 * Sets LD_LIBRARY_PATH to standard system locations to avoid
 * conflicts with Intel oneAPI or other custom library paths.
 *
 * @note Click the button to exit successfully
 * @author ENSTA Paris - IN204 Project
 * @date January 2026
 */

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