//
// Created by 29697 on 24-12-6.
//

#ifndef CHATVIEW_H
#define CHATVIEW_H
#include <QVBoxLayout>
#include <QScrollArea>

class ChatView : public QWidget {
    Q_OBJECT

public:
    explicit ChatView(QWidget *parent = Q_NULLPTR);

    void appendChatItem(QWidget *item);

    void prependChatItem(QWidget *item);

    void insertChatItem(QWidget *before, QWidget *item);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout *m_pVl;
    QScrollArea *m_pScrollArea;
    bool isAppended;

    void initStyleSheet();

private slots:
    void onVScrollBarMoved(int min, int max);
};


#endif //CHATVIEW_H
