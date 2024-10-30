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

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog), _countdown(5) {
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirmpass_edit->setEchoMode(QLineEdit::Password);
    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);
    connect(HttpManager::GetInstance().get(), &HttpManager::sig_reg_mod_finish, this,
            &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
    ui->error_tip->clear();

    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this]() {
        checkUserValid();
    });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]() {
        checkEmailValid();
    });

    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this]() {
        checkPassValid();
    });

    connect(ui->confirmpass_edit, &QLineEdit::editingFinished, this, [this]() {
        checkConfirmPassValid();
    });

    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this]() {
        checkVerifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->SetState("invisible", "invisible_hover", "", "visible", "visible_hover", "");
    ui->confirm_visible->SetState("invisible", "invisible_hover", "", "visible", "visible_hover", "");

    connect(ui->pass_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->pass_visible->GetCurState();
        if (state == ClickLabelState::NORMAL) {
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "pass state is " << state;
    });

    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->GetCurState();
        if (state == ClickLabelState::NORMAL) {
            ui->confirmpass_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->confirmpass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "confirm state is " << state;
    });

    // 创建定时器
    _countdown_timer = new QTimer(this);

    // 连接信号和槽
    connect(_countdown_timer, &QTimer::timeout, this, [this]() {
        if (_countdown == 0) {
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功， %1 s后自动跳转到登录界面").arg(_countdown);
        ui->tip1_label->setText(str);
    });
}

RegisterDialog::~RegisterDialog() {
    qDebug() << "register dialog destruct";
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
        std::cout << "match successfully" << std::endl;
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verifycode"), json_obj,
                                                ReqId::ID_GET_VERIFY_CODE, Modules::REGISTERMOD);
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
    if (!jsonDoc.isObject()) {
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

    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObject) {
        int error = jsonObject["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = jsonObject["email"].toString();
        showTip(tr("用户注册成功 "), true);
        qDebug() << "email is " << email;
        qDebug() << "user is " << jsonObject["user"].toString();
        ChangeTipPage();
    });
}

bool RegisterDialog::checkUserValid() {
    if (ui->user_edit->text().isEmpty()) {
        AddTipError(TipError::TIP_USER_ERROR, tr("用户名不能为空！"));
        return false;
    }
    DelTipError(TipError::TIP_USER_ERROR);
    return true;
}

bool RegisterDialog::checkEmailValid() {
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if (!match) {
        //提示邮箱不正确
        AddTipError(TipError::TIP_EMAIL_ERROR, tr("邮箱地址不正确"));
        return false;
    }

    DelTipError(TipError::TIP_EMAIL_ERROR);
    return true;
}

bool RegisterDialog::checkPassValid() {
    auto pass = ui->pass_edit->text();

    if (pass.isEmpty()) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码不能为空"));
        return false;
    }

    if (pass.length() < 6 || pass.length() > 15) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码长度应在6-15位之间"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if (!match) {
        AddTipError(TipError::TIP_PWD_ERROR, tr("密码包含非法字符"));
        return false;
    }
    DelTipError(TipError::TIP_PWD_ERROR);
    return true;
}

bool RegisterDialog::checkConfirmPassValid() {
    auto con_pass = ui->confirmpass_edit->text();
    if (con_pass.isEmpty()) {
        AddTipError(TipError::TIP_CONFIRM_ERROR, tr("确认密码不能为空"));
        return false;
    }
    if (con_pass.length() < 6 || con_pass.length() > 15) {
        AddTipError(TipError::TIP_CONFIRM_ERROR, tr("密码长度应在6-15位之间"));
        return false;
    }
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(con_pass).hasMatch();
    if (!match) {
        AddTipError(TipError::TIP_CONFIRM_ERROR, tr("验证密码包含非法字符"));
        return false;
    }
    DelTipError(TipError::TIP_PWD_ERROR);
    if (con_pass != ui->pass_edit->text()) {
        AddTipError(TipError::TIP_PWD_CONFIRM, tr("两次密码不一致"));
        return false;
    } else {
        DelTipError(TipError::TIP_PWD_CONFIRM);
    }
    return true;
}

bool RegisterDialog::checkVerifyValid() {
    auto verify_code = ui->verify_edit->text();
    if (verify_code.isEmpty()) {
        AddTipError(TipError::TIP_VERIFY_ERROR, tr("验证码不能为空"));
        return false;
    }
    DelTipError(TipError::TIP_VERIFY_ERROR);
    return true;
}

void RegisterDialog::AddTipError(TipError error, QString tips) {
    _tip_errors[error] = tips;
    showTip(tips, false);
}

void RegisterDialog::DelTipError(TipError error) {
    _tip_errors.remove(error);
    if (_tip_errors.empty()) {
        ui->error_tip->clear();
        return;
    }
    showTip(_tip_errors.first(), false);
}

void RegisterDialog::ChangeTipPage() {
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    // 启动定时器, 设置间隔为1000ms（1s）
    _countdown_timer->start(1000);
}

void RegisterDialog::on_sure_btn_clicked() {
    bool valid = checkUserValid();
    if (!valid) return;
    valid = checkEmailValid();
    if (!valid) return;
    valid = checkPassValid();
    if (!valid) return;
    valid = checkConfirmPassValid();
    if (!valid) return;
    valid = checkVerifyValid();
    if (!valid) return;

    // 发送http请求
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = md5Encrypt(ui->pass_edit->text());
    json_obj["confirm"] = md5Encrypt(ui->confirmpass_edit->text());
    json_obj["verify_code"] = ui->verify_edit->text();
    HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
                                            json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}

void RegisterDialog::on_return_btn_clicked() {
    _countdown_timer->stop();
    emit sigSwitchLogin();
}
