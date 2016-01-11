#ifndef START_H
#define START_H

#include <QFrame>
#include "mainwindow.h"
#include "new.h"
#include <QtWidgets>
#include "httpdownload.h"
namespace Ui {
class Start;
}

class Start : public QFrame
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);

    ~Start();
    void resizeEvent(QResizeEvent* event);

   New *nw;
   MainWindow *mn;
   HttpDownload *up;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void requestFinished();

private:

    Ui::Start *ui;
};

#endif // START_H
