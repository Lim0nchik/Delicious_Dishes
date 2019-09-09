#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"

#include <QString>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <string>
#include <utility>

#include <tuple>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->MainWindow::centralWidget->setMinimumSize(1000, 800);
    ui->centralWidget->setMaximumSize(1000, 800);
    ui->tree_view->setMinimumSize(300, 530);
    ui->edit_description->setMinimumSize(370, 530);

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    model_tree = new QStandardItemModel(0, 1, this);
    ui->tree_view->setModel(model_tree);

    model_tree->setHeaderData(0, Qt::Horizontal, "Name");

    ui->tree_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


    connect(ui->tree_view, SIGNAL(clicked(QModelIndex)), this, SLOT(on_clicked_name(QModelIndex)));


    vector <string> AllIngredients = storage.MadeAllIngredients();

    for (const auto &element: AllIngredients ){
        ui->box_ingredient_1->addItem(element.c_str()); // type можно добавить программно из файла типы*/
        ui->box_ingredient_2->addItem(element.c_str());
        ui->box_ingredient_3->addItem(element.c_str());
        ui->box_ingredient_4->addItem(element.c_str());
    }
}

MainWindow::~MainWindow()
{
//delete storage;
delete model_tree;
delete ui;
}


void MainWindow::on_push_sort_clicked()
{
    Storage::FilterParams filtered_data;

    QString combobox_type = ui->box_type->currentText();
    filtered_data.dish_type  = static_cast<Storage::DishType>(ui->box_type->currentIndex()) ;


    QString combobox_price = ui->box_price->currentText();
    switch (ui->box_price->currentIndex()) {
        case 0:
            filtered_data.cost = make_pair (0, 100000);
            break;
        case 1:
            filtered_data.cost = make_pair (0, 50);
            break;
        case 2:
            filtered_data.cost = make_pair (50, 100);
            break;
        case 3:
            filtered_data.cost = make_pair (100, 300);
            break;
        case 4:
            filtered_data.cost = make_pair (300, 500);
            break;
        case 5:
            filtered_data.cost = make_pair (500, 1000);
            break;
        case 6:
            filtered_data.cost = make_pair (1000, 5000);
            break;
        case 7:
            filtered_data.cost = make_pair (5000, 100000);
            break;
    }

    QString combobox_time = ui->box_time->currentText();

    switch (ui->box_time->currentIndex()) {
        case 0:
            filtered_data.time = make_pair (0, 100000);
            break;
        case 1:
            filtered_data.time = make_pair (0, 30);
            break;
        case 2:
            filtered_data.time = make_pair (30, 60);
            break;
        case 3:
            filtered_data.time = make_pair (60, 120);
            break;
        case 4:
            filtered_data.time = make_pair (120, 180);
            break;
        case 5:
            filtered_data.time = make_pair (180, 10000);
            break;
    }

    QString combobox_calorie = ui->box_calorie->currentText();

    filtered_data.calories = make_pair (0, 1000);


    QString combobox_ingred_1 = ui->box_ingredient_1->currentText();
    QString combobox_ingred_2 = ui->box_ingredient_2->currentText();
    QString combobox_ingred_3 = ui->box_ingredient_3->currentText();
    QString combobox_ingred_4 = ui->box_ingredient_4->currentText();


    if (combobox_ingred_1 != "Auto")
        filtered_data.ingredients.push_back(combobox_ingred_1.toStdString());
    if (combobox_ingred_2 != "Auto")
        filtered_data.ingredients.push_back(combobox_ingred_2.toStdString());

    if (combobox_ingred_3 != "Auto")
        filtered_data.ingredients.push_back(combobox_ingred_3.toStdString());

    if (combobox_ingred_4 != "Auto")
        filtered_data.ingredients.push_back(combobox_ingred_4.toStdString());


    QMessageBox::information(this, "Title", "Вы ввели type: " + combobox_type + ",\n price: " + combobox_price+ ",\n time:"
                             + combobox_time + ",\n calorie:" + combobox_calorie + ",\n ingredient_1:"+ combobox_ingred_1
                             + ",\n ingredient_2:" + combobox_ingred_2 + ",\n ingredient_3:" + combobox_ingred_3
                             + ",\n ingredient_4:" + combobox_ingred_4);




    vector <BasicDishProperty*> SuitableDishes = storage.Sorting(filtered_data);
    auto count = SuitableDishes.size();
    model_tree->setRowCount(static_cast<int>(count)); //хз выдавал предупреждение!!!
    int current_row = 0;
    QMap <int, QVariant> prop_map;
        for (const auto current_dish : SuitableDishes)
        {
            QString description;
            prop_map[Qt::EditRole] = QString(current_dish->title.c_str());
            prop_map[Qt::DisplayRole] = QString(current_dish->title.c_str());

            description += "Title : ";
            description += current_dish->title.c_str();
            description += '\n';

            description += "Cost (rub) : ";
            description += to_string(current_dish->average_cost_rub).c_str();
            description += '\n';

            description += "Prepearing time (min) : ";
            description += to_string(current_dish->PrepearingTime).c_str();
            description += '\n';

            description += "Calories on 100gr : ";
            description += to_string(current_dish->calorie_content_100gr).c_str();
            description += '\n';

            description += "\nIngredients :";
            description += '\n';


            for (size_t item = 0 ; item < current_dish->ingredients.size(); ++item)
            {

                description += (current_dish->ingredients[item]).c_str();
                description += " : ";
                description += (current_dish->ingredients_count[item]).c_str();
                description += '\n';

            }

            description += "\nHow to cook: ";
            description += '\n';

            for (const auto &element : current_dish->HowToCook)
            {

                description += element.c_str();
                description += '\n';

            }



            prop_map[Qt::UserRole] = description;
            auto idx = model_tree->index(current_row, 0);
            model_tree->setItemData(idx, prop_map);
            ++current_row;

        }
    }


void MainWindow::on_push_add_triggered()
{

    Dialog *wnd = new Dialog(this);

    wnd->show();
}

void MainWindow::on_clicked_name(const QModelIndex &index) {
    QModelIndex idx = model_tree->index(index.row(), 0);
    QString description = model_tree->data(idx, Qt::UserRole).toString();
    backup_name = (idx.model()->data(idx.model()->index(idx.row(),0)).toString()).toStdString();
     ui->edit_description->setText(description);
}

void MainWindow::on_push_save_triggered()
{

    storage.JsonClose();
    QMessageBox::information(this, "Save", "Saved successfully");
}


void MainWindow::parsing(BasicDishProperty *pars)
{
    name = pars->title;
    price = to_string(pars->average_cost_rub);
    time = to_string(pars->PrepearingTime);
    calorie = to_string(pars->calorie_content_100gr);
    for(const auto &element: pars->HowToCook){
        cook += element;
        cook +='\n';  }

    for (size_t item = 0 ; item < pars->ingredients.size(); ++item)
    {

        ingredient += (pars->ingredients[item]).c_str();
        ingredient += " : ";
        ingredient += (pars->ingredients_count[item]).c_str();
        ingredient += '\n';

    }


}


void MainWindow::change_pars()
{
    bool success;
    BasicDishProperty *it;

    tie(success, it) = storage.search_dish(backup_name,  storage.DessertDish );
    if (success)
       { type = "Dessert Dish";
        parsing(it);}

    tie(success, it) = storage.search_dish(backup_name,  storage.FirstDish );
    if (success) {
        type = "First Dish";
        parsing(it);}

    tie(success, it) = storage.search_dish(backup_name,  storage.SecondDish );
    if (success) {
        type = "Second Dish";
        parsing(it);}

    tie(success, it) = storage.search_dish(backup_name,  storage.SaladSnackDish );
    if (success) {
        type = "Salad&Snack";
        parsing(it);}


}


void MainWindow::on_push_change_triggered()
{
    change_pars();

    Dialog *wnd = new Dialog(this, name, price, time, calorie, cook, ingredient, false, type);

    wnd->show();

}
template <typename T >
void MainWindow::delete_dish(const string& name, vector <T> &vec) {
   auto it = find_if(vec.begin(), vec.end(), [&name](const T &x) {
            return x.title == name;
                                        });
    if (it != vec.end())
        vec.erase(it);
  }

void MainWindow::on_push_sdelete_triggered()
{

    delete_dish(backup_name,  storage.DessertDish );
    delete_dish(backup_name,  storage.FirstDish );
    delete_dish(backup_name,  storage.SecondDish );
    delete_dish(backup_name,  storage.SaladSnackDish );

}
