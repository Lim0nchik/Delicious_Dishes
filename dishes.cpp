#include "dishes.h"




    BasicDishProperty::BasicDishProperty ( const Json::Value &Dish)
    {
        title = Dish["info"]["title"].asString();
        PrepearingTime = Dish["info"]["time_min"].asInt();
        average_cost_rub = Dish["info"]["average_cost_rub"].asInt();
        calorie_content_100gr = Dish["info"]["calorie_content_100gr"].asInt();

        for (unsigned int i=0; i < Dish["ingredients"].size(); i++)
        {
            ingredients.push_back(Dish["ingredients"][i]["Name"].asString());
            ingredients_count.push_back(Dish["ingredients"][i]["count"].asString());
        }

        for (unsigned int i=0; i < Dish["howtocook"].size(); i++)
        {
            HowToCook.push_back(Dish["howtocook"][i].asString());
        }

    }

    BasicDishProperty::BasicDishProperty (Storage::New_Dish new_dish)
    {
        title = new_dish.title;
        PrepearingTime = new_dish.PrepearingTime;
        average_cost_rub = new_dish.average_cost_rub;
        calorie_content_100gr = new_dish.calorie_content_100gr;
        ingredients = new_dish.ingredients;
        ingredients_count = new_dish.ingredients_count;
        HowToCook = new_dish.HowToCook;

    }

    BasicDishProperty& BasicDishProperty::operator=(const Json::Value &value) {
    *this = BasicDishProperty (value);
    return *this;
    }

    Json::Value BasicDishProperty::toJson() const {
        Json::Value val(Json::objectValue);
        val["info"] = Json::Value(Json::objectValue);
        val["info"]["title"] = title;
        val["info"]["type"] = "BaseDish";
        val["info"]["time_min"] = PrepearingTime;
        val["info"]["average_cost_rub"] = average_cost_rub;
        val["info"]["calorie_content_100gr"] = calorie_content_100gr;


        val["ingredients"] = Json::Value(Json::arrayValue);
        for (size_t i = 0; i < ingredients.size(); ++i) {
            auto &ing = val["ingredients"].append(Json::Value(Json::objectValue));
            ing["Name"] = ingredients[i];
            ing["count"] = ingredients_count[i];
        }

        val["howtocook"] = Json::Value(Json::arrayValue);
        for (size_t i = 0; i < HowToCook.size(); ++i) {
            val["howtocook"].append(HowToCook[i]);
        }

    return val;
    }

    Json::Value DessertDishProperty::toJson() const {
        auto val = BasicDishProperty::toJson();
        val["info"]["type"] = "Dessert";
        return val;
    }

    Json::Value FirstDishProperty::toJson() const {
        auto val = BasicDishProperty::toJson();
        val["info"]["type"] = "First";
        return val;
    }

    Json::Value SecondDishProperty::toJson() const {
        auto val = BasicDishProperty::toJson();
        val["info"]["type"] = "Second";
        return val;
    }

    Json::Value SaladSnackDishProperty::toJson() const {
        auto val = BasicDishProperty::toJson();
        val["info"]["type"] = "SaladSnack";
        return val;
    }


    string Storage::getDisplayType(DishType dishType)
    {
        switch (dishType) {
            case DishTypeDessert:
                return "Dessert";
            case DishTypeFirst:
                return "First";
            case DishTypeSecond:
                return "Second";
            case DishTypeSaladSnack:
                return "SaladSnack";
            case Auto:
                return "Auto";
        }
    }

    vector <string> Storage::MadeAllIngredients()
    {
        vector <string> AllIng;
        AllDishes();

        PushBackAllIng(DessertDish, AllIng);
        PushBackAllIng(FirstDish, AllIng);
        PushBackAllIng(SecondDish, AllIng);
        PushBackAllIng(SaladSnackDish, AllIng);

        return AllIng;
    }

    vector <BasicDishProperty*> Storage::Sorting( FilterParams FilterData)
    {
        vector <BasicDishProperty*> SuitableData;
        SuitableData = getDishesByType(FilterData.dish_type);
        SuitableData = filterByAvgCost(SuitableData, FilterData.cost.first, FilterData.cost.second);
        SuitableData = filterByCookingTime(SuitableData, FilterData.time.first, FilterData.cost.second) ;
        SuitableData = filterByCalories(SuitableData, FilterData.calories.first, FilterData.calories.second);
        SuitableData = filterByIngredients(SuitableData, FilterData.ingredients);

        return SuitableData;
    }


    void Storage::Add_NewDish(New_Dish new_dish)
    {
        switch (new_dish.dish_type) {

            case DishTypeDessert:
                return DessertDish.push_back(new_dish);
            case DishTypeFirst:
                return FirstDish.push_back(new_dish);
            case DishTypeSecond:
                return SecondDish.push_back(new_dish);
            case DishTypeSaladSnack:
                return SaladSnackDish.push_back(new_dish);
        }

    }


    void Storage::JsonClose()
    {
        Json::Value New_rep;
        appendToJsonArray(New_rep, DessertDish);
        appendToJsonArray(New_rep, FirstDish);
        appendToJsonArray(New_rep, SecondDish);
        appendToJsonArray(New_rep, SaladSnackDish);

        ofstream Recep_2("Recepies.json");
        Recep_2 << New_rep;
    }


    Json::Value Storage::JsonOpen ()
    {
        ifstream Recep("rep.json");
        Json::Reader reader;
        Json::Value Recepies_1;
        reader.parse( Recep, Recepies_1);
        return Recepies_1;
    }

    void Storage::AllDishes()
    {
        for (unsigned int i = 0; i < Recepies.size(); i++)
        {
            if (Recepies[i]["info"]["type"] == "DessertDish"){
                DessertDish.push_back(Recepies[i]);
            } else if (Recepies[i]["info"]["type"].asString() == "FirstDish") {
                FirstDish.push_back(Recepies[i]);
            } else if (Recepies[i]["info"]["type"].asString() == "SecondDish") {
                SecondDish.push_back(Recepies[i]);
            } else if (Recepies[i]["info"]["type"].asString() == "SaladSnackDish") {
                SaladSnackDish.push_back(Recepies[i]);
            } else
                cout << "Data error. " ;
         }
    }


    vector<BasicDishProperty *> Storage::filterBySmth(const vector <BasicDishProperty *> &allDishes,
                                             int BasicDishProperty::* filterMember,
                                             int left, int right) {
        vector<BasicDishProperty *> filteredDishes;
        for (const auto &currentDish : allDishes) {
            if ((left < currentDish->*filterMember) && (currentDish->*filterMember <= right ))
                filteredDishes.push_back(currentDish);
        }
        return filteredDishes;
    }


    vector <BasicDishProperty*> Storage::getDishesByType(size_t dishType)
    {
        vector <BasicDishProperty*> DishesTypeVec;
        switch (dishType) {
            case DishTypeDessert:
                return { getPtrsFromVec(DessertDish)};
            case DishTypeFirst:
                return { getPtrsFromVec(FirstDish)};
            case DishTypeSecond:
                return { getPtrsFromVec(SecondDish)};
            case DishTypeSaladSnack:
                return { getPtrsFromVec(SaladSnackDish)};
            case Auto:
                {
                    DishesTypeVec = getPtrsFromVec(DessertDish);
                    auto tmpVec = getPtrsFromVec(FirstDish);
                    DishesTypeVec.insert(DishesTypeVec.end(), tmpVec.begin(), tmpVec.end());
                    tmpVec = getPtrsFromVec(SecondDish);
                    DishesTypeVec.insert(DishesTypeVec.end(), tmpVec.begin(), tmpVec.end());
                    tmpVec = getPtrsFromVec(SaladSnackDish);
                    DishesTypeVec.insert(DishesTypeVec.end(), tmpVec.begin(), tmpVec.end());
                    return  DishesTypeVec;
                }
            default:
                return DishesTypeVec;
        }
    }

    vector <BasicDishProperty *> Storage::filterByCookingTime(const vector<BasicDishProperty *> allDishes,
                                                    int left, int right) {
        return filterBySmth(allDishes, &BasicDishProperty::PrepearingTime, left, right);
    }

    vector <BasicDishProperty *> Storage::filterByAvgCost(const vector<BasicDishProperty *> allDishes,
                                                    int left, int right) {
        return filterBySmth(allDishes, &BasicDishProperty::average_cost_rub, left, right);
    }

    vector <BasicDishProperty *> Storage::filterByCalories(const vector<BasicDishProperty *> allDishes,
                                                    int left, int right) {
        return filterBySmth(allDishes, &BasicDishProperty::calorie_content_100gr, left, right);
    }

    vector <BasicDishProperty *> Storage::filterByIngredients( vector<BasicDishProperty *> allDishes, vector <string> ChosenIng){
        vector <BasicDishProperty*> SuitableDishes;

        for (unsigned int i=0; i < allDishes.size(); i++)
        {
          if (! ResearchIng(ChosenIng, allDishes[i]->ingredients))
                    SuitableDishes.push_back(allDishes[i]);
        }
        return SuitableDishes;
    }

    int Storage::ResearchIng(vector <string> &ChosenIng, vector <string> &DishIng ){
        unsigned j = 0;
        bool i=0;
        while ((j < ChosenIng.size())  && !i){
            if (find(DishIng.begin(), DishIng.end(), ChosenIng[j])==DishIng.end())
                i=1;
            j++;
        }
        return i;
    }




