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

#include "TcpManager.h"
#include "Headers/resetdialog.h"
#include "Forms/ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    setWindowIcon(QIcon(":/resources/icons/icon_small.ico"));
    ui->setupUi(this);
    loginDialog = new LoginDialog(this);
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(loginDialog);
    // loginDialog->show();
    // 创建和注册消息链接
    connect(loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(loginDialog, &LoginDialog::switchResetPwd, this, &MainWindow::SlotSwitchReset);
    connect(TcpManager::GetInstance().get(), &TcpManager::sig_switch_chatdlg, this, &MainWindow::SlotSwitchChat);
    emit TcpManager::GetInstance()->sig_switch_chatdlg();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SlotSwitchReg() {
    registerDialog = new RegisterDialog(this);
    registerDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    // 连接注册界面返回登录信号
    connect(registerDialog, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
    setCentralWidget(registerDialog);
    loginDialog->hide();
    registerDialog->show();
}

void MainWindow::SlotSwitchLogin() {
    // 创建一个CentralWidget，并将其设置为MainWindow的中心部件
    loginDialog = new LoginDialog(this);
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(loginDialog);

    registerDialog->hide();
    loginDialog->show();

    connect(loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(loginDialog, &LoginDialog::switchResetPwd, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset() {
    resetDialog = new ResetDialog(this);
    resetDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(resetDialog);

    loginDialog->hide();
    resetDialog->show();

    connect(resetDialog, &ResetDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLoginFromReset);
}

void MainWindow::SlotSwitchLoginFromReset() {
    loginDialog = new LoginDialog(this);
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(loginDialog);

    resetDialog->hide();
    loginDialog->show();

    connect(loginDialog, &LoginDialog::switchResetPwd, this, &MainWindow::SlotSwitchReset);
    connect(loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}

void MainWindow::SlotSwitchChat() {
    chatDialog = new ChatDialog();
    chatDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(chatDialog);
    chatDialog->show();
    loginDialog->hide();
    this->setMinimumSize(QSize(1050, 700));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}
