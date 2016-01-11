#include "start.h"
#include "ui_start.h"
#include "mainwindow.h"
#include "httpdownload.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>

QByteArray datas;
 QNetworkReply *reply;
Start::Start(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(qApp->applicationDirPath()+"/src/icon.png"));
    ui->groupBox->setStyleSheet("background-color:white;");
    ui->label->setStyleSheet("QLabel {color : gray; }");
    ui->webView->setVisible(false);
    ui->pushButton_4->setVisible(false);



    QNetworkAccessManager *nam = new QNetworkAccessManager();
       QUrl url2("https://4f127c4feef351b26b75ad61883fdaec47fdc464.googledrive.com/host/0B8ftKunTL2iLeWo1NXE5LWJtLWM/updatenew.txt");
       QNetworkRequest req(url2);
       reply = nam->get(req);

       connect(reply, SIGNAL(finished()), this, SLOT(requestFinished()));

}

void Start::requestFinished()
{
   datas = reply->readAll();
   QString command(datas);

   QFile inputFile(qApp->applicationDirPath()+"/src/ver.ver");
   if (inputFile.open(QIODevice::ReadOnly))
   {
      QTextStream in(&inputFile);
      QString line;
      while (!in.atEnd())
      {
          line = in.readLine();

      }

      if(line.toDouble()<command.toDouble()){
          up = new HttpDownload();
          up->show();
      }
      inputFile.close();
   }

}
void Start::resizeEvent(QResizeEvent* event)
{

   QPixmap bkgnd(qApp->applicationDirPath()+"/src/bs_space_back.png");
   bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
   QPalette palette;
   palette.setBrush(QPalette::Background, bkgnd);
   this->setPalette(palette);
    ui->groupBox->move(this->rect().center() - ui->groupBox->rect().center());
    ui->webView->move(this->rect().center() - ui->webView->rect().center());
    ui->pushButton_4->move(ui->webView->x(),ui->webView->y());

}


Start::~Start()
{
    delete ui;
}

void Start::on_pushButton_clicked()
{
    nw=new New();
    nw->show();
    close();
}

void Start::on_pushButton_2_clicked()
{
   QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Project"), "", tr("Image Files (*.bss)"));
    QFile f(fileName);
    pro= fileName.section("/",-1,-1).remove(".bss");
    loc = fileName.section("/",0,-2)+"/";
    source = qApp->applicationDirPath();
    if (f.open(QIODevice::ReadOnly))
    {
       QTextStream in(&f);
       type = in.readLine();
    }
    f.close();

    qDebug() << type;
    mn=new MainWindow();
    mn->resize(1200, 840);
    mn->show();
    close();

}

void Start::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (ui->comboBox->currentText()=="Light"){
        theme="light";
    }else{
        theme="dark";
    }
}

void Start::on_pushButton_3_clicked()
{
    ui->webView->setVisible(true);
    ui->webView->load(QUrl("http://bestsoft.azurewebsites.net/developer/question.aspx"));
    ui->pushButton_4->setVisible(true);
}

void Start::on_pushButton_4_clicked()
{
    ui->webView->setVisible(false);
    ui->pushButton_4->setVisible(false);
}
