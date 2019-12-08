#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>

#include <QModelIndex>

#include <input_dialog.h>
#include <dishes.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    template <typename T >
    void delete_dish(const string& name, vector <T> & vec);

    void change_pars();

    void parsing(BasicDishProperty *pars);



private slots:

    void on_push_sort_clicked();

    void on_push_add_triggered();

    void on_push_save_triggered();
    void click_name(const QModelIndex &index);




    void on_push_sdelete_triggered();

    void on_push_change_triggered();

private:
    Ui::MainWindow *ui;



    // Указатели на таблицы
    QStandardItemModel *model_tree;

    // Хранилище блюд
   Storage &storage = Storage::getinstance();

    // Заполняет ряд с номером row инфой о блюде dish
    void setDishRow(size_t row, size_t dishType,const BasicDishProperty &dish);

    string backup_name;

    string name;
    string price;
    string time;
    string calorie;
    string cook;
    string ingredient;
    string type;

};

#endif // MAINWINDOW_H
