//
// Created by dat on 19/08/2018.
//

#ifndef TIKITEST_CUSER_H
#define TIKITEST_CUSER_H

#include <map>
#include <list>
#include <string>
#include <jsoncpp/json/json.h>
#include <CRuleManager.h>
#include <set>

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/uuid/uuid_generators.hpp"

#include "CProductManager.h"

#define UNREGISTER "UNREGISTER"
#define REGISTER "REGISTER"
#define GOLD "GOLD"
#define SILVER "SILVER"

typedef struct SHOPPING_CART{
    float calculatePrice(std::shared_ptr<CProductManager> productManager, std::shared_ptr<CRuleManager> ruleManager) {
        float sum = 0.0f;
        for (auto it = items.begin(); it != items.end(); it++) {
            auto product = productManager->getProductByUUID(it->first);
            if (product != nullptr) {
                sum += product->price * (it->second);
            }
        }

        float discount = 0.0f;
        for (auto it = rules.begin(); it != rules.end(); it++) {
            auto rule = ruleManager->getRuleByUUID(*it);
            if (rule != nullptr) {
                struct tm tm;
                memset(&tm, 0, sizeof(struct tm));
                time_t ts;

                strptime(rule->fromDate.c_str(), "%Y-%m-%d", &tm);
                ts = mktime(&tm);
//    printf("fromDate: %d \n", (int)ts); //unix time-stamp
                auto fromDate = (int) ts;

                strptime(rule->toDate.c_str(), "%Y-%m-%d", &tm);
                ts = mktime(&tm);
//    printf("toDate: %d \n", (int)ts); //unix time-stamp
                auto toDate = (int) ts;

//    strptime("2018-08-21", "%Y-%m-%d", &tm);
//    tm.tm_mon = tm.tm_mon -1;
//    ts = mktime(&tm);
//    printf("now: %d \n", (int)ts); //unix time-stamp

                time_t t = std::time(0);
                int now = static_cast<int> (t);
//    printf("now: %d \n", (int)now);

                bool shouldAdd = (now >= fromDate && now <= toDate);
                if (!shouldAdd) {
                    continue;
                }

                discount += rule->discount;
            }
        }
        return sum - discount;
    }
    void addRule(std::shared_ptr<RULE> rule, std::shared_ptr<CProductManager> productManager){
        float sum=0.0f;
        for(auto it=this->items.begin(); it!=this->items.end(); it++){
            auto product=productManager->getProductByUUID(it->first);
            if(product!= nullptr){
                if(product->color==rule->color){
                    sum+=product->price*(it->second);
                }
            }
        }
        if(sum>=rule->subtotal){
            rules.insert(rule->getUUID());
        }
    }

    std::map<std::string, int> items; //based on productID
    std::set<std::string> rules;
}SHOPPING_CART;

class CUser {
public:
    CUser() = delete;
    CUser(const std::string& name, const std::string& email, const std::string& group);

    std::string getUUID();
    std::string getEmail();
    std::string getGroup();
    std::string getName();
    Json::Value getJsonDetail();
    void add2Cart(const std::string& productUUID, const unsigned int& quantity);
    unsigned int removeFromCart(const std::string& productUUID, const unsigned int& quantity);
    float calculateTotalPrice(std::shared_ptr<CProductManager> productManager, std::shared_ptr<CRuleManager> ruleManager);
    void addRule(std::shared_ptr<RULE> rule, std::shared_ptr<CProductManager> productManager);
private:
    std::string name, email, group;
    std::string uuid;
    std::shared_ptr<SHOPPING_CART> shoppingCart;
};


#endif //TIKITEST_CUSER_H
