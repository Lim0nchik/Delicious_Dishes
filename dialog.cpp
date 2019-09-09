#include "dialog.h"
#include "ui_dialog.h"
#include <QPushButton>
#include <QMessageBox>
#include <string>

using namespace std;

Dialog::Dialog(QWidget *parent, string name_line, string price_line, string time_line,
               string calorie_line, string cook_text, string ingredient_text, bool enable, string type) : QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QRegExp reg_name ("[a-zA-Z ]{0,40}");
    QRegExp reg_price ("[1-9]{1}[0-9]{0,5}");
    QRegExp reg_calorie ("[1-9]{1}[0-9]{0,7}");
    QRegExp reg_time ("[1-9]{1}[0-9]{0,3}");

    QRegExp reg_ingredient ("[a-zA-Z ]{0,20}:[1-9]{1}[0-9]{0,4}");


    ui->name_edit->setValidator(new QRegExpValidator(reg_name, this));
    ui->price_edit->setValidator(new QRegExpValidator(reg_price, this));
    ui->calorie_edit->setValidator(new QRegExpValidator(reg_calorie, this));
    ui->time_edit->setValidator(new QRegExpValidator(reg_time, this));


    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(okClicked()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(close()));

    connect(ui->name_edit,SIGNAL(textChanged(QString)), this, SLOT(okEnabled()));
    connect(ui->price_edit,SIGNAL(textChanged(QString)), this, SLOT(okEnabled()));
    connect(ui->calorie_edit,SIGNAL(textChanged(QString)), this, SLOT(okEnabled()));
    connect(ui->time_edit, SIGNAL(textChange(Qstring)), this, SLOT(okEnabled()));

    ui->name_edit->setText(name_line.c_str());
    ui->price_edit->setText(price_line.c_str());
    ui->time_edit->setText(time_line.c_str());
    ui->calorie_edit->setText(calorie_line.c_str());
    ui->text_cook->setText(cook_text.c_str());
    ui->text_ingredient->setText(ingredient_text.c_str());


    int index = ui->dialog_box->findText(type.c_str());
    ui->dialog_box->setCurrentIndex(index);
    ui->dialog_box->setEnabled(enable);



}
void Dialog::okEnabled()
{



    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->name_edit->hasAcceptableInput() &&
                                                            ui->price_edit->hasAcceptableInput() &&
                                                            ui->calorie_edit->hasAcceptableInput()&&
                                                            ui->time_edit->hasAcceptableInput());
}
void Dialog::okClicked()
{

    emit file_dialog(ui->name_edit->text());

    vector <string> total;

    QString dialog_type = ui->dialog_box->currentText();
    QString dialog_name = ui->name_edit->text();
    QString dialog_price = ui->price_edit->text();
    QString dialog_time = ui->time_edit->text();
    QString dialog_calorie = ui->calorie_edit->text();

    total.insert(total.end(),{dialog_name.toStdString(), dialog_type.toStdString(),
                              dialog_price.toStdString(), dialog_time.toStdString(),
                              dialog_calorie.toStdString()});



    QString dialog_inredient = ui->text_ingredient->toPlainText();
    QString dialog_cook = ui->text_cook->toPlainText();

    QStringList dialog_inredient_list = dialog_inredient.split("\n");
    QStringList dialog_cook_list = dialog_cook.split("\n");

    vector <string> proverka;
    vector <string> vector_ingredient_count;
    vector <string> vector_ingredient;
    vector <string> vector_how_to_cook;


    for(auto element:dialog_inredient_list)
        if(element.indexOf(QRegExp("[a-zA-Z ]{0,20}[ ]{1}:[ ]{1}[1-9]{1}[0-9]{0,4}[a-z ]{0,6}")) == 0)
        {
            auto element_split = element.split(" : ");
            vector_ingredient.push_back(element_split[0].toStdString());
            vector_ingredient_count.push_back(element_split[1].toStdString());
        }

        else {
            proverka.push_back(element.toStdString());
        }

    for (auto element : dialog_cook_list)
        vector_how_to_cook.push_back(element.toStdString());

    if (!proverka.empty())
    {
        QString warning;
        for(vector<string>::iterator it = proverka.begin(); it != proverka.end(); ++it)
        {
            warning += it->c_str();
            warning += '\n';
        }
         QMessageBox::warning(this, "warning", "У вас ошибки в строчках:\n" + warning);
     }
    else {

        Storage::New_Dish NewDish;

        NewDish.dish_type = static_cast <Storage::DishType> (ui->dialog_box->currentIndex() + 1);
        NewDish.title = dialog_name.toStdString();
        NewDish.PrepearingTime = dialog_time.toInt();
        NewDish.average_cost_rub = dialog_price.toInt();
        NewDish.calorie_content_100gr = dialog_calorie.toInt();
        NewDish.ingredients = vector_ingredient;
        NewDish.ingredients_count = vector_ingredient_count;
        NewDish.HowToCook = vector_how_to_cook;

        //дальше *накосячил надо наверное написать функцию чтобы не загромождать проверками*

        bool success, success_1, success_2, success_3;
        BasicDishProperty *it, *it_1, *it_2, *it_3;
        tie(success, it) = storage.search_dish(dialog_name.toStdString(), storage.DessertDish);
        tie(success_1, it_1) = storage.search_dish(dialog_name.toStdString(), storage.FirstDish);
        tie(success_2, it_2) = storage.search_dish(dialog_name.toStdString(), storage.SecondDish);
        tie(success_3, it_3) = storage.search_dish(dialog_name.toStdString(), storage.SaladSnackDish);
        if (success || success_1 || success_2 || success_3)
        {

            if (success)
            {
                *it = NewDish;}
            if (success_1)
            {
                *it = NewDish;}
            if (success_2)
            {
                *it = NewDish;}
            if (success_3)
            {
                *it = NewDish;}

            QMessageBox::information(this, "Change", "Изменение прошло успешно");
            close();

        }
        else {


        storage.Add_NewDish(NewDish);

     QMessageBox::information(this, "Add", "Блюдо было добавлена");

     close();
        }
    }

}



Dialog::~Dialog()
{
    delete ui;
}
