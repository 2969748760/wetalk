//
// Created by 29697 on 24-12-4.
//

#ifndef LISTITEM_H
#define LISTITEM_H

#include "global.h"

class ListItemBase : public QWidget {
    Q_OBJECT

public:
    explicit ListItemBase(QWidget *parent = nullptr);

    void SetItemType(ListItemType itemType);

    ListItemType GetItemType();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    ListItemType _itemType;

public slots:
signals:
};


#endif //LISTITEM_H
