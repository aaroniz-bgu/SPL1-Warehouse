#include "../include/WareHouse.h"
#include "../include/Action.h"

#include <iostream>
#include <fstream>
#include "Volunteer.h"

/**
 * Initializes the warehouse according to the config file. all ids start at 0.
 * @param configFilePath
 */
WareHouse::WareHouse(const string &configFilePath)  : isOpen(true), customerCounter(0),
volunteerCounter(0), orderCounter(0) {
    std::ifstream configFile(configFilePath);
    std::string line;
    if (!configFile.is_open()) {
        std::cout << "Failed to open config file: " << configFilePath << std::endl;
        return;
    }
    while (getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

        // Create and execute the action
        BaseAction* action = actionFactory.createAction(line);
        if (action) {
            action->act(*this);
            delete action; // Clean up after executing the action
        } else {
            std::cout << "Error processing line in config file: " << line << std::endl;
        }
    }
    configFile.close();
}


/**
 * Starts the warehouse.
 * @note This function is blocking.
 */
void WareHouse::start() { //TODO Listener loop here
    cout << "Warehouse is open!" << endl;
    while(isOpen) {
        string input;
        getline(cin, input);  // Read user input
        try {
            BaseAction *action = actionFactory.createAction(input);
            if (action) {
                action->act(*this);
                // If the action resulted in an error it should have already printed the error.
                actionsLog.push_back(action);
            }
            else {
                cout << "Unrecognizable action!" << endl;
            }
        }
        catch (exception &ex)
        {
            std::cout << "unknown error" << std::endl;
        }
    }
    // Cleanup here if needed, I think the Close() action should handle this, not sure though.
}

/**
 * Adds a the order to the pending orders vector.
 * @note Must be a newly allocated order.
 * @param order - the order to add.
 */
void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
    orderCounter += 1;
}

/**
 * This action is called only once when action is performed.
 * @param action - the action that was performed.
 */
void WareHouse::addAction(BaseAction* action) {
    //Only call this function once per action;
    actionsLog.push_back(action);
}

/**
 * Adds a volunteer to the volunteer vector.
 * @param volunteer
 * @return the id of the volunteer added
 */
int WareHouse::addVolunteer(Volunteer* volunteer) {
    volunteers.push_back(volunteer);
    volunteerCounter += 1;
    return volunteer->getId();
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

/**
 * Returns the volunteer with the given id.
 * @param volunteerId the identifier of the volunteer.
 * @return A reference to the volunteer if exists.
 * @throws invalid_argument if volunteer doesn't exist.
 */
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

/**
 * Returns the order with the given id.
 * @param orderId the identifier of the volunteer.
 * @return A reference to the order if exists.
 * @throws invalid_argument if order doesn't exist.
 */
Order &WareHouse::getOrder(int orderId) const {
    for(Order *o : pendingOrders) {
        if(o->getId() == orderId) {
            return *o;
        }
    }
    for(Order *o : inProcessOrders) {
        if(o->getId() == orderId) {
            return *o;
        }
    }
    for(Order *o : completedOrders) {
        if(o->getId() == orderId) {
            return *o;
        }
    }
    throw invalid_argument("Order doesn't exist");
}

/**
 * @return A reference to the pending orders vector.
 */
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
 * ifs just to be safe
 */
WareHouse::~WareHouse() {
    for(Volunteer * volunteer : volunteers) {
        if(volunteer != nullptr) {
            delete volunteer; //FIXME - yet to implement destructor of volunteer
        }
    }
    for (Customer * customer : customers) {
        if(customer != nullptr) {
            delete customer;
        }
    }
    for (Order * order : pendingOrders) {
        if(order != nullptr) {
            delete order;
        }
    }
    for (Order * order : inProcessOrders) {
        if(order != nullptr) {
            delete order;
        }
    }
    for(Order * order : completedOrders) {
        if(order != nullptr) {
            delete order;
        }
    }
    for (BaseAction * action : actionsLog) {
        if(action != nullptr) {
            delete action; //FIXME - yet to implement destructor of BaseAction
        }
    }
}

/**
 * Copy constructor of WareHouse.
 * @param other - the WareHouse to copy.
 */
WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen),
customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter) {
    int size = other.volunteers.size(); // Minimizing calls to size()
    volunteers = vector<Volunteer*>(size);
    for (int i = 0; i < size; i++) {
        volunteers[i] = other.volunteers[i]->clone();
    }
    size = other.customers.size();
    customers = vector<Customer*>(size);
    for (int i = 0; i < size; i++) {
        customers[i] = other.customers[i]->clone();
    }
    size = other.pendingOrders.size();
    pendingOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        pendingOrders[i] = new Order(*other.pendingOrders[i]);
    }
    size = other.inProcessOrders.size();
    inProcessOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        inProcessOrders[i] = new Order(*other.inProcessOrders[i]);
    }
    size = other.completedOrders.size();
    completedOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        completedOrders[i] = new Order(*other.completedOrders[i]);
    }
    size = other.actionsLog.size();
    actionsLog = vector<BaseAction*>(size);
    for (int i = 0; i < size; i++) {
        actionsLog[i] = other.actionsLog[i]->clone();
    }
}

/*** Move constructor of WareHouse.
 * @param other - the WareHouse to move.
 */
WareHouse::WareHouse(WareHouse &&other) noexcept : isOpen(other.isOpen),
customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter) {
    int size = other.volunteers.size(); // Minimizing calls to size()
    volunteers = vector<Volunteer*>(size);
    for (int i = 0; i < size; i++) {
        volunteers[i] = other.volunteers[i];
        other.volunteers[i] = nullptr;
    }
    size = other.customers.size();
    customers = vector<Customer*>(size);
    for (int i = 0; i < size; i++) {
        customers[i] = other.customers[i];
        other.customers[i] = nullptr;
    }
    size = other.pendingOrders.size();
    pendingOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        pendingOrders[i] = other.pendingOrders[i];
        other.pendingOrders[i] = nullptr;
    }
    size = other.inProcessOrders.size();
    inProcessOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        inProcessOrders[i] = other.inProcessOrders[i];
        other.inProcessOrders[i] = nullptr;
    }
    size = other.completedOrders.size();
    completedOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        completedOrders[i] = other.completedOrders[i];
        other.completedOrders[i] = nullptr;
    }
    size = other.actionsLog.size();
    actionsLog = vector<BaseAction*>(size);
    for (int i = 0; i < size; i++) {
        actionsLog[i] = other.actionsLog[i];
        other.actionsLog[i] = nullptr;
    }

    other.volunteers.clear();
    other.customers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.actionsLog.clear();
}

WareHouse& WareHouse::operator=(const WareHouse &other) {
    if(this == &other) return *this; // Self assignment

    freeResources();

    isOpen = other.isOpen;
    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;

    for (const Volunteer * v: other.volunteers) {
        volunteers.push_back(v->clone());
    }
    for (const Customer * c: other.customers) {
        customers.push_back(c->clone());
    }
    for (const Order * o: other.pendingOrders) {
        pendingOrders.push_back(new Order(*o));
    }
    for (const Order * o: other.inProcessOrders) {
        inProcessOrders.push_back(new Order(*o));
    }
    for (const Order * o: other.completedOrders) {
        completedOrders.push_back(new Order(*o));
    }
    for(const BaseAction * a : other.actionsLog) {
        actionsLog.push_back(a->clone());
    }
}

WareHouse& WareHouse::operator=(WareHouse &&other) noexcept {
    if(this == &other) return *this; // Self assignment

    freeResources();
    isOpen = other.isOpen;
    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;

    int size = other.volunteers.size(); // Minimizing calls to size()
    volunteers = vector<Volunteer*>(size);
    for (int i = 0; i < size; i++) {
        volunteers[i] = other.volunteers[i];
        other.volunteers[i] = nullptr;
    }
    size = other.customers.size();
    customers = vector<Customer*>(size);
    for (int i = 0; i < size; i++) {
        customers[i] = other.customers[i];
        other.customers[i] = nullptr;
    }
    size = other.pendingOrders.size();
    pendingOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        pendingOrders[i] = other.pendingOrders[i];
        other.pendingOrders[i] = nullptr;
    }
    size = other.inProcessOrders.size();
    inProcessOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        inProcessOrders[i] = other.inProcessOrders[i];
        other.inProcessOrders[i] = nullptr;
    }
    size = other.completedOrders.size();
    completedOrders = vector<Order*>(size);
    for (int i = 0; i < size; i++) {
        completedOrders[i] = other.completedOrders[i];
        other.completedOrders[i] = nullptr;
    }
    size = other.actionsLog.size();
    actionsLog = vector<BaseAction*>(size);
    for (int i = 0; i < size; i++) {
        actionsLog[i] = actionsLog[i];
        other.actionsLog[i] = nullptr;
    }

    other.volunteers.clear();
    other.customers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.actionsLog.clear();

    return *this;
}

/**
 * Don't even think about using this function.
 * Frees all resources used by the WareHouse.
 */
void WareHouse::freeResources() {
    //[!] Notice - this is a delete operation in an if statement which is in a for loop.
    for(Volunteer * v : volunteers) if(v != nullptr) delete v;
    for(Customer * c : customers) if(c != nullptr) delete c;
    for(Order * o : pendingOrders) if(o != nullptr) delete o;
    for(Order * o : inProcessOrders) if(o != nullptr) delete o;
    for(Order * o : completedOrders) if(o != nullptr) delete o;
    for(BaseAction * a : actionsLog) if(a != nullptr) delete a;

    volunteers.clear();
    customers.clear();
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    actionsLog.clear();
}

/**
 * @return amount of customers made, for id purposes.
 */
int WareHouse::getCustomerCount() const {
    return customerCounter;
}

/**
 * @return amount of volunteers made, for id purposes.
 */
int WareHouse::getVolunteerCount() const {
    return volunteerCounter;
}

/**
 * @return amount of orders made, for id purposes.
 */
int WareHouse::getOrderCount() const {
    return orderCounter;
}

/**
 * @return pendingOrders vector
 */
const vector<Order*>& WareHouse::GetPendingOrders() const {
    return pendingOrders;
}

/**
 * @return inProcessOrders vector
 */
const vector<Order*>& WareHouse::GetInProcessOrders() const {
    return inProcessOrders;
}

/**
 * @return completedOrders vector
 */
const vector<Order*>& WareHouse::GetCompletedOrders() const {
    return completedOrders;
}
