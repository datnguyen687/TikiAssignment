//
// Created by dat on 19/08/2018.
//

#ifndef TIKITEST_CPRODUCTMANAGER_H
#define TIKITEST_CPRODUCTMANAGER_H


#include <map>
#include <memory>
#include <list>

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "jsoncpp/json/json.h"

typedef struct PRODUCT{
    PRODUCT()=delete;
    PRODUCT(const std::string name, const std::string &color, const std::string &symbol, const float &price){
        this->name=name;
        this->color=color;
        this->symbol=symbol;
        this->price=price;
        boost::uuids::uuid temp=boost::uuids::random_generator()();
        this->uuid=boost::uuids::to_string(temp);
        this->quantity=0;
    }
    std::string getUUID(){
        return uuid;
    }
    std::string name, color, symbol;
    float price;
    unsigned int quantity;
private:
    std::string uuid;
}PRODUCT;

class CProductManager {
public:
    CProductManager();
    ~CProductManager();

    bool addProduct(const std::string& name, const std::string& color, const std::string& symbol, const float& price, const unsigned int& quantity);
    Json::Value getDescriptionJson();
    std::shared_ptr<PRODUCT> getProduct(const std::string& name, const std::string& color);
    std::shared_ptr<PRODUCT> getProductByUUID(const std::string& uuid);
private:
    std::map<std::string, std::list<std::shared_ptr<PRODUCT>>> products;
};


#endif //TIKITEST_CPRODUCTMANAGER_H
