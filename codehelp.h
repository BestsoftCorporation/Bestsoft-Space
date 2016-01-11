#ifndef CODEHELP_H
#define CODEHELP_H

#include <QDialog>

namespace Ui {
class codeHelp;
}

class codeHelp : public QDialog
{
    Q_OBJECT

public:
    explicit codeHelp(QWidget *parent = 0);
    ~codeHelp();
    void setData(const QString &labelText,const QString &langu);
    void eror(const QString &err);
signals:
    void mySignal(QString);

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

private:

    Ui::codeHelp *ui;
};

#endif // CODEHELP_H
