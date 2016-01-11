#include "new.h"
#include "ui_new.h"
#include "mainwindow.h"
QString dir;
//QString loc;
//QString source;

New::New(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(qApp->applicationDirPath()+"/src/icon.png"));
    ui->plainTextEdit_2->setPlainText(QDir::homePath()+"/");
    ui->plainTextEdit->setPlainText("OS");

    source=qApp->applicationDirPath();

}

New::~New()
{
    delete ui;
}



void New::on_pushButton_clicked()
{
    loc=ui->plainTextEdit_2->toPlainText()+ui->plainTextEdit->toPlainText()+"/";
    pro=ui->plainTextEdit->toPlainText();




    if(ui->listWidget->currentItem()!=0)
        {
    if(ui->listWidget->currentItem()->text() =="Assembly (FAT FileSystem, Bestsoft Bootloader)")
    {

        QDir dir(ui->plainTextEdit_2->toPlainText()+ui->plainTextEdit->toPlainText());

        if (!dir.exists()) {
            dir.mkpath(".");
        }
        type="ASM";
        QFile::copy(source+"/src/Bootloader/Kernel.asm",loc+"Kernel.asm");
        QFile::copy(source+"/src/Bootloader/Bootloader.asm",loc+"Boot.asm");
        QFile file(loc+pro+".bss");
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "ASM\nBoot\nKernel";
        }
        file.close();
        mn=new MainWindow();
        mn->resize(1200, 840);
        mn->show();
        close();


    }else{
        if(ui->listWidget->currentItem()->text() =="C (Unix-like Operating System, Grub Bootloader)")
        {
            type="C";
            QDir dir(ui->plainTextEdit_2->toPlainText()+ui->plainTextEdit->toPlainText());
            if (!dir.exists()) {
                dir.mkpath(".");
            }
            QDir recoredDir(source+"/src/C/project");
            QStringList allFiles = recoredDir.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

            for (int i=0;i<allFiles.length();i++){
              QFile::copy(source+"/src/C/project/"+allFiles.at(i),loc+allFiles.at(i));
            }

            QFile file(loc+pro+".bss");
            if (file.open(QIODevice::ReadWrite)) {
                QTextStream stream(&file);
                stream << "C\nBoot.s\nvsprintf\nheap\nkeyboard\ntimer\npmm\nvmm\nidt\nthread\nMonitor\nCommon\npanic\nlock\nprintk\nscheduler\ngdt\nMain\nelf\nthread_asm.s\ngdt_s.s\nidt_s.s";
            }
            file.close();
            mn=new MainWindow();
            mn->resize(1200, 840);
            mn->show();
            close();
        }else{
            if(ui->listWidget->currentItem()->text() =="Dash (FAT FileSystem, Bestsoft Bootloader)")
            {
                type="DASH";
                QDir dir(ui->plainTextEdit_2->toPlainText()+ui->plainTextEdit->toPlainText());
                if (!dir.exists()) {
                    dir.mkpath(".");
                }
                QFile::copy(source+"/src/dash/Kernel.dsh",loc+"Kernel.dsh");
                QFile::copy(source+"/src/dash/Boot.dsh",loc+"Boot.dsh");
                QFile file(loc+pro+".bss");
                if (file.open(QIODevice::ReadWrite)) {
                    QTextStream stream(&file);
                    stream << "DASH\nBoot\nKernel";
                }
                file.close();
                mn=new MainWindow();
                mn->resize(1200, 840);
                mn->show();
                close();
            }else{
                QMessageBox msgBox;
                msgBox.setText("You Have to select project language");
                msgBox.exec();

            }
        }
    }
   }else{
        QMessageBox msgBox;
        msgBox.setText("You Have to select project language");
        msgBox.exec();
    }


}





void New::on_pushButton_2_clicked()
{
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
     ui->plainTextEdit_2->setPlainText(dir+"/");


}



void New::on_plainTextEdit_textChanged()
{

}

void New::on_listWidget_currentTextChanged(const QString &currentText)
{

}

void New::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{

}
