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
