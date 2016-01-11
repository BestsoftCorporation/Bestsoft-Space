#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include <QtWidgets>
#include <QMainWindow>
#include <QThread>

#include "textedit.h"
class QTextEdit;
class QCompleter;
class QAbstractItemModel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);





protected:
     void closeEvent(QCloseEvent *event);
     void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    void debug();
    void dev();
    void about();
    void newFile();
    void undoo();
    void redoo();
    void build();
    void gotoline(int lineNumber);
    void writefile(QString Filename);
    void newClass();
    void onTextChanged();
    void oncodeEdited();

private slots:
      void addMyItems(QString);
private:
    void setupEditor();
    void proces(QString program);

    void setupFileMenu();
    QAbstractItemModel *modelFromFile(const QString& fileNa);

    QCompleter *completer;
    TextEdit* editor;
    Highlighter *highlighter;
    QTreeView *fileExlorer;
    QFileSystemModel *model;
    QTextEdit *output;


};

#endif // MAINWINDOW_H
