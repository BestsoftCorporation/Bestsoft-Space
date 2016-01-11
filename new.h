#ifndef NEW_H
#define NEW_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class New;
}

class New : public QDialog
{
    Q_OBJECT

public:
    explicit New(QWidget *parent = 0);
    ~New();
     MainWindow *mn;
private slots:


    void on_pushButton_clicked();



    void on_plainTextEdit_textChanged();

    void on_pushButton_2_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_currentTextChanged(const QString &currentText);

private:
    Ui::New *ui;
};

#endif // NEW_H
