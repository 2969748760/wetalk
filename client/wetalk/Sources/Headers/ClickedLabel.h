//
// Created by 29697 on 24-10-30.
//

#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H
#include <QLabel>
#include <QMouseEvent>
#include "global.h"


class ClickedLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickedLabel(QWidget *parent = nullptr);

    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void SetState(QString normal = "", QString hover = "", QString press = "", QString select = "",
                  QString select_hover = "", QString select_press = "");

    ClickLabelState GetCurState() const;

private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLabelState _curstate;

signals:
    void clicked(void);
};


#endif //CLICKEDLABEL_H
