#include "../include/Customer.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

// ####################################################################################### //
//                                     AbstractCustomer                                    //
// ####################################################################################### //

/**
 * Constructor
 * @param id - customer's identifier.
 * @param name - customer's name.
 * @param locationDistance - customer's distance from the Warehouse.
 * @param maxOrders - customer's order limit.
 */
Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) :
id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders) { /* Hi bodek/et */ }

/**
 * @returns the customer's name.
 */
const string & Customer::getName() const {
    return name;
}

/**
 * @returns the customer's identifier.
 */
int Customer::getId() const {
    return id;
}

/**
 * @returns the customer's distance from the Warehouse.
 */
int Customer::getCustomerDistance() const {
    return locationDistance;
}

/**
 * @returns the customer's max orders.
 */
int Customer::getMaxOrders() const {
    return maxOrders;
}

/**
 * @returns num of orders the customer has made so far
 */
int Customer::getNumOrders() const {
   return ordersId.size();
}

/**
 * @returns true if the customer can make another order, false otherwise.
 */
bool Customer::canMakeOrder() const {
    return ordersId.size() < maxOrders;
}

/**
 * @returns vector containing the customer's orders.
 */
const vector<int> &Customer::getOrdersIds() const {
    return ordersId;
}

/**
 * @param orderId - Adds an order to the customer's orders.
 * @returns -1 if the customer reached max orders, else returns the orderId.
 */
int Customer::addOrder(int orderId) {
    if(canMakeOrder()) {
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}

// ####################################################################################### //
//                                     SoldierCustomer                                     //
// ####################################################################################### //

/**
 * Constructor
 * @param id - customer's identifier.
 * @param name - customer's name.
 * @param locationDistance - customer's distance from the Warehouse.
 * @param maxOrders - customer's order limit.
 */
SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders) :
Customer(id, name, locationDistance, maxOrders) { }

/**
 * @returns a pointer to a copy of the customer.
 */
SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

// ####################################################################################### //
//                                     CivilianCustomer                                    //
// ####################################################################################### //

/**
 * Constructor
 * @param id - customer's identifier.
 * @param name - customer's name.
 * @param locationDistance - customer's distance from the Warehouse.
 * @param maxOrders - customer's order limit.
 */
CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders) :
Customer(id, name, locationDistance, maxOrders) { }

/**
 * @returns a pointer to a copy of the customer.
 */
CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
