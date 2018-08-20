//
// Created by dat on 20/08/2018.
//

#ifndef TIKITEST_CRULEMANAGER_H
#define TIKITEST_CRULEMANAGER_H

#include <string>
#include <map>
#include <memory>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <jsoncpp/json/json.h>

typedef struct RULE{
    RULE(){
        fromDate="";
        toDate="";
        color="";
        group="";
        symbol="";
        subtotal=0.0f;
        discount=0.0f;
        boost::uuids::uuid temp=boost::uuids::random_generator()();
        this->uuid=boost::uuids::to_string(temp);
    }
    std::string getUUID(){
        return uuid;
    }
    Json::Value getJsonDetail(){
        Json::Value root;
        root["fromDate"]=fromDate;
        root["toDate"]=toDate;
        root["color"]=color;
        root["group"]=group;
        root["symbol"]=symbol;
        root["subtotal"]=subtotal;
        root["discount"]=discount;
        root["uuid"]=uuid;
        return root;
    }
    bool operator==(const RULE& rule) const{
        return this->fromDate==rule.fromDate && this->toDate==rule.toDate
                && this->color==rule.color && this->group==rule.group
                && this->symbol==rule.symbol && this->subtotal==rule.subtotal
                && this->discount==rule.discount;
    }
    std::string fromDate;
    std::string toDate;
    std::string color;
    std::string group;
    std::string symbol;
    float subtotal, discount;
private:
    std::string uuid;
}RULE;

class CRuleManager {
public:
    CRuleManager();

    void addRule(std::shared_ptr<RULE> rule);
    Json::Value getJsonDetail();
    std::shared_ptr<RULE> getRuleByUUID(const std::string& uuid);
private:
    std::map<std::string, std::shared_ptr<RULE>> rules;
};


#endif //TIKITEST_CRULEMANAGER_H
