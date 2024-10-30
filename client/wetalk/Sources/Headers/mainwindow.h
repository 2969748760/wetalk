/**
  ******************************************************************************
  * @File           : mainwindow.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/24
  ******************************************************************************
 */

#ifndef WETALK_MAINWINDOW_H
#define WETALK_MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void SlotSwitchReg();

    void SlotSwitchLogin();

private:
    Ui::MainWindow *ui;
    LoginDialog *loginDialog;
    RegisterDialog *registerDialog;
};


#endif //WETALK_MAINWINDOW_H
