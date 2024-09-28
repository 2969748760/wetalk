/**
  ******************************************************************************
  * @File           : global.cpp
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/27
  ******************************************************************************
 */
#include "global.h"

std::function<void(QWidget *)> repolish = [](QWidget *widget) {
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
};