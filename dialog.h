#pragma once
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "dishes.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr, string name_line = "", string price_line = "", string time_line = "",
                    string calorie_line = "", string cook_text = "", string ingredient = "", bool enable = true, string type = "");
    ~Dialog();

private:
    Ui::Dialog *ui;

    Storage &storage = Storage::getinstance();

   // BasicDishProperty* Basic;


   //BasicDishProperty add_dish;


signals:
    void file_dialog(const QString &str);
private slots:

   void okClicked();
   void okEnabled();

};

#endif // DIALOG_H
