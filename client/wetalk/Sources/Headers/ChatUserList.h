//
// Created by 29697 on 24-12-4.
//

#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#include <QListWidget>
#include <qtmetamacros.h>


class ChatUserList : public QListWidget {
    Q_OBJECT

public:
    ChatUserList(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void sig_loading_chat_user();
};


#endif //CHATUSERLIST_H
