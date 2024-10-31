/**
  ******************************************************************************
  * @File           : logindialog.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/24
  ******************************************************************************
 */

#ifndef WETALK_LOGINDIALOG_H
#define WETALK_LOGINDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LoginDialog;
}

QT_END_NAMESPACE

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    ~LoginDialog();

private:
    Ui::LoginDialog *ui;

private slots:
    void slot_forget_pwd();

signals:
    void switchRegister();

    void switchResetPwd();
};


#endif //WETALK_LOGINDIALOG_H
