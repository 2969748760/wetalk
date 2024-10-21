/**
  ******************************************************************************
  * @File           : main.cpp
  * @Author         : 钟先生
  * @Brief          : 主窗口
  * @Date           : 24-9-24
  ******************************************************************************
 */

#include <QApplication>
#include "Headers/mainwindow.h"
#include "Headers/global.h"
#include <QString>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QFile qss(":/style/stylesheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug("open success");
        QString style = QString::fromUtf8(QLatin1String(qss.readAll()));
        a.setStyleSheet(style);
        qss.close();
    } else {
        qDebug("open failed");
    }

    QString app_path = QCoreApplication::applicationDirPath();
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);
    std::cout << "config_path is " << config_path.toStdString() << std::endl;
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;
    std::cout << "gate_url_prefix is " << gate_url_prefix.toStdString() << std::endl;

    MainWindow w;
    w.show();
    return a.exec();
}
