//
// Created by 29697 on 24-12-4.
//

#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H
#include <QLineEdit>


class CustomizeEdit : public QLineEdit {
    Q_OBJECT

public:
    CustomizeEdit(QWidget *parent = nullptr);

    void SetMaxLength(int maxLen);

protected:
    void focusOutEvent(QFocusEvent *event) override;

private:
    void limitTextLength(QString text);

    int _max_len;

    signals:
    void sig_focus_out();
};


#endif //CUSTOMIZEEDIT_H
