#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;
class BaseAction;

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
#define VOLUNTEER "volunteer"
#define NOT_LIMITED (-1)

class ActionFactory {
public:
    ActionFactory();  // Constructor if needed for initializing state

    BaseAction* createAction(const std::string& input); //TODO complete

private:
    std::vector<std::string> splitIntoWords(const std::string& str);
};

