/**
  ******************************************************************************
  * @File           : global.cpp
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/27
  ******************************************************************************
 */
#include "Headers/global.h"

std::function<void(QWidget *)> repolish = [](QWidget *widget) {
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
};

std::function<QString(QString)> xorString = [](QString input) {
    QString res = input;
    int len = input.length();
    len = len % 255;
    for (int i = 0; i < len; i++) {
        res[i] = QChar(static_cast<ushort>(input[i].unicode() ^ static_cast<ushort>(len)));
    }
    return res;
};

QString md5Encrypt(const QString &input) {
    QByteArray byteArray = input.toUtf8(); // 将输入字符串转换为字节数组
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5); // 使用 MD5 进行加密
    return QString(hash.toHex()); // 返回十六进制格式的加密结果
}

QString gate_url_prefix = "";
