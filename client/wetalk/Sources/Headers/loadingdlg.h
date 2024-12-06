//
// Created by 29697 on 24-12-5.
//

#ifndef LOADINGDLG_H
#define LOADINGDLG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class LoadingDlg; }
QT_END_NAMESPACE

class LoadingDlg : public QDialog {
Q_OBJECT

public:
    explicit LoadingDlg(QWidget *parent = nullptr);
    ~LoadingDlg() override;

private:
    Ui::LoadingDlg *ui;
};


#endif //LOADINGDLG_H
