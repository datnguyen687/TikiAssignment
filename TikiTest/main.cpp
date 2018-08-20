#include <iostream>
#include "CEngine.h"

int main() {
    CEngine::getInstance()->addProduct("Iphone", "Silver", "$", 999, 1);
    CEngine::getInstance()->addProduct("Iphone", "Silver", "$", 999, 1);
    CEngine::getInstance()->addProduct("Iphone", "Black", "$", 899, 1);

    CEngine::getInstance()->createUser("John Doe 1", "john.doe@example.com", GOLD);

    std::cout << "#BEFORE:" << std::endl;
    CEngine::getInstance()->printDetail();

    std::string uuid = CEngine::getInstance()->login("John Doe 1", "john.doe@example.com", GOLD);

    CEngine::getInstance()->add2Cart(uuid, "Iphone", "Silver", 1);
    CEngine::getInstance()->add2Cart(uuid, "Iphone", "Silver", 1);

    CEngine::getInstance()->removeFromCart(uuid, "Iphone", "Silver", 1);
    CEngine::getInstance()->add2Cart(uuid, "Iphone", "Black", 1);
    CEngine::getInstance()->removeFromCart(uuid, "Iphone", "Silver", 1);
    CEngine::getInstance()->add2Cart(uuid, "Iphone", "Silver", 1);
    CEngine::getInstance()->add2Cart(uuid, "Iphone", "Silver", 1);

    std::cout<<"Total Price (Before):"<<CEngine::getInstance()->calculateTotalPrice(uuid)<<std::endl;
    std::shared_ptr<RULE> rule=std::make_shared<RULE>();
    rule->discount=50;
    rule->subtotal=150;
    rule->symbol="$";
    rule->toDate="2018-08-21";
    rule->fromDate="2018-08-19";
    rule->group=GOLD;
    rule->color="Black";
    CEngine::getInstance()->addRule(rule);
    std::cout << "#AFTER:" << std::endl;
    CEngine::getInstance()->printDetail();
    std::cout<<"Total Price (After):"<<CEngine::getInstance()->calculateTotalPrice(uuid)<<std::endl;
    return 0;
}