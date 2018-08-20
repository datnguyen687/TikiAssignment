//
// Created by dat on 19/08/2018.
//

#include <iostream>

#include "CEngine.h"

std::shared_ptr<CEngine> CEngine::instance= nullptr;
std::mutex CEngine::mutex;

CEngine::CEngine() {
    this->productManager=std::make_shared<CProductManager>();
    this->userManager=std::make_shared<CUserManager>();
    this->ruleManager=std::make_shared<CRuleManager>();
}

std::shared_ptr<CEngine> CEngine::getInstance() {
    std::lock_guard<std::mutex> lock_guard(mutex);
    if(instance== nullptr){
        instance=std::shared_ptr<CEngine>(new CEngine());
    }
    return instance;
}

void CEngine::printDetail() {
    std::cout<<"DEBUG: (CEngine::printDetail) Product:"<<std::endl;
    std::cout<<this->productManager->getDescriptionJson().toStyledString()<<std::endl;
    std::cout<<"DEBUG: (CEngine::printDetail) User:"<<std::endl;
    std::cout<<this->userManager->getJsonDetail().toStyledString()<<std::endl;
    std::cout<<"DEBUG: (CEngine::printDetail) Rule:"<<std::endl;
    std::cout<<this->ruleManager->getJsonDetail().toStyledString()<<std::endl;
}

bool CEngine::addProduct(const std::string &name, const std::string &color, const std::string &symbol,
                         const float &price, const unsigned int &quantity) {
    return this->productManager->addProduct(name, color, symbol, price, quantity);
}

bool CEngine::createUser(const std::string &name, const std::string &email, const std::string &group) {
    return this->userManager->createNewUser(name, email, group);
}

std::string CEngine::login(const std::string &name, const std::string &email, const std::string& group) {
    return this->userManager->getUserUUID(name, email, group);
}

void CEngine::add2Cart(const std::string &userUUID, const std::string &productName, const std::string &productColor,
                       const unsigned int &quantity) {
    auto product=this->productManager->getProduct(productName, productColor);
    if(product== nullptr){
        return;
    }
    if(product->quantity<quantity){
        std::cout<<"ERROR: (CEngine::add2Cart) can't add to cart with user "<<userUUID<<": "<<productName<<" "<<productColor<<" "<<quantity<<std::endl;
        return;
    }
    auto user=this->userManager->getUserByUUID(userUUID);
    if(user== nullptr){
        return;
    }
    product->quantity-=quantity;
    user->add2Cart(product->getUUID(), quantity);
}

void CEngine::removeFromCart(const std::string &userUUID, const std::string &productName,
                             const std::string &productColor, const unsigned int &quantity) {
    auto product=this->productManager->getProduct(productName, productColor);
    if(product== nullptr){
        return;
    }
    auto user=this->userManager->getUserByUUID(userUUID);
    if(user== nullptr){
        return;
    }

    auto actualRemoval=user->removeFromCart(product->getUUID(), quantity);
    product->quantity+=actualRemoval;
}

float CEngine::calculateTotalPrice(const std::string &userUUID) {
    float sum=0.0f;

    auto user=this->userManager->getUserByUUID(userUUID);
    if(user!= nullptr){
        sum=user->calculateTotalPrice(this->productManager, ruleManager);
    }

    return sum;
}

void CEngine::addRule(const std::shared_ptr<RULE> &rule) {
    this->ruleManager->addRule(rule);
    this->userManager->addRule(rule, productManager);
}

void CEngine::applyRule() {

}