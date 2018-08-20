//
// Created by dat on 19/08/2018.
//

#ifndef TIKITEST_CUSERMANAGER_H
#define TIKITEST_CUSERMANAGER_H


#include <map>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>

#include "CRuleManager.h"
#include "CUser.h"

class CUserManager {
public:
    CUserManager();
    ~CUserManager();

    bool createNewUser(const std::string& name, const std::string& email, const std::string& group);
    Json::Value getJsonDetail();
    std::string getUserUUID(const std::string& name, const std::string& email, const std::string& group);
    std::shared_ptr<CUser> getUserByUUID(const std::string& uuid);
    void addRule(std::shared_ptr<RULE> rule, std::shared_ptr<CProductManager> productManager);
private:
    std::map<std::string, std::shared_ptr<CUser>> users; //based on email
};


#endif //TIKITEST_CUSERMANAGER_H
