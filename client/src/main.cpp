#include <QtWidgets/QApplication>
#include <QDebug>
#include "login/login_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);

    MINIOICQ::LoginWindow login_window;
    if(login_window.exec() == QDialog::Accepted) {
        qDebug() << "Login success";
    } else {
        qDebug() << "Login failed";
    }

    return a.exec();
}
