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
namespace Ui { class RegisterDialog; }
QT_END_NAMESPACE

class RegisterDialog : public QDialog {
Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog() override;

private:
    void showTip(QString str, bool b_ok);
    void initHttpHandlers();
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    Ui::RegisterDialog *ui;

private slots:
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_get_code_clicked();
};


#endif //WETALK_REGISTERDIALOG_H
