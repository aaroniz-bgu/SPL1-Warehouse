#include "Customer.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

// #######################################################################################
//                                     AbstractCustomer
// #######################################################################################

// Constructor
Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) :
id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders) { /* Hi bodek/et */ }

// Returns the customer's name.
const string & Customer::getName() const {
    return name;
}

// Returns the customer's id.
int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

// Returns maxOrders
int Customer::getMaxOrders() const {
    return maxOrders;
}

// Returns num of orders the customer has made so far
int Customer::getNumOrders() const {
   return ordersId.size();
}

// Returns true if the customer didn't reach max orders
bool Customer::canMakeOrder() const {
    return ordersId.size() < maxOrders;
}

// Returns a vector of the customer's orders ids.
const vector<int> &Customer::getOrdersIds() const {
    return ordersId;
}

// Returns OrderId if order was added successfully, -1 otherwise
int Customer::addOrder(int orderId) {
    if(canMakeOrder()) {
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}

// #######################################################################################
//                                     SoldierCustomer
// #######################################################################################

// Constructor
SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders) :
Customer(id, name, locationDistance, maxOrders) { }

// Copy constructor
SoldierCustomer::SoldierCustomer(const SoldierCustomer &other) :
Customer(other.getId(), other.getName(),
         other.getCustomerDistance(),
         other.getMaxOrders()){
    // Copying the orders
    const vector<int> &otherOrders = other.getOrdersIds();
    for (int i : otherOrders) {
        addOrder(i);
    }
}

// Returns a copy of the customer.
SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

// #######################################################################################
//                                     CivilianCustomer
// #######################################################################################

// Constructor
CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders) :
Customer(id, name, locationDistance, maxOrders) { }

// Copy constructor
CivilianCustomer::CivilianCustomer(const CivilianCustomer &other) :
        Customer(other.getId(), other.getName(),
                 other.getCustomerDistance(),
                 other.getMaxOrders()) {
    // Copying the orders
    const vector<int> &otherOrders = other.getOrdersIds();
    for (int i: otherOrders) {
        addOrder(i);
    }
}

// Returns a copy of the customer.
CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
