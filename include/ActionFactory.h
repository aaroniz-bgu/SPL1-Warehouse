#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;
class BaseAction; //Couldn't include since it makes a circular include, it's troublesome, need to think of a proper solution.
//TODO decide whether to add each of the different action type classes here or find a better solution.



#define STEP "step"
#define ORDER "order"
#define CUSTOMER "customer"
#define ORDERSTATUS "orderStatus"
#define CUSTOMERSTATUS "customerStatus"
#define VOLUNTEERSTATUS "volunteerStatus"
#define LOG "log"
#define CLOSE "close"
#define BACKUP "backup"
#define RESTORE "restore"

class ActionFactory {
public:
    ActionFactory();  // Constructor if needed for initializing state

    BaseAction* createAction(const std::string& input); //TODO complete

private:
    std::vector<std::string> splitIntoWords(const std::string& str);
};

