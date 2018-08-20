//
// Created by dat on 20/08/2018.
//

#include <iostream>

#include "CRuleManager.h"

CRuleManager::CRuleManager() {}

void CRuleManager::addRule(std::shared_ptr<RULE> rule) {
    for(auto it=this->rules.begin(); it!=this->rules.end(); it++){
        if(*(it->second)==*rule){
            std::cout<<"ERROR: (CRuleManager::addRule) rule already exists "
                     << rule->getJsonDetail().toStyledString()<<std::endl;
            return;
        }
    }

    this->rules.insert({rule->getUUID(), rule});
}

Json::Value CRuleManager::getJsonDetail(){
    Json::Value root;
    for(auto it=this->rules.begin(); it!=this->rules.end(); it++){
        root[it->first]=it->second->getJsonDetail();
    }
    return root;
}

std::shared_ptr<RULE> CRuleManager::getRuleByUUID(const std::string &uuid) {
    auto found=this->rules.find(uuid);
    if(found!=this->rules.end()){
        return found->second;
    }
    return nullptr;
}