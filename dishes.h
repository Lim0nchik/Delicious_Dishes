//#pragma once
#ifndef DISHES_H
#define DISHES_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.h"

#include <stdio.h>
#include <vector>
using namespace std;


class BasicDishProperty;
class DessertDishProperty;
class FirstDishProperty;
class SecondDishProperty;
class SaladSnackDishProperty;


class Storage {

public:

    static Storage& getinstance()
    {
        static Storage theSingleInstance;
            return theSingleInstance;

    }

    vector <DessertDishProperty> DessertDish;
    vector <FirstDishProperty> FirstDish;
    vector <SecondDishProperty> SecondDish;
    vector <SaladSnackDishProperty> SaladSnackDish;

    Json::Value Recepies = JsonOpen();

    enum DishType { Auto = 0,
                    DishTypeDessert = 1,
                    DishTypeFirst = 2,
                    DishTypeSecond = 3,
                    DishTypeSaladSnack = 4
                  };

    struct FilterParams {
    DishType dish_type;

    pair <int, int> time;
    pair <int, int> cost;
    pair <int, int> calories;
    vector <string> ingredients;
    };

    typedef struct New_Dish{

        DishType dish_type;


        string title;
        int PrepearingTime;
        int average_cost_rub;
        int calorie_content_100gr;

        vector <string> ingredients;
        vector <string> ingredients_count;

        vector <string> HowToCook;

    } New_Dish;

    string getDisplayType(DishType dishType);

    vector <string> MadeAllIngredients();

    vector <BasicDishProperty*> Sorting( FilterParams FilterData);

    //vector <BasicDishProperty*> Write( FilterParams FilterData);

    void Add_NewDish (New_Dish new_dish);

    void JsonClose();

    template <typename T >
    tuple <bool, T*> search_dish(const string& name, vector <T> &vec) {
       auto it = find_if(vec.begin(), vec.end(), [&name](const T &x) {
                return x.title == name;
                                            });
        if (it != vec.end())
            {return {true, &*it};}
        else  {
            return {false, &*it};
        }
      }



private:

    Storage(){}
    Storage(const Storage& root) = delete;


    Json::Value JsonOpen ();

    void AllDishes();

    template <class IngD>
    void PushBackAllIng(vector <IngD> &Dishes, vector <string> &AllIng)
    {
        for (unsigned int i=0; i < Dishes.size(); i++)
        {
            unsigned int j=0;
            while (j < Dishes[i].ingredients.size()){
                 if  (find ( AllIng.begin(), AllIng.end(), Dishes[i].ingredients[j] ) == AllIng.end())
                    AllIng.push_back(Dishes[i].ingredients[j]);
                 j++;
            }
        }
    }


    vector<BasicDishProperty *> filterBySmth(const vector <BasicDishProperty *> &allDishes,
                                             int BasicDishProperty::* filterMember,
                                             int left, int right);

    template <class IngD>
    vector <BasicDishProperty*> getPtrsFromVec (vector <IngD> &Dishes){

        vector <BasicDishProperty*> PtrsVec;
        for (auto &currentDish : Dishes) {
        PtrsVec.push_back(&currentDish);
        }
    return  PtrsVec;
    }


    vector <BasicDishProperty*> getDishesByType(size_t dishType);

    vector <BasicDishProperty *> filterByCookingTime(const vector<BasicDishProperty *> allDishes,
                                                    int left, int right);

    vector <BasicDishProperty *> filterByAvgCost(const vector<BasicDishProperty *> allDishes,
                                                    int left, int right);

    vector <BasicDishProperty *> filterByCalories(const vector<BasicDishProperty *> allDishes,
                                                    int left, int right);
    vector <BasicDishProperty *> filterByIngredients( vector<BasicDishProperty *> allDishes, vector <string> ChosenIng);

    int ResearchIng(vector <string> &ChosenIng, vector <string> &DishIng );

    template <class IngD>
    void appendToJsonArray(Json::Value &value, vector <IngD> dishes) {
    for (size_t i = 0; i < dishes.size(); ++i)
        value.append(dishes[i].toJson());
    }

    /*vector <BasicDishProperty *> Add_Dessert(const vector<BasicDishProperty *> allDishes,
                                                    int left, int right);*/




};



class BasicDishProperty {

public:

    string title;
    int PrepearingTime;
    int average_cost_rub;
    int calorie_content_100gr;

    vector <string> ingredients;
    vector <string> ingredients_count;

    vector <string> HowToCook;


    BasicDishProperty ( const Json::Value &Dish);

    BasicDishProperty ( Storage::New_Dish new_dish);

    virtual ~BasicDishProperty() {}

    BasicDishProperty &operator=(const Json::Value &value);

    virtual Json::Value toJson() const;
};

class DessertDishProperty : public BasicDishProperty {
public:
    using BasicDishProperty::operator =;

    DessertDishProperty(const Json::Value &value): BasicDishProperty(value) {}

    DessertDishProperty(Storage::New_Dish new_dish) : BasicDishProperty(new_dish){}

    virtual Json::Value toJson() const override;
};

class FirstDishProperty : public BasicDishProperty {
public:
    using BasicDishProperty::operator =;

    FirstDishProperty(const Json::Value &value): BasicDishProperty(value) {}

    FirstDishProperty(Storage::New_Dish new_dish) : BasicDishProperty(new_dish){}

    virtual Json::Value toJson() const override;
};

class SecondDishProperty : public BasicDishProperty {
public:
    using BasicDishProperty::operator =;

    SecondDishProperty(const Json::Value &value): BasicDishProperty(value) {}

    SecondDishProperty(Storage::New_Dish new_dish) : BasicDishProperty(new_dish){}

    virtual Json::Value toJson() const override;
};

class SaladSnackDishProperty : public BasicDishProperty {
public:
    using BasicDishProperty::operator =;

    SaladSnackDishProperty(const Json::Value &value): BasicDishProperty(value) {}

    SaladSnackDishProperty(Storage::New_Dish new_dish) : BasicDishProperty(new_dish){}

    virtual Json::Value toJson() const override;
};



#endif // DISHES_H
