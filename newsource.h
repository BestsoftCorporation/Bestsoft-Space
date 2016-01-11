#ifndef NEWSOURCE_H
#define NEWSOURCE_H

#include <QDialog>

namespace Ui {
class newsource;
}

class newsource : public QDialog
{
    Q_OBJECT

public:
    explicit newsource(QWidget *parent = 0);
    ~newsource();

private slots:
    void on_pushButton_clicked();

private:
    Ui::newsource *ui;
};

#endif // NEWSOURCE_H
