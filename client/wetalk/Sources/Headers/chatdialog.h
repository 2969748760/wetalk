//
// Created by 29697 on 24-12-3.
//

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class ChatDialog;
}

QT_END_NAMESPACE

class ChatDialog : public QDialog {
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);

    ~ChatDialog() override;

    void addChatUserList();

private:
    void ShowSearch(bool bsearch = false);

    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;

private slots:
    void slot_loading_chat_user();
};


#endif //CHATDIALOG_H
