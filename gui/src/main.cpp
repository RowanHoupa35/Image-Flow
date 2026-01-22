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