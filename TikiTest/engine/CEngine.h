//
// Created by dat on 19/08/2018.
//

#ifndef TIKITEST_CENGINE_H
#define TIKITEST_CENGINE_H


#include <mutex>
#include <memory>

#include "CProductManager.h"
#include "CUserManager.h"
#include "CRuleManager.h"

class CEngine {
public:
    static std::shared_ptr<CEngine> getInstance();

    void printDetail();
    bool addProduct(const std::string& name, const std::string& color, const std::string& symbol, const float& price, const unsigned int& quantity);
    bool createUser(const std::string& name, const std::string& email, const std::string& group);
    std::string login(const std::string& name, const std::string& email, const std::string& group);
    void add2Cart(const std::string& userUUID, const std::string& productName, const std::string& productColor, const unsigned int& quantity);
    void removeFromCart(const std::string& userUUID, const std::string& productName, const std::string& productColor, const unsigned int& quantity);
    float calculateTotalPrice(const std::string& userUUID);
    void addRule(const std::shared_ptr<RULE>& rule);
    void applyRule();
private:
    static std::shared_ptr<CEngine> instance;
    static std::mutex mutex;

    std::shared_ptr<CProductManager> productManager;
    std::shared_ptr<CUserManager> userManager;
    std::shared_ptr<CRuleManager> ruleManager;

    CEngine();
};


#endif //TIKITEST_CENGINE_H
