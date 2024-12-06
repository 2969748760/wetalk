//
// Created by 29697 on 24-12-5.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoadingDlg.h" resolved

#include "loadingdlg.h"
#include "Forms/ui_LoadingDlg.h"
#include <QMovie>

LoadingDlg::LoadingDlg(QWidget *parent) : QDialog(parent), ui(new Ui::LoadingDlg) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(parent->size());
    QMovie *movie = new QMovie(":/resources/images/loading.gif");
    ui->loading_lb->setMovie(movie);
    movie->start();
}

LoadingDlg::~LoadingDlg() {
    delete ui;
}
