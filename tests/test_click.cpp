/**
 * @file test_click.cpp
 * @brief Qt signal/slot connection verification test
 *
 * Tests Qt's signal/slot mechanism with debug output to verify
 * that button clicks are properly detected and handled.
 *
 * @details
 * Test: Button in a vertical layout that quits on click.
 *
 * What it Verifies:
 * - QWidget and QVBoxLayout work correctly
 * - SIGNAL/SLOT macros connect properly
 * - qDebug() outputs to console
 * - Event loop processes click events
 *
 * Debug Output:
 * Prints "Button created, waiting for click..." to console
 * to confirm initialization completed successfully.
 *
 * @note Uses old-style SIGNAL/SLOT syntax for Qt4 compatibility
 * @author ENSTA Paris - IN204 Project
 * @date January 2026
 */

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);
    
    QPushButton *button = new QPushButton("Test Button");
    layout->addWidget(button);
    
    QObject::connect(button, SIGNAL(clicked()), &app, SLOT(quit()));
    
    qDebug() << "Button created, waiting for click...";
    window.show();
    
    return app.exec();
}
