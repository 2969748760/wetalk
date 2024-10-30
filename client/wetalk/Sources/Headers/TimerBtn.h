//
// Created by 29697 on 24-10-30.
//

#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QTimer>
#include <QPushButton>

class TimerBtn : public QPushButton {
public:
    TimerBtn(QWidget *parent = nullptr);

    ~TimerBtn();

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QTimer *_timer;
    int _counter;
};


#endif //TIMERBTN_H
