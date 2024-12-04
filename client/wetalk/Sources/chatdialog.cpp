//
// Created by 29697 on 24-12-3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatDialog.h" resolved

#include "chatdialog.h"
#include "Forms/ui_ChatDialog.h"
#include <QRandomGenerator>

#include "chatuserwid.h"


ChatDialog::ChatDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ChatDialog), _mode(ChatUIMode::ChatMode),
                                          _state(ChatUIMode::ChatMode), _b_loading(false) {
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");
    ui->search_edit->SetMaxLength(15);

    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/resources/images/search.png"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));

    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/resources/images/close_transparent.png"));
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);
    connect(ui->search_edit, &QLineEdit::textChanged, [clearAction](const QString &text) {
        if (!text.isEmpty()) {
            clearAction->setIcon(QIcon(":/resources/images/close_search.png"));
        } else {
            clearAction->setIcon((QIcon(":/resources/images/close_transparent.png")));
        }
    });

    connect(clearAction, &QAction::triggered, [this, clearAction]() {
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/resources/images/close_transparent.png"));
        ui->search_edit->clearFocus();
        ShowSearch(false);
    });

    ShowSearch(false);
    addChatUserList();
}

ChatDialog::~ChatDialog() {
    delete ui;
}

std::vector<QString> strs = {
    "hello world !",
    "nice to meet u",
    "New year，new life",
    "You have to love yourself",
    "My love is written in the wind ever since the whole world is you"
};

std::vector<QString> heads = {
    ":/resources/images/head1.png",
    ":/resources/images/head2.png",
    ":/resources/images/head3.png",
    ":/resources/images/head4.png",
    ":/resources/images/head5.png",
    ":/resources/images/head6.png"
};

std::vector<QString> names = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};

void ChatDialog::addChatUserList() {
    // 创建QListWidgetItem，并设置自定义的widget
    for (int i = 0; i < 13; i++) {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();
        auto *chat_user_wid = new ChatUserWid();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }
}

void ChatDialog::ShowSearch(bool bsearch) {
    if (bsearch) {
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    } else if (_state == ChatUIMode::ChatMode) {
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ContactMode;
    } else if (_state == ChatUIMode::ContactMode) {
        ui->chat_user_list->hide();
        ui->search_list->hide();
        ui->con_user_list->show();
        _mode = ChatUIMode::ContactMode;
    }
}
