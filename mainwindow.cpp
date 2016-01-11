#include "mainwindow.h"
#include <QtWidgets>
#include "ui_mainwindow.h"
#include "start.h"
#include "new.h"
#include "textedit.h"
#include <QString>
#include <QtDebug>
#include "newsource.h"
QStringList arguments;
QString pkgName;
QString SelectedFile="";
QStringList files;
QString def;
QString source;
QString pro;
QString loc;
QString type;
QString theme;
bool outp=false;
bool deb=false;
bool error=false;
QProgressBar *bar;

QColor color = QColor(230,230,230);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), completer(0)
{
    setupFileMenu();



    setupEditor();
    fileExlorer=new QTreeView;
    model=new QFileSystemModel;
    output = new QTextEdit();

    QDockWidget *dock = new QDockWidget("");
    QWidget* multiWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();
    QPushButton* filter1 = new QPushButton(QLatin1String("Build"));
    QPushButton* filter2 = new QPushButton(QLatin1String("Build and Start"));
    QPushButton* filter3 = new QPushButton(QLatin1String("New Class"));
    QPushButton* filter4 = new QPushButton(QLatin1String(""));
    QPushButton* filter5 = new QPushButton(QLatin1String(""));
    filter1->setMaximumWidth(60);
    filter2->setMaximumWidth(140);
    filter3->setMaximumWidth(80);
    filter4->setMaximumWidth(50);
    filter5->setMaximumWidth(50);
    filter1->setIcon(QIcon(qApp->applicationDirPath()+"/src/build.png"));
    filter2->setIcon(QIcon(qApp->applicationDirPath()+"/src/start-xxl.png"));
    filter4->setIcon(QIcon(qApp->applicationDirPath()+"/src/undo.png"));
    filter5->setIcon(QIcon(qApp->applicationDirPath()+"/src/redo.png"));

    layout->addWidget(filter1);
    layout->addWidget(filter2);
    layout->addWidget(filter3);
    layout->addWidget(filter4);
    layout->addWidget(filter5);
    multiWidget->setLayout(layout);
    dock->setWidget(multiWidget);
    addDockWidget(Qt::TopDockWidgetArea, dock);
    layout->addStretch(1);


    QGroupBox *gr = new QGroupBox(parent);
    gr->move(10, 10);



    QDockWidget *dockWidget2 = new QDockWidget("Output");

    addDockWidget(Qt::BottomDockWidgetArea, dockWidget2);
    dockWidget2->setWidget(output);
    QDockWidget *dockWidget3 = new QDockWidget("fileExlporer");
    QDockWidget *procesbar = new QDockWidget("Compiling Status");
    bar = new QProgressBar();
    bar->setMinimum(0);
    bar->setMaximum(100);
    procesbar->setWidget(bar);
    addDockWidget(Qt::RightDockWidgetArea, procesbar);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget3);

    //editor color
    if (theme=="dark"){
      QPalette p = editor->palette();
      p.setColor(QPalette::Base, QColor(31, 31, 31));
      editor->setPalette(p);
      editor->setTextColor(Qt::white);
      color=QColor(61,61,61);

      output->setPalette(p);
      output->setTextColor(Qt::white);
      dockWidget2->setStyleSheet("background:QColor(31,31,31);");
      multiWidget->setStyleSheet("background:QColor(31,31,31);");
      filter1->setStyleSheet("color:white;");
      filter2->setStyleSheet("color:white;");
      filter3->setStyleSheet("color:white;");
      filter4->setStyleSheet("color:white;");
      filter5->setStyleSheet("color:white;");


      fileExlorer->setStyleSheet("color: white;background:QColor(31,31,31);");
    }

       completer = new QCompleter(this);
       completer->setModel(modelFromFile(source+"/src/C/C.hlp"));
       completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
       completer->setCaseSensitivity(Qt::CaseInsensitive);
       completer->setWrapAround(false);
       editor->setCompleter(completer);
       if(type=="C"){
           editor->setPlainText("Welcome to Operating System Example in C Programming Language\nStart by selecting Main.c\n from File Explorer on the right\n===========================>");
       }
       if(type=="ASM"){
           editor->setPlainText("Welcome to Operating System Example in Assembly Programming Language(NASM Compiler)\nStart by selecting Kernel.asm\n from File Explorer on the right\n===========================>");
       }
       if(type=="DASH"){
           editor->setPlainText("Welcome to Operating System Example in Dash Programming Language\nStart by selecting Kernel.dsh\n from File Explorer on the right\n===========================>");
       }



    setWindowIcon(QIcon(qApp->applicationDirPath()+"/src/icon.png"));

    dockWidget3->setWidget(fileExlorer);
    setCentralWidget(editor);

    fileExlorer->setModel(model);
    fileExlorer->setRootIndex(model->setRootPath(loc));
    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(onTextChanged()));

    connect(editor, SIGNAL(textChanged()), this, SLOT(oncodeEdited()));
    connect(fileExlorer,SIGNAL(clicked(QModelIndex)),this,SLOT(about()));

    setWindowTitle(tr("Bestsoft Space"));
    connect(filter1, SIGNAL(clicked()),this,SLOT(build()));
    connect(filter2,SIGNAL(clicked()),this,SLOT(debug()));
    connect(filter3,SIGNAL(clicked()),this,SLOT(newClass()));
    connect(filter4,SIGNAL(clicked()),this,SLOT(undoo()));
    connect(filter5,SIGNAL(clicked()),this,SLOT(redoo()));

}


QAbstractItemModel *MainWindow::modelFromFile(const QString& fileNa)
{


    QFile file(fileNa);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}


void MainWindow::about()
{


    if (SelectedFile!="")
    {
        QDir destination(loc+pkgName);
        destination.remove(loc+pkgName);
        writefile(loc+pkgName);
    }
    pkgName = (model->fileName(fileExlorer->currentIndex()));
    SelectedFile=pkgName;
    QFile inputFile(loc+pkgName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       editor->setPlainText(in.readAll());
       inputFile.close();
    }

}

void MainWindow::newFile()
{
    Start *st = new Start();
    st->show();
}

void MainWindow::newClass()
{
     newsource *sr = new newsource();
     sr->show();
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new TextEdit;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document(),type);


    QFile file("mainwindow.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);

    fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);

    QMenu *compile = new QMenu(tr("&Compile"), this);
    menuBar()->addMenu(compile);

    compile->addAction(tr("&Build"), this, SLOT(build()));
    compile->addAction(tr("Build & Debug"), this, SLOT(debug()));

    QMenu *edit = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(edit);

    edit->addAction(tr("&Undo"), this, SLOT(undoo()));
    edit->addAction(tr("Redo"), this, SLOT(redoo()));

    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("Bestsoft Developer"), this, SLOT(dev()));
}
void MainWindow::dev(){
    QString link = "http://bestsoft.azurewebsites.net/developer";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::undoo()
{
editor->document()->undo();
}
void MainWindow::redoo()
{
 editor->document()->redo();
}
void MainWindow::build()
{


    outp=true;
       //cd->close();
       QString password;
       bool ok;
          QString text = QInputDialog::getText(this, tr("Authorisation Password"),
                                               tr("Authorisation Password:"), QLineEdit::Password,
                                               "", &ok);
          if (ok && !text.isEmpty()){
              password=text;
          }
          output->setText("");
          int i=-1;
          QDir destination;
          destination.remove(loc+pro+".iso");
          destination.remove(loc+pro+".flp");
          if (SelectedFile!="")
          {
             destination.remove(loc+SelectedFile);
             writefile(loc+SelectedFile);

          }
          QFile inputFile(loc+pro+".bss");
          if (inputFile.open(QIODevice::ReadOnly))
          {
             QTextStream in(&inputFile);
             QString line = in.readLine();
             if (line=="ASM"||line=="DASH")
             {
                 bar->setValue(10);

                 while (!in.atEnd())
                 {
                   i++;
                   files<<in.readLine();
                 }
                 if(line=="DASH")
                 {
                     for (int p=0;p<=i;p++)
                     {
                         arguments <<loc+files.at(p)+".dsh"<<loc+files.at(p)+".asm"<<source+"/src/dash";
                         proces(source+"/src/dash/Dash");
                     }

                 }
                for (int p=0;p<=i;p++)
                {

                    arguments <<"-f"<<"bin"<<"-o"<<loc+files.at(p)+".bin"<<loc+files.at(p)+".asm";
                    outp=true;
                    proces(source+"/src/ASM/nasm");
                    if(line=="DASH")
                    {
                      destination.remove(loc+files.at(p)+".asm");
                    }

                }

                 arguments<<"-C"<<loc+pro+".flp"<<"1440";
                 proces("/sbin/mkdosfs");
                 arguments<<"status=noxfer"<<"conv=notrunc"<<"if="+loc+files.at(0)+".bin"<<"of="+loc+pro+".flp";
                 proces("/bin/dd");
                 QString bld = "";
                  for (int p = 1;p<=i;p++){
                   bld+=" && cp "+files.at(p)+".bin tmp-loop/ ";
                  }
                 QString filename = loc+"/build.sh";
                     QFile file(filename);
                     if (file.open(QIODevice::ReadWrite)) {
                         QTextStream stream(&file);
                         stream << "cd "+loc +"\nmkdir tmp-loop && mount -o loop -t vfat "+pro+".flp tmp-loop && cp Kernel.bin tmp-loop/"+bld+"\nsleep 2.0\numount tmp-loop\nrm -rf tmp-loop\n'"+source+"/src/mkisofs/mkisofs' -quiet -V '"+pro+"' -input-charset iso8859-1 -o " + "'"+loc+"/"+pro+".iso' -b "+ pro+".flp "+loc << endl;
                     }

                 arguments<<"-c"<<"echo "+password+" | sudo -S /bin/sh "+loc+"/build.sh";
                 proces("/bin/bash");
                 bar->setValue(50);

                 bar->setValue(100);
                 if (deb){
                     QFile db("/usr/bin/qemu-system-x86_64");
                     if (db.exists()){
                     arguments<<"-cdrom"<<loc+pro+".iso";
                     proces("/usr/bin/qemu-system-x86_64");
                     deb=false;
                    }else{
                         QMessageBox msgBox;
                         msgBox.setText("For Debugging you need to install QEMU in (/usr/bin/)\nUbuntu: sudo apt-get install qemu\nFedora:yum install qemu\nDebian:sudo apt-get install qemu\nSuse: yast -i qemu\nArch:sudo pacman -S qemu\nGentoo: emerge -av app-emulation/qemu");
                         msgBox.exec();
                    }
                 }

             }
             QStringList toCompile;
             toCompile.clear();

             if (line=="C")
             {
                  bar->setValue(10);
                 destination.remove(loc+pro+".iso");

                 while (!in.atEnd())
                 {
                   i++;
                   files<<in.readLine();
                 }
                 arguments <<"-f"<<"elf"<<"-o"<<loc+files.at(0)+".o"<<loc+files.at(0)+".s";
                 outp=true;
                 proces(source+"/src/ASM/nasm");

                 QString bld = "";
                 for (int p=0;p<=i;p++){
                     if (files.at(p).contains(".s")){
                         QString fnam = files.at(p);
                         fnam=fnam.simplified().replace(".s","");
                         bld+=" "+fnam+".o";
                     }else{
                         bld+=" "+files.at(p)+".o";
                     }
                  }
                  QString filename = loc+"/makefile";
                  QFile db(filename);
                     if (db.open(QIODevice::ReadWrite)) {
                         QTextStream stream(&db);
                         stream << "SOURCES="+bld+" \nCFLAGS=-nostdlib -fno-builtin -m32 \nLDFLAGS=-melf_i386 -T '"+source+"/src/C/link.ld'\nASFLAGS=-felf\nall: $(SOURCES) link\nclean:\n	-rm *.o kernel\nlink:\n		ld $(LDFLAGS) -o kernel $(SOURCES)\n.s.o:\n		'"+source+"/src/ASM/nasm' $(ASFLAGS) $<"<< endl;
                     }
                  db.close();
                  db.setFileName("/usr/bin/xorriso");
                  if (db.exists()){
                     db.setFileName(loc+"/build.sh");
                     if (db.open(QIODevice::ReadWrite)) {
                         QTextStream stream(&db);
                         stream << "#/bin/sh\ncd '"+loc+"'\nmake -f makefile \nmkdir -p isodir\nmkdir -p isodir/boot\ncp kernel isodir/boot/kernel\nmkdir -p isodir/boot/grub\ncp grub.cfg isodir/boot/grub/grub.cfg\ngrub-mkrescue -o "+pro+".iso  isodir"<< endl;
                     }
                     db.close();
                     db.setFileName(loc+"/grub.cfg");
                     if (db.open(QIODevice::ReadWrite)) {
                         QTextStream stream(&db);
                         stream << "menuentry "+pro+" {\nmultiboot /boot/kernel\n}"<< endl;
                     }
                     db.close();
                     outp=true;
                     if (error){
                         error=false;
                         return;
                     }
                     arguments << "-c"<<"echo "+password+" | sudo -S /bin/sh "+loc+"/build.sh";
                     proces("/bin/bash");

                  }else{
                      QMessageBox msgBox;
                      msgBox.setText("For Creating ISO Image of your OS you need to install XORRISO in (/usr/bin/)\nUbuntu: sudo apt-get install xorriso\nFedora:yum install xorriso\nDebian:sudo apt-get install xorriso\nSuse: yast -i xorriso\nArch:sudo pacman -S xorriso");
                      msgBox.exec();
                  }


                  bar->setValue(50);
                 bar->setValue(100);
                   if (deb){
                       QFile db("/usr/bin/qemu-system-x86_64");
                       if (db.exists()){
                       arguments<<"-cdrom"<<loc+pro+".iso";
                       proces("/usr/bin/qemu-system-x86_64");
                       deb=false;
                      }else{
                           QMessageBox msgBox;
                           msgBox.setText("For Debugging you need to install QEMU in (/usr/bin/)\nUbuntu: sudo apt-get install qemu\nFedora:yum install qemu\nDebian:sudo apt-get install qemu\nSuse: yast -i qemu\nArch:sudo pacman -S qemu\nGentoo: emerge -av app-emulation/qemu");
                           msgBox.exec();
                      }
                   }
                 }



             inputFile.close();
          }

}

void MainWindow::debug()
{

    deb=true;
    build();
}



void MainWindow::proces(QString program)
{

    QProcess *process = new QProcess();
    if (def!=0)
    {
      process->setWorkingDirectory(def);
    }
    process->start(program,arguments);
    process->waitForFinished();
    if (outp){
        output->moveCursor(QTextCursor::End);
        QTextStream rsyncStdoutStream(process->readAllStandardError());
        QString all;
        while (true)
        {

            QString line = rsyncStdoutStream.readLine();

            if (line.isNull())
                break;
            else{

                if (line.contains("error")){
                 error=true;
                QString digit;
                digit="";
                int st=0;
                    for (int var = 0; var < line.length(); ++var) {
                        bool ok;
                        if (st==2){
                            break;
                        }
                        if (line.at(var)==':'){
                            st++;
                        }
                        if (line.at(var).isDigit()){
                            digit = digit+ line.at(var);
                        }

                    }

                   if (line.contains(SelectedFile)){
                       color=QColor(112,0,0);
                       onTextChanged();
                       int lnnmb = digit.toInt();
                       gotoline(lnnmb-1);
                       all=all+"<br><p style=\"color:red\"><img src=\"file:/"+source+"/src/error.png\" style=\"width:16px;height:16px\">"+line.remove(loc+SelectedFile)+"</p>";

                   }

                   output->textCursor().insertHtml("<br><p style=\"color:red\"><img src=\"file:/"+source+"/src/error.png\" style=\"width:16px;height:16px\">"+line+"</p>");


                  return;
                }

                if (line.contains("warning")){
                    if (theme=="dark"){
                       output->textCursor().insertHtml("<br><p style=\"color:white\"><img src=\"file:/"+source+"/src/warning.png\" style=\"width:16px;height:16px\">"+line+"</p>");
                    }else{
                        output->textCursor().insertHtml("<br><p style=\"color:black\"><img src=\"file:/"+source+"/src/warning.png\" style=\"width:16px;height:16px\">"+line+"</p>");
                    }

                }
           }
       }

        //output->textCursor().insertHtml("<br>"+process->readAllStandardError().replace("\n","<br>") +"<br>"+ process->readAllStandardOutput().replace("\n","<br>"));
    }
    outp=false;
    process->close();
    arguments.clear();
}
void MainWindow::writefile(QString Filename)
{
    QFile file(Filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << editor->toPlainText() << endl;
    }
    file.close();
}
void MainWindow::oncodeEdited()
{

}

void MainWindow::gotoline(int lineNumber)
{
    if (error){
         color=QColor(112,0,0);
    }
    const QTextBlock &block = editor->document()->findBlockByNumber(lineNumber);
    QTextCursor cursor(block);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 0);
    editor->setTextCursor(cursor);

    editor->setFocus();



}
void MainWindow::onTextChanged()
{

    QTextEdit::ExtraSelection highlight;

    highlight.cursor = editor->textCursor();
    highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
    highlight.format.setBackground(color);

    QList<QTextEdit::ExtraSelection> extras;
    extras << highlight;
    editor->setExtraSelections(extras);

    int i = editor->document()->blockCount();
    QString lines="";
    for (int p=0;p<i;p++){
        QString s = QString::number(p);
        lines=lines+"<br>"+s;
    }
    if (theme=="dark"){
      color = QColor(61,61,61);
    }else{
      color = QColor(230,230,230);
    }

}
void MainWindow::addMyItems(QString name)
{
  editor->insertPlainText(name);
}
void MainWindow::closeEvent(QCloseEvent *event) {

}

void MainWindow::keyPressEvent ( QKeyEvent * event )
{


}
