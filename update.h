#ifndef UPDATE_H
#define UPDATE_H

#include <QFrame>

namespace Ui {
class Update;
}

class Update : public QFrame
{
    Q_OBJECT

public:
    explicit Update(QWidget *parent = 0);
    ~Update();

private:
    Ui::Update *ui;
};

#endif // UPDATE_H
