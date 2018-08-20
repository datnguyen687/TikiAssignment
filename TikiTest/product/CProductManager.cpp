//
// Created by dat on 19/08/2018.
//
#include <iostream>
#include "jsoncpp/json/json.h"

#include "CProductManager.h"

CProductManager::CProductManager() {}

CProductManager::~CProductManager() {
    if(this->products.size()>0){
        for(auto it=this->products.begin(); it!=this->products.end(); it++){
            if(it->second.size()>0){
                it->second.clear();
            }
        }
    }
}

bool CProductManager::addProduct(const std::string &name, const std::string &color, const std::string& symbol, const float& price, const unsigned int &quantity) {
    if(name.empty() || color.empty()){
        std::cout<<"ERROR: (CProductManager::addProduct) failed to addProduct, empty name|color"<<std::endl;
        return false;
    }

    auto foundName=this->products.find(name);
    if(foundName!=this->products.end()){
        auto foundColor=std::find_if(foundName->second.begin(), foundName->second.end(), [&color](const std::shared_ptr<PRODUCT>& product)->bool{
            return product->color==color;
        });
        if(foundColor!=foundName->second.end()){
            std::cout<<"DEBUG: (CProductManager::addProduct) add product, adjust symbol|price|quantity "<<name<<" "<<color<<std::endl;
            foundColor->get()->quantity+=quantity;
            if(!symbol.empty()){
                foundColor->get()->symbol=symbol;
            }
            if(price>0.0f){
                foundColor->get()->price=price;
            }
            return true;
        }
    }

    std::shared_ptr<PRODUCT> product=std::make_shared<PRODUCT>(name, color, symbol, price);
    product->quantity=quantity;
    this->products[name].push_back(product);
    std::cout<<"DEBUG: (CProductManager::addProduct) add product "<<name<<" "<<color<<" "<<symbol<<price<<std::endl;

    return true;
}

Json::Value CProductManager::getDescriptionJson() {
    Json::Value root;
    for(auto it=this->products.begin(); it!=this->products.end(); it++){
        Json::Value products=Json::arrayValue;
        for(auto it_=it->second.begin(); it_!=it->second.end(); it_++){
            Json::Value product;
            product["name"]=it_->get()->name;
            product["quantity"]=it_->get()->quantity;
            product["price"]=it_->get()->price;
            product["symbol"]=it_->get()->symbol;
            product["color"]=it_->get()->color;
            product["uuid"]=it_->get()->getUUID();

            products.append(product);
        }
        root[it->first]=products;
    }
    return root;
}

std::shared_ptr<PRODUCT> CProductManager::getProduct(const std::string &name, const std::string &color) {
    auto foundName=this->products.find(name);
    if(foundName!=this->products.end()){
        auto foundColor=std::find_if(foundName->second.begin(), foundName->second.end(), [&color](const std::shared_ptr<PRODUCT>& product)->bool{
            return product->color==color;
        });
        if(foundColor!=foundName->second.end()){
            return *foundColor;
        }else{
            std::cout<<"ERROR: (CProductManager::getProduct) can't find product with color "<<color<<std::endl;
            return nullptr;
        }
    }
    std::cout<<"ERROR: (CProductManager::getProduct) can't find product with name "<<name<<std::endl;
    return nullptr;
}

std::shared_ptr<PRODUCT> CProductManager::getProductByUUID(const std::string &uuid) {
    for(auto it=this->products.begin(); it!=this->products.end(); it++){
        auto found=std::find_if(it->second.begin(), it->second.end(), [&uuid](const std::shared_ptr<PRODUCT>& product)->bool{
            return product->getUUID()==uuid;
        });

        if(found!=it->second.end()){
            return *found;
        }
    }
    return nullptr;
}