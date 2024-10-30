/**
  ******************************************************************************
  * @File           : logindialog.cpp
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/24
  ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_LoginDialog.h" resolved

#include "Headers/logindialog.h"
#include "Forms/ui_LoginDialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
}

LoginDialog::~LoginDialog() {
    qDebug() << "login dialog destruct";
    delete ui;
}
