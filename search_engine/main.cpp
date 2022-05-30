#include "mainwindow.h"

#include <QApplication>
#include <QObject>
#include "./ui_caller.h"
// сторонние библиотеки
#include <nlohmann/json.hpp>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    Ui::MainWindow caller;
    caller.setupUi(&w);
    w.qLabel = caller.label;
    w.textEdit = caller.textEdit;
    w.set_initial();
    QObject::connect(caller.pushButton, &QPushButton::clicked, [&w]() {w.get_config();});
    QObject::connect(caller.pushButton_4, &QPushButton::clicked, [&w]() {w.set_config();});
    QObject::connect(caller.pushButton_2, &QPushButton::clicked, [&w]() {w.set_request();});
    QObject::connect(caller.pushButton_3, &QPushButton::clicked, [&w]() {w.send_request();});
    QObject::connect(caller.pushButton_5, &QPushButton::clicked, [&w]() {w.get_answer();});

    w.show();
    return a.exec();
}
