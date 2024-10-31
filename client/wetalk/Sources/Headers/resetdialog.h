//
// Created by 29697 on 24-10-31.
//

#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>

#include "global.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class ResetDialog;
}

QT_END_NAMESPACE

class ResetDialog : public QDialog {
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);

    ~ResetDialog() override;

private slots:
    void on_return_btn_clicked();

    void on_verify_btn_clicked();

    void slot_reset_mod_finish(ReqId reqId, QString res, ErrorCodes error);

    void on_sure_btn_clicked();

private:
    Ui::ResetDialog *ui;

    bool checkUserValid();

    bool checkPwdValid();

    bool checkEmailValid();

    bool checkVerifyValid();

    void showTips(const QString &tips, bool b_ok);

    void AddTipError(TipError error, const QString &tips);

    void DelTipError(TipError error);

    void initHandlers();

    QMap<TipError, QString> _tip_errors;
    QMap<ReqId, std::function<void(const QJsonObject &)> > _handlers;

signals:
    void sigSwitchLogin();
};


#endif //RESETDIALOG_H
