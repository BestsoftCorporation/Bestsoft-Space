#include "mainwindow.h"
#include "codehelp.h"
#include "ui_codehelp.h"
#include <QtWidgets>
QString str;
QStringList hlp;
int i = -1;
int s;
codeHelp::codeHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::codeHelp)
{


    Qt::WindowFlags flags = this->windowFlags();
    ui->setupUi(this);
    this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    ui->plainTextEdit->hide();
    this->resize(221,161);


}

codeHelp::~codeHelp()
{
    delete ui;
}
void codeHelp::eror(const QString &err){
    ui->plainTextEdit->setPlainText("");
   ui->plainTextEdit->show();
   ui->plainTextEdit->textCursor().insertHtml(err);
   ui->plainTextEdit->resize(400,161);
   this->resize(400,161);

}

void codeHelp::setData(const QString &labelText,const QString &langu) {
    this->resize(221,161);
  ui->plainTextEdit->hide();
     QString fle;
    if (labelText!=" "&&labelText!=""){
       str=str+labelText;
     if (langu=="C"){
       fle=(source+"/src/C/C.hlp");
     }
     if (langu=="ASM"){
       fle=(source+"/src/ASM/ASM.hlp");
     }
     if (langu=="DASH"){
       fle=(source+"/src/dash/Dash.hlp");
     }
     QFile inputFile(fle);
       if (inputFile.open(QIODevice::ReadOnly))
       {

          QTextStream in(&inputFile);
              while (!in.atEnd())
              {
                i++;
                hlp<<in.readLine();


                if (hlp.at(i).mid(0,str.length()).toLower()==str.toLower()){
                    bool found = false;
                    for (int r = 0; r < ui->listWidget->count(); ++r) {
                        if (ui->listWidget->item(r)->data(Qt::DisplayRole) == hlp.at(i)) {
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        ui->listWidget->addItem(hlp.at(i));
                    }
                 }else{
                  close();
                }
              }
        }

       inputFile.close();
    }else{
        str="";
        i=-1;
        ui->listWidget->clear();
        hlp.clear();
    }

}

void codeHelp::on_listWidget_clicked(const QModelIndex &index)
{
    //mn->editor->insertPlainText("write(");
     emit mySignal(ui->listWidget->currentItem()->text().remove(str));
    //emit mySignal(ui->listWidget->currentItem()->text().mid(str.length(),hlp.at(i).length()));
    this->close();
}
