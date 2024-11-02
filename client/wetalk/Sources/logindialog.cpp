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

#include "Headers/ClickedLabel.h"
#include "Forms/ui_LoginDialog.h"
#include <QPainter>
#include <QPainterPath>

#include "Headers/httpmanager.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");
    ui->forget_label->setCursor(Qt::PointingHandCursor);
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);
    initHead();
    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);
    ui->error_tip->clear();
    initHttpHandlers();
    connect(HttpManager::GetInstance().get(), &HttpManager::sig_login_mod_finish, this,
            &LoginDialog::slot_login_mod_finish);
}

LoginDialog::~LoginDialog() {
    qDebug() << "login dialog destruct";
    delete ui;
}

void LoginDialog::initHead() {
    // 加载图片
    QPixmap originalPixmap(":/resources/images/head1.png");
    // 设置图片自动缩放
    qDebug() << originalPixmap.size() << ui->head_label->size();
    originalPixmap = originalPixmap.scaled(ui->head_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 创建一个和原始图片相同大小的QPixMap对象，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 使用QPainterPath绘制圆角
    QPainterPath path;
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10);
    painter.setClipPath(path);

    // 将原始图片绘制到圆角矩形中
    painter.drawPixmap(0, 0, originalPixmap);

    // 设置label的图片
    ui->head_label->setPixmap(roundedPixmap);
}

bool LoginDialog::checkEmailValid() {
    auto email = ui->email_edit->text();
    if (email.isEmpty()) {
        AddTipError(TipError::TIP_EMAIL_ERROR, tr("邮箱不能为空"));
        return false;
    }
    DelTipError(TipError::TIP_EMAIL_ERROR);
    return true;
}

void LoginDialog::slot_forget_pwd() {
    std::cout << "forget pwd" << std::endl;
    emit switchResetPwd();
}

bool LoginDialog::checkPwdValid() {
    auto pwd = ui->pass_edit->text();
    if (pwd.isEmpty()) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码不能为空"));
        return false;
    }
    if (pwd.length() < 6 || pwd.length() > 15) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码长度应在6-15位之间"));
        return false;
    }
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(pwd).hasMatch();
    if (!match) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码包含非法字符"));
        return false;
    }
    DelTipError(TipError::TIP_PWD_ERROR);
    return true;
}

void LoginDialog::AddTipError(TipError error, QString tips) {
    _tip_errors[error] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipError(TipError error) {
    _tip_errors.remove(error);
    if (_tip_errors.empty()) {
        ui->error_tip->clear();
        return;
    }
    showTip(_tip_errors.first(), false);
}

void LoginDialog::showTip(QString str, bool b_ok) {
    if (b_ok) {
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "error");
    }
    ui->error_tip->setText(str);
    repolish(ui->error_tip);
}

bool LoginDialog::enableBtn(bool cond) {
    ui->login_btn->setEnabled(cond);
    ui->reg_btn->setEnabled(cond);
    return true;
}

void LoginDialog::initHttpHandlers() {
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误！"), false);
            enableBtn(true);
            return;
        }
        auto email = jsonObj["email"].toString();

        ServerInfo serverInfo;
        serverInfo.Uid = jsonObj["uid"].toInt();
        serverInfo.Host = jsonObj["host"].toString();
        serverInfo.Port = jsonObj["port"].toString();
        serverInfo.Token = jsonObj["token"].toString();

        _uid = serverInfo.Uid;
        _token = serverInfo.Token;
        qDebug() << "email is " << email << " uid is " << _uid << " Host is " << serverInfo.Host << " Port is "
                << serverInfo.Port << " Token is " << _token;

        emit sig_connect_tcp(serverInfo);
    });
}

void LoginDialog::on_login_btn_clicked() {
    qDebug() << "login btn clicked";
    if (!checkEmailValid()) return;
    if (!checkPwdValid()) return;
    enableBtn(false);
    auto email = ui->email_edit->text();
    auto pwd = ui->pass_edit->text();
    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["passwd"] = md5Encrypt(pwd);
    HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_login"), json_obj, ReqId::ID_LOGIN_USER,
                                            Modules::LOGINMOD);
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes error) {
    if (error != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());

    if (jsonDoc.isNull()) {
        showTip(tr("JSON解析错误"), false);
        return;
    }

    if (!jsonDoc.isObject()) {
        showTip(tr("JSON对象解析错误"), false);
        return;
    }

    _handlers[id](jsonDoc.object());
}
