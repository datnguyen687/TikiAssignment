//
// Created by dat on 19/08/2018.
//

#include <iostream>
#include "CUserManager.h"

CUserManager::CUserManager() {}

CUserManager::~CUserManager() {
}

bool CUserManager::createNewUser(const std::string &name, const std::string &email, const std::string& group) {
    if(this->users.find(email)!=this->users.end()){
        std::cout<<"ERROR: (CUserManager::createNewUser) user existed "<<name<<" "<<email<<std::endl;
        return false;
    }
    std::shared_ptr<CUser> user=std::make_shared<CUser>(name, email, group);
    this->users[email]=user;
    std::cout<<"DEBUG: (CUserManager::createNewUser) user created "<<name<<" "<<email<<" "<<group<<std::endl;
    return true;
}

Json::Value CUserManager::getJsonDetail() {
    Json::Value root;
    for(auto it=this->users.begin(); it!=this->users.end(); it++){
        Json::Value user=it->second->getJsonDetail();
        root[it->first]=user;
    }
    return root;
}

std::string CUserManager::getUserUUID(const std::string &name, const std::string &email, const std::string& group) {
    auto foundEmail=this->users.find(email);
    if(foundEmail!=this->users.end()){
        if(foundEmail->second->getName()==name && foundEmail->second->getGroup()==group){
            return foundEmail->second->getUUID();
        }else{
            std::cout<<"ERROR: (CUserManager::getUserUUID) no user with name|group "<<name<<" "<<group<<std::endl;
            return "";
        }
    }
    std::cout<<"ERROR: (CUserManager::getUserUUID) no user with email "<<email<<std::endl;
    return "";
}

std::shared_ptr<CUser> CUserManager::getUserByUUID(const std::string &uuid) {
    for(auto it=this->users.begin(); it!=this->users.end(); it++){
        if(it->second->getUUID()==uuid){
            return it->second;
        }
    }

    return nullptr;
}

void CUserManager::addRule(std::shared_ptr<RULE> rule, std::shared_ptr<CProductManager> productManager) {
    for(auto it=users.begin(); it!=users.end(); it++){
        it->second->addRule(rule, productManager);
    }
}