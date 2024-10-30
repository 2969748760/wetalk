//
// Created by 29697 on 24-10-30.
//

#include "Headers/TimerBtn.h"
#include <QMouseEvent>
#include <QDebug>

TimerBtn::TimerBtn(QWidget *parent) : QPushButton(parent), _counter(180) {
    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, [this]() {
        _counter--;
        if (_counter <= 0) {
            _timer->stop();
            _counter = 180;
            this->setText("获取验证码");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn() {
    _timer->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Button was released!";
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);
        emit clicked();
    }

    QPushButton::mouseReleaseEvent(event);
}
