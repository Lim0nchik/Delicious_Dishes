#include "input_dialog.h"

input_dialog::input_dialog(QWidget *parent) : QDialog(parent), i_validator(1, 10000, this) {
    ok = new QPushButton("ok", this);

    name_label = new QLabel("Name", this);
    type_label = new QLabel("Type", this);
    calorie_label = new QLabel("Сalorie", this);
    ingredient_label = new QLabel("Ingredient", this);
    price_label = new QLabel("Price", this);
    cook_label = new QLabel("How to cook", this);

    name_line = new QLineEdit(this);
    type_line = new QLineEdit(this);
    calorie_line = new QLineEdit(this);
    ingredient_line = new QLineEdit(this);
    price_line = new QLineEdit(this);
    cook_line = new QLineEdit(this);

    manager = new QGridLayout(this);

    manager->addWidget(name_label, 0, 0);
    manager->addWidget(type_label, 0, 1);
    manager->addWidget(calorie_label, 0, 2);
    manager->addWidget(ingredient_label, 0, 3);
    manager->addWidget(price_label, 0, 4);
    manager->addWidget(cook_label, 0, 5);

    manager->addWidget(name_line, 1, 0);
    manager->addWidget(type_line, 1, 1);
    manager->addWidget(calorie_line, 1, 2);
    manager->addWidget(ingredient_line, 1, 3);
    manager->addWidget(price_line, 1, 4);
    manager->addWidget(cook_line, 1, 5);


    price_line->setValidator(&i_validator);
    //QRegExp price("[0-9](1)");


    connect(cook_line, SIGNAL(textChanged(QString)), this, SLOT(on_changed()));


    manager->addWidget(ok, 2, 6);
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));


}

void input_dialog::on_ok_button_clicked() {

}

void input_dialog::on_changed() {

}

input_dialog::~input_dialog() {
    delete ok;
}
