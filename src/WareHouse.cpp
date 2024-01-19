#include "WareHouse.h"

WareHouse::WareHouse(const string &configFilePath) {

}

void WareHouse::start() {

}

void WareHouse::addOrder(Order* order) {

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
    //TODO - like addCustomer; change signature.
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

Customer &WareHouse::getCustomer(int customerId) const {

}

Volunteer &WareHouse::getVolunteer(int volunteerId) const {

}

Order &WareHouse::getOrder(int orderId) const {

}

const vector<BaseAction*> &WareHouse::getActions() const {

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
WareHouse::~WareHouse() { // Consult with Daniel about this, bc it's disgusting imo.
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
