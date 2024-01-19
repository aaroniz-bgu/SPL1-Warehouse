#include "WareHouse.h"

WareHouse::WareHouse(const string &configFilePath) {

}

void WareHouse::start() {

}

void WareHouse::addOrder(Order* order) {

}

void WareHouse::addAction(BaseAction* action) {

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

}

void WareHouse::open() {

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
