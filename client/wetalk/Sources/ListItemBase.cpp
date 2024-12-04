//
// Created by 29697 on 24-12-4.
//

#include "ListItemBase.h"

ListItemBase::ListItemBase(QWidget *parent) : QWidget(parent) {
}

void ListItemBase::SetItemType(ListItemType itemType) {
    _itemType = itemType;
}

ListItemType ListItemBase::GetItemType() {
    return _itemType;
}
