#ifndef WIDGET_SIMPLEDRIVERBACKUP_H
#define WIDGET_SIMPLEDRIVERBACKUP_H

#include <QWidget>
#include "aboutdialog.h"

namespace Ui {
class Widget_SimpleDriverBackup;
}

class Widget_SimpleDriverBackup : public QWidget
{
    Q_OBJECT

public:
    explicit Widget_SimpleDriverBackup(QWidget *parent = 0);
    ~Widget_SimpleDriverBackup();

private slots:
    void on_pushButton_destFolder_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_about_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::Widget_SimpleDriverBackup *ui;

    AboutDialog *aDialog;

};

#endif // WIDGET_SIMPLEDRIVERBACKUP_H
