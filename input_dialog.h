#ifndef INPUT_DIALOG_H
#define INPUT_DIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QValidator>
#include <QTableView>
#include <string>

using namespace std;


class input_dialog : public QDialog
{
    Q_OBJECT
public:

    input_dialog(QWidget *parent = nullptr);
    ~input_dialog();

    QLineEdit *name_line, *type_line, *calorie_line, *ingredient_line, *price_line, *cook_line, *needed_quantity_le;

  private slots:
    void on_ok_button_clicked();
    void on_changed();

  private:
    QLabel *name_label, *type_label, *calorie_label, *ingredient_label, *price_label, *cook_label;
    QPushButton *ok;
    QGridLayout *manager;
    QIntValidator i_validator;
};

#endif // INPUT_DIALOG_H
