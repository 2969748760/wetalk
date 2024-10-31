/**
  ******************************************************************************
  * @File           : registerdialog.h
  * @Author         : 钟先生
  * @Brief          : 注册界面
  * @Date           : 2024/09/26
  ******************************************************************************
 */

#ifndef WETALK_REGISTERDIALOG_H
#define WETALK_REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class RegisterDialog;
}

QT_END_NAMESPACE

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog() override;

private:
    void showTip(QString str, bool b_ok);

    void initHttpHandlers();

    bool checkUserValid();

    bool checkEmailValid();

    bool checkPassValid();

    bool checkConfirmPassValid();

    bool checkVerifyValid();

    void AddTipError(TipError error, QString tips);

    void DelTipError(TipError error);

    void ChangeTipPage();

    QMap<ReqId, std::function<void(const QJsonObject &)> > _handlers;
    QMap<TipError, QString> _tip_errors;
    Ui::RegisterDialog *ui;

    QTimer *_countdown_timer;
    int _countdown;

private slots:
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_get_code_clicked();

    void on_sure_btn_clicked();

    void on_return_btn_clicked();

    void on_cancel_btn_clicked();

signals:
    void sigSwitchLogin();
};


#endif //WETALK_REGISTERDIALOG_H
