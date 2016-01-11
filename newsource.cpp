#include "newsource.h"
#include "ui_newsource.h"
#include "mainwindow.h"
newsource::newsource(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newsource)
{
    ui->setupUi(this);
    if(type=="ASM"||type=="DASH"){
        ui->comboBox->setVisible(false);
    }
}

newsource::~newsource()
{
    delete ui;

}

void newsource::on_pushButton_clicked()
{
    if(type=="C"){
        if (ui->comboBox->currentIndex()==0)
        {
        QFile file(loc+ui->textEdit->toPlainText()+".c");

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "#include "<<"\""<<ui->textEdit->toPlainText()<<".h"<<"\""<<"\n\n void "<<ui->textEdit->toPlainText()<<"(){\n\n\nreturn 0;\n\n}" << endl;
        }
        file.close();

        file.setFileName(loc+ui->textEdit->toPlainText()+".h");
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "#ifndef  "<<ui->textEdit->toPlainText().toUpper()<<"\n#define "<<ui->textEdit->toPlainText().toUpper()<<"\n#include "<<"\""<<"bestsoft.h"<<"\""<<"\n#endif"<< endl;
        }
        file.close();
        QString line;
        QFile inputFile(loc+pro+".bss");
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           line = in.readLine();
           while (!in.atEnd())
           {
              line =line+"\n"+ in.readLine();

           }

           inputFile.close();

           line=line+"\n"+ui->textEdit->toPlainText();

        }
        QString filename = loc+pro+".bss";
           QFile file2(filename);
           if (file2.open(QIODevice::ReadWrite)) {
               QTextStream stream(&file2);
               stream << line;
           }
           file2.close();

     }
        if (ui->comboBox->currentIndex()==1)
        {
        QFile file(loc+ui->textEdit->toPlainText()+".s");

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "[global "<<ui->textEdit->toPlainText()<<"]"<<"\n"<<ui->textEdit->toPlainText()<<":\nret" << endl;
        }
        file.close();


        QString line;
        QFile inputFile(loc+pro+".bss");
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           line = in.readLine();
           while (!in.atEnd())
           {
              line =line+"\n"+ in.readLine();

           }

           inputFile.close();

           line=line+"\n"+ui->textEdit->toPlainText()+".s";

        }
        QString filename = loc+pro+".bss";
           QFile file2(filename);
           if (file2.open(QIODevice::ReadWrite)) {
               QTextStream stream(&file2);
               stream << line;
           }
           file2.close();

     }
    }
    if(type=="DASH"){
        ui->comboBox->setVisible(false);
        QFile file(loc+ui->textEdit->toPlainText()+".dsh");
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "Import Bestsoft.Dash\n\nClass "+ui->textEdit->toPlainText()+"\n     Start_Sub()\n     End_Start_Sub\nEnd_Class" << endl;
        }
        file.close();
        QString line;
        QFile inputFile(loc+pro+".bss");
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           line = in.readLine();
           while (!in.atEnd())
           {
              line =line+"\n"+ in.readLine();

           }

           inputFile.close();

           line=line+"\n"+ui->textEdit->toPlainText();

        }
        QString filename = loc+pro+".bss";
           QFile file2(filename);
           if (file2.open(QIODevice::ReadWrite)) {
               QTextStream stream(&file2);
               stream << line;
           }
           file2.close();

    }
    if(type=="ASM"){
        ui->comboBox->setVisible(false);
        QFile file(loc+ui->textEdit->toPlainText()+".asm");


        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "BITS 16\n[org 0]" << endl;
        }
        file.close();
        QString line;
        QFile inputFile(loc+pro+".bss");
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           line = in.readLine();
           while (!in.atEnd())
           {
              line =line+"\n"+ in.readLine();

           }

           inputFile.close();

           line=line+"\n"+ui->textEdit->toPlainText()+".s";

        }
        QString filename = loc+pro+".bss";
           QFile file2(filename);
           if (file2.open(QIODevice::ReadWrite)) {
               QTextStream stream(&file2);
               stream << line;
           }
           file2.close();


     }
    this->close();
}

