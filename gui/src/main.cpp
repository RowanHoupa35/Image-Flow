/**
 * @file main.cpp (GUI)
 * @brief Entry point for the ImageFlow GUI application
 *
 * Initializes the Qt application, registers all filters, sets up the dark
 * theme, and launches the main window.
 *
 * @details
 * Initialization Order:
 * 1. Register all filters with FilterFactory (before QApplication)
 * 2. Create QApplication instance
 * 3. Configure Fusion style with dark palette
 * 4. Create and show MainWindow
 * 5. Enter Qt event loop
 *
 * Dark Theme:
 * - Uses Qt's Fusion style for cross-platform consistency
 * - Custom dark palette for better image contrast visibility
 * - Professional appearance suitable for image editing
 *
 * Filter Registration:
 * - Calls registerAllFilters() before GUI creation
 * - Ensures filters are available when MainWindow builds its menu
 * - Static initialization in FilterRegistration.cpp also runs
 *
 * @see MainWindow.hpp for main window class
 * @see FilterRegistration.cpp for filter registration
 * @author Rowan HOUPA
 * @date January 2026
 */

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include "MainWindow.hpp"
#include "FilterFactory.hpp"
#include <iostream>

// Declare the filter registration function
extern void registerAllFilters();

int main(int argc, char* argv[]) {
    // IMPORTANT: Initialize filters before creating GUI
    std::cout << "Initializing filter factory..." << std::endl;
    registerAllFilters();
    std::cout << "Filters registered: " << FilterFactory::instance().getFilterIds().size() << std::endl;

    QApplication app(argc, argv);

    // Set Fusion style for modern look
    app.setStyle(QStyleFactory::create("Fusion"));

    // Set dark palette for image processing
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);

    app.setApplicationName("ImageFlow");
    app.setApplicationVersion("1.0.0");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}