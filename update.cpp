#include "update.h"
#include "ui_update.h"
#include "qtdownload.h"
#include "httpdownload.h"
Update::Update(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
    HttpDownload w;
        w.setWindowTitle("Http Download");
        w.show();
}

Update::~Update()
{
    delete ui;
}
