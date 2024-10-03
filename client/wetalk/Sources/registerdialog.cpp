/**
  ******************************************************************************
  * @File           : registerdialog.cpp
  * @Author         : 钟先生
  * @Brief          : XXX Fu
  * nction
  * @Date           : 2024/09/26
  ******************************************************************************
 */

// You may need to build the project (run Qt uic code generator) to get "ui_RegisterDialog.h" resolved

#include "Headers/registerdialog.h"
#include "Forms/ui_RegisterDialog.h"
#include "Headers/global.h"
#include "Headers/httpmanager.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirmpass_edit->setEchoMode(QLineEdit::Password);
    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);
    connect(HttpManager::GetInstance().get(), &HttpManager::sig_reg_mod_finish, this,
            &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}

void RegisterDialog::showTip(QString str, bool b_ok) {
    if (b_ok) {
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "error");
    }
    ui->error_tip->setText(str);
    repolish(ui->error_tip);
}

void RegisterDialog::on_get_code_clicked() {
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if (match) {
        // 发送验证码
        showTip(tr("验证码已发送"), true);
        qDebug("匹配成功");
    } else {
        showTip(tr("邮箱格式错误"), false);
        qDebug("匹配失败");
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err) {
    if (err != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);
        return;
    }
    // 解析json字符串，res转化为QbyteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()) {
        showTip(tr("json解析失败"), false);
        return;
    }
    // json解析错误
    if (jsonDoc.isObject()) {
        showTip(tr("json解析错误"), false);
        return;
    }

    _handlers[id](jsonDoc.object());
    return;
}

void RegisterDialog::initHttpHandlers() {
    // 注册获取验证码回包的逻辑
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject &jsonObject) {
        int error = jsonObject["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = jsonObject["email"].toString();
        showTip(tr("验证码已发送至邮箱: ") + email, true);
        qDebug() << "email is " << email;
    });
}
