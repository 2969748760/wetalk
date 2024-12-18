//
// Created by 29697 on 24-12-4.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatUserWid.h" resolved

#include "chatuserwid.h"
#include "Forms/ui_ChatUserWid.h"


ChatUserWid::ChatUserWid(QWidget *parent) : ListItemBase(parent), ui(new Ui::ChatUserWid) {
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatUserWid::~ChatUserWid() {
    delete ui;
}


QSize ChatUserWid::sizeHint() const {
    return QSize(250, 70);
}

void ChatUserWid::SetInfo(QString name, QString head, QString msg) {
    _name = name;
    _head = head;
    _msg = msg;

    QPixmap pixmap(_head);

    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(_name);
    ui->user_chat_lb->setText(_msg);
}
