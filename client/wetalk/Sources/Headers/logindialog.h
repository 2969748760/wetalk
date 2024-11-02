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

#include "global.h"


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
    QMap<TipError, QString> _tip_errors;
    QMap<ReqId, std::function<void(const QJsonObject &)> > _handlers;
    int _uid;
    QString _token;

    void initHead();

    bool checkEmailValid();

    bool checkPwdValid();

    void AddTipError(TipError error, QString tips);

    void DelTipError(TipError error);

    void showTip(QString str, bool b_ok);

    bool enableBtn(bool cond);

    void initHttpHandlers();

private slots:
    void slot_forget_pwd();

    void on_login_btn_clicked();

    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes error);

signals:
    void switchRegister();

    void switchResetPwd();

    void sig_connect_tcp(ServerInfo);
};


#endif //WETALK_LOGINDIALOG_H
