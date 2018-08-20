//
// Created by dat on 19/08/2018.
//

#include "CUser.h"

CUser::CUser(const std::string &name, const std::string &email, const std::string &group) {
    this->name=name;
    this->email=email;
    this->group=group;
    boost::uuids::uuid temp=boost::uuids::random_generator()();
    this->uuid=boost::uuids::to_string(temp);
    this->shoppingCart=std::make_shared<SHOPPING_CART>();
}

std::string CUser::getUUID() {
    return this->uuid;
}

std::string CUser::getEmail() {
    return this->email;
}

std::string CUser::getGroup() {
    return this->group;
}

std::string CUser::getName(){
    return this->name;
}

Json::Value CUser::getJsonDetail() {
    Json::Value root;
    root["name"]=name;
    root["email"]=email;
    root["group"]=group;
    root["uuid"]=uuid;

    Json::Value shoppingCart;
    for(auto it=this->shoppingCart->items.begin(); it!=this->shoppingCart->items.end(); it++) {
        Json::Value item;
        item["productUUID"] = it->first;
        item["quantity"] = it->second;
        shoppingCart.append(item);
    }

    Json::Value items;
    items["items"]=shoppingCart;

    Json::Value rules;
    int index=0;
    for(auto it=this->shoppingCart->rules.begin(); it!=this->shoppingCart->rules.end(); it++){
        rules[index]=*it;
        index++;
    }
    root["shoppingCart"]["items"]=shoppingCart;
    root["shoppingCart"]["rules"]=rules;

    return root;
}

void CUser::add2Cart(const std::string &productUUID, const unsigned int &quantity) {
    auto found=this->shoppingCart->items.find(productUUID);
    if(found!=this->shoppingCart->items.end()){
        found->second+=quantity;
    }else{
        this->shoppingCart->items.insert({productUUID, quantity});
    }
}

unsigned int CUser::removeFromCart(const std::string &productUUID, const unsigned int &quantity) {
    auto found=this->shoppingCart->items.find(productUUID);
    if(found!=this->shoppingCart->items.end()){
        if(found->second>=quantity) {
            found->second -= quantity;
            if(found->second<=0){
                this->shoppingCart->items.erase(productUUID);
            }
            return quantity;
        }
    }
    return 0;
}

float CUser::calculateTotalPrice(std::shared_ptr<CProductManager> productManager, std::shared_ptr<CRuleManager> ruleManager) {
    return this->shoppingCart->calculatePrice(productManager, ruleManager);
}

void CUser::addRule(std::shared_ptr<RULE> rule, std::shared_ptr<CProductManager> productManager) {
    bool shouldAdd=this->group==rule->group;
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    time_t ts;

    strptime(rule->fromDate.c_str(), "%Y-%m-%d", &tm);
    ts = mktime(&tm);
//    printf("fromDate: %d \n", (int)ts); //unix time-stamp
    auto fromDate=(int)ts;

    strptime(rule->toDate.c_str(), "%Y-%m-%d", &tm);
    ts = mktime(&tm);
//    printf("toDate: %d \n", (int)ts); //unix time-stamp
    auto toDate=(int)ts;

//    strptime("2018-08-21", "%Y-%m-%d", &tm);
//    tm.tm_mon = tm.tm_mon -1;
//    ts = mktime(&tm);
//    printf("now: %d \n", (int)ts); //unix time-stamp

    time_t t = std::time(0);
    int now = static_cast<int> (t);
//    printf("now: %d \n", (int)now);

    shouldAdd &= (now>=fromDate && now<=toDate);

    if(shouldAdd){
        this->shoppingCart->addRule(rule, productManager);
    }
}