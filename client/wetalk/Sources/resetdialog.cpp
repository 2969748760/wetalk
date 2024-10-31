//
// Created by 29697 on 24-10-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ResetDialog.h" resolved

#include "Headers/resetdialog.h"
#include "Forms/ui_ResetDialog.h"
#include <QDebug>
#include <QRegularExpression>

#include "Headers/global.h"
#include "Headers/httpmanager.h"


ResetDialog::ResetDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ResetDialog) {
    ui->setupUi(this);

    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this]() {
        checkUserValid();
    });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]() {
        checkEmailValid();
    });

    connect(ui->pwd_edit, &QLineEdit::editingFinished, this, [this]() {
        checkPwdValid();
    });

    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this]() {
        checkVerifyValid();
    });

    initHandlers();
    connect(HttpManager::GetInstance().get(), &HttpManager::sig_reset_mod_finish, this,
            &ResetDialog::slot_reset_mod_finish);
}

ResetDialog::~ResetDialog() {
    delete ui;
}

void ResetDialog::on_return_btn_clicked() {
    std::cout << "receive return btn clicked " << std::endl;
    emit sigSwitchLogin();
}

void ResetDialog::on_verify_btn_clicked() {
    std::cout << "receive verify btn clicked " << std::endl;
    auto email = ui->email_edit->text();
    auto bcheck = checkEmailValid();
    if (!bcheck) {
        return;;
    }
    QJsonObject json_obj;
    json_obj["email"] = email;
    HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verifycode"), json_obj,
                                            ReqId::ID_GET_VERIFY_CODE, Modules::RESETMOD);
}

void ResetDialog::slot_reset_mod_finish(ReqId reqId, QString res, ErrorCodes error) {
    if (error != ErrorCodes::SUCCESS) {
        showTips(tr("网络错误"), false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()) {
        showTips(tr("JSON为空"), false);
        return;
    }

    if (!jsonDoc.isObject()) {
        showTips(tr("json对象错误"), false);
        return;
    }

    _handlers[reqId](jsonDoc.object());

    return;
}

void ResetDialog::on_sure_btn_clicked() {
    bool valid = checkUserValid();
    if (!valid)return;
    valid = checkEmailValid();
    if (!valid)return;
    valid = checkPwdValid();
    if (!valid)return;
    valid = checkVerifyValid();
    if (!valid) return;

    QJsonObject json_object;
    json_object["user"] = ui->user_edit->text();
    json_object["email"] = ui->email_edit->text();
    json_object["passwd"] = md5Encrypt(ui->pwd_edit->text());
    json_object["verifycode"] = ui->verify_edit->text();

    HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/reset_pwd"), json_object, ReqId::ID_RESET_PWD,
                                            Modules::RESETMOD);
}

bool ResetDialog::checkUserValid() {
    qDebug() << "user is " << ui->user_edit->text();
    if (ui->user_edit->text().isEmpty() || ui->user_edit->text().length() == 0 || ui->user_edit->text() == "") {
        AddTipError(TipError::TIP_USER_ERROR, tr("用户名不能为空！"));
        return false;
    }
    DelTipError(TipError::TIP_USER_ERROR);
    return true;
}

bool ResetDialog::checkPwdValid() {
    auto pass = ui->pwd_edit->text();
    if (pass.isEmpty()) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码不能为空"));
        return false;
    }

    if (pass.length() < 6 || pass.length() > 15) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码长度应在6-15位之间"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if (!match) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码包含非法字符"));
        return false;
    }

    DelTipError(TipError::TIP_PWD_ERROR);
    return true;
}

bool ResetDialog::checkEmailValid() {
    auto email = ui->email_edit->text();
    qDebug() << "email is " << email;
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if (!match) {
        AddTipError(TipError::TIP_EMAIL_ERROR, tr("邮箱地址不正确"));
        return false;
    }
    DelTipError(TipError::TIP_EMAIL_ERROR);
    return true;
}

bool ResetDialog::checkVerifyValid() {
    auto verify_code = ui->verify_edit->text();
    if (verify_code.isEmpty()) {
        AddTipError(TipError::TIP_VERIFY_ERROR, tr("验证码不能为空"));
        return false;
    }
    DelTipError(TipError::TIP_VERIFY_ERROR);
    return true;
}

void ResetDialog::showTips(const QString &tips, bool b_ok) {
    if (b_ok) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "error");
    }

    ui->err_tip->setText(tips);
    repolish(ui->err_tip);
}

void ResetDialog::AddTipError(TipError error, const QString &tips) {
    _tip_errors[error] = tips;
    showTips(tips, false);
}

void ResetDialog::DelTipError(TipError error) {
    _tip_errors.remove(error);
    if (_tip_errors.isEmpty()) {
        ui->err_tip->clear();
        return;
    }
    showTips(_tip_errors.begin().value(), false);
}

void ResetDialog::initHandlers() {
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject &jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTips(tr("参数错误"), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTips("验证码已发送至邮箱: " + email, true);
        std::cout << "email is " << email.toStdString() << std::endl;
    });

    _handlers.insert(ReqId::ID_RESET_PWD, [this](const QJsonObject &jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTips(tr("参数错误"), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTips(tr("重置成功，点击返回登录"), true);
        std::cout << "email is " << email.toStdString() << std::endl;
        std::cout << "user uuid is " << jsonObj["uid"].toString().toStdString() << std::endl;
    });
}
