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


QT_BEGIN_NAMESPACE
namespace Ui { class RegisterDialog; }
QT_END_NAMESPACE

class RegisterDialog : public QDialog {
Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog() override;

private:
    Ui::RegisterDialog *ui;
};


#endif //WETALK_REGISTERDIALOG_H
