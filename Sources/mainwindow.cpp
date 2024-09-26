/**
  ******************************************************************************
  * @File           : mainwindow.cpp
  * @Author         : 钟先生
  * @Brief          : 主界面源码
  * @Date           : 2024/09/24
  ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "Headers/mainwindow.h"
#include "Forms/ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    setWindowIcon(QIcon(":/resources/icons/icon_small.ico"));
    ui->setupUi(this);
    loginDialog = new LoginDialog();
    setCentralWidget(loginDialog);
    loginDialog->show();
    // 创建和注册消息链接
    connect(loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    registerDialog = new RegisterDialog();
}

MainWindow::~MainWindow() {
    delete ui;
    if (loginDialog) {
        delete loginDialog;
        loginDialog = nullptr;
    }
    if (registerDialog) {
        delete registerDialog;
        registerDialog = nullptr;
    }
}

void MainWindow::SlotSwitchReg() {
    setCentralWidget(registerDialog);
    loginDialog->hide();
    registerDialog->show();
}
