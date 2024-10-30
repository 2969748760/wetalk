//
// Created by 29697 on 24-10-30.
//

#include "Headers/ClickedLabel.h"

ClickedLabel::ClickedLabel(QWidget *parent) : QLabel(parent), _curstate(ClickLabelState::NORMAL) {
}

void ClickedLabel::enterEvent(QEnterEvent *event) {
    if(_curstate == ClickLabelState::NORMAL) {
        qDebug() << "enter, change to normal hover: " << _normal_hover;
        setProperty("state", _normal_hover);
        repolish(this);
        update();
    } else {
        qDebug() << "enter, change to selected hover: " << _selected_hover;
        setProperty("state", _selected_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent *event) {
    if(_curstate == ClickLabelState::NORMAL) {
        qDebug() << "leave, change to normal: " << _normal;
        setProperty("state", _normal);
        repolish(this);
        update();
    } else {
        qDebug() << "leave, change to selected: " << _selected;
        setProperty("state", _selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickedLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (_curstate == ClickLabelState::NORMAL) {
            qDebug() << "clicked, change to selected hover: " << _selected_hover;
            _curstate = ClickLabelState::SELECTED;
            setProperty("state", _selected_hover);
            repolish(this);
            update();
        } else {
            qDebug() << "clicked, change to normal hover: " << _normal_hover;
            _curstate = ClickLabelState::NORMAL;
            setProperty("state", _normal_hover);
            repolish(this);
            update();
        }
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}

void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover,
                            QString select_press) {
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state", normal);
    repolish(this);
}

ClickLabelState ClickedLabel::GetCurState() const {
    return _curstate;
}
