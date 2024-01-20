#include "WareHouse.h"

#include <iostream>

WareHouse::WareHouse(const string &configFilePath)  : isOpen(false), customerCounter(0), volunteerCounter(0) {
    // TODO read file and init members

    start();
}

void WareHouse::start() { //TODO Listener loop here
    cout << "Warehouse is open!" << endl;
    while(isOpen) {
        string input;
    }
}

void WareHouse::addOrder(Order* order) {
//TODO
}

/**
 * This action is called only once when action is performed.
 * @param action - the action that was performed.
 */
void WareHouse::addAction(BaseAction* action) {
    //Only call this function once per action;
    actionsLog.push_back(action);
}

int WareHouse::addVolunteer(Volunteer* volunteer) {
    //TODO - like addCustomer;
    return -1;
}

/**
 * Adds a new Customer instance into the system.
 * @param type Casted from enum {@link CustomerType} at {@link Action.h}.
 * @param name The name of the customer.
 * @param locationDistance Costumer's distance from the warehouse.
 * @param maxOrders The maximum number of orders the customer can make.
 * @returns A unique identifier of the customer for future querying.
 */
int WareHouse::addCustomer(int type, const string &name, int locationDistance, int maxOrders) {
    Customer *customer = nullptr;
    // Consider using a map/switch instead of if-else, right now it's ok.
    if(type == 0) {
        customer = new SoldierCustomer(customerCounter, name, locationDistance, maxOrders);
    } else if (type == 1) {
        customer = new CivilianCustomer(customerCounter, name, locationDistance, maxOrders);
    } else {
        return -1;
    }
    customers.push_back(customer);
    customerCounter++;
    return customerCounter - 1;
}

/**
 * Returns the customer with the given id.
 * @param customerId
 * @return
 * @throws invalid_argument if customer doesn't exist.
 * @note to self: If you're displaying this project in resume - use std::optional, it's not supported in C++11.
 */
Customer &WareHouse::getCustomer(int customerId) const {
    if(customerId < customerCounter) {
        for (Customer *customer: customers) {
            if (customer->getId() == customerId) {
                return *customer;
            }
        }
    }
    throw invalid_argument("Customer doesn't exist");
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const {
    if(volunteerId < volunteerCounter) {
        for (Volunteer *volunteer: volunteers) {
            if (volunteer->getId() == volunteerId) {
                return *volunteer;
            }
        }
    }
    throw invalid_argument("Volunteer doesn't exist");
}

Order &WareHouse::getOrder(int orderId) const {

}

const vector<BaseAction*> &WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
    isOpen = false;
}

void WareHouse::open() {
    isOpen = true;
}

/**
 * Destructor of WareHouse.
 */
WareHouse::~WareHouse() {
    for(Volunteer * volunteer : volunteers) {
        delete volunteer; //FIXME - yet to implement destructor of volunteer
    }
    for (Customer * customer : customers) {
        delete customer;
    }
    for (Order * order : pendingOrders) {
        delete order;
    }
    for (Order * order : inProcessOrders) {
        delete order;
    }
    for(Order * order : completedOrders) {
        delete order;
    }
    for (BaseAction * action : actionsLog) {
        delete action; //FIXME - yet to implement destructor of BaseAction
    }
}
