#include "../include/WareHouse.h"
#include "../include/Action.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include "../include/Volunteer.h"

/**
 * Initializes the warehouse according to the config file. all ids start at 0.
 * @param configFilePath
 */
WareHouse::WareHouse(const string &configFilePath)  :
    isOpen(false),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(0),
    volunteerCounter(0),
    actionFactory(),
    orderCounter(0)
{
    std::ifstream configFile(configFilePath);
    std::string line;
    if (!configFile.is_open()) {
        std::cout << "Failed to open config file: " << configFilePath << std::endl;
        return;
    }
    while (getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

        // Create and execute the action
        BaseAction* action = actionFactory.createAction(line, isOpen);
        if (action) {
            action->act(*this);
            delete action; // Clean up after executing the action
        } else {
            std::cout << "Error processing line in config file: " << line << std::endl;
        }
    }
    open();
    configFile.close();
}

// ONLY IN DEBUG ENVIRONMENT - DO NOT USE ANYWHERE ELSE
//void removeCarriageReturn(std::string &str) {
//    std::size_t lastPos = str.length() - 1;
//
//    if (lastPos != std::string::npos && str[lastPos] == '\r') {
//        str.erase(lastPos);
//    }
//}

/**
 * Starts the warehouse.
 * @note This function is blocking.
 */
void WareHouse::start() { //TODO Listener loop here
    cout << "Warehouse is open!" << endl;
    while(isOpen) {
        string input;
        getline(cin, input);  // Read user input
        //removeCarriageReturn(input); ONLY IN DEBUG
        try {
            BaseAction *action = actionFactory.createAction(input, isOpen);
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
            std::cout << "Error: " << ex.what() << std::endl;
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
        if(volunteer) {
            delete volunteer; //FIXME - yet to implement destructor of volunteer
        }
    }
    for (Customer * customer : customers) {
        if(customer) {
            delete customer;
        }
    }
    for (Order * order : pendingOrders) {
        if(order) {
            delete order;
        }
    }
    for (Order * order : inProcessOrders) {
        if(order) {
            delete order;
        }
    }
    for(Order * order : completedOrders) {
        if(order) {
            delete order;
        }
    }
    for (BaseAction * action : actionsLog) {
        if(action) {
            delete action; //FIXME - yet to implement destructor of BaseAction
        }
    }
}

/**
 * Copy constructor of WareHouse.
 * @param other - the WareHouse to copy.
 */
WareHouse::WareHouse(const WareHouse &other) :
    isOpen(other.isOpen),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    actionFactory(),
    orderCounter(other.orderCounter)
{
    unsigned long size = other.volunteers.size(); // Minimizing calls to size()
    volunteers = vector<Volunteer*>(size);
    for (unsigned long i = 0; i < size; i++) {
        volunteers[i] = other.volunteers[i]->clone();
    }
    size = other.customers.size();
    customers = vector<Customer*>(size);
    for (unsigned long i = 0; i < size; i++) {
        customers[i] = other.customers[i]->clone();
    }
    size = other.pendingOrders.size();
    pendingOrders = vector<Order*>(size);
    for (unsigned long i = 0; i < size; i++) {
        pendingOrders[i] = new Order(*other.pendingOrders[i]);
    }
    size = other.inProcessOrders.size();
    inProcessOrders = vector<Order*>(size);
    for (unsigned long i = 0; i < size; i++) {
        inProcessOrders[i] = new Order(*other.inProcessOrders[i]);
    }
    size = other.completedOrders.size();
    completedOrders = vector<Order*>(size);
    for (unsigned long i = 0; i < size; i++) {
        completedOrders[i] = new Order(*other.completedOrders[i]);
    }
    size = other.actionsLog.size();
    actionsLog = vector<BaseAction*>(size);
    for (unsigned long i = 0; i < size; i++) {
        actionsLog[i] = other.actionsLog[i]->clone();
    }
}

/*** Move constructor of WareHouse.
 * @param other - the WareHouse to move.
 */
WareHouse::WareHouse(WareHouse &&other) noexcept :
    isOpen(other.isOpen),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    actionFactory(),
    orderCounter(other.orderCounter)
{
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
    return *this;
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
    for(Volunteer * v : volunteers) if(v) delete v;
    for(Customer * c : customers) if(c) delete c;
    for(Order * o : pendingOrders) if(o) delete o;
    for(Order * o : inProcessOrders) if(o) delete o;
    for(Order * o : completedOrders) if(o) delete o;
    for(BaseAction * a : actionsLog) if(a) delete a;

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
const vector<Order*>& WareHouse::getPendingOrders() const {
    return pendingOrders;
}

/**
 * @return inProcessOrders vector
 */
const vector<Order*>& WareHouse::getInProcessOrders() const {
    return inProcessOrders;
}

/**
 * @return completedOrders vector
 */
const vector<Order*>& WareHouse::getCompletedOrders() const {
    return completedOrders;
}

/**
 * Performs a single step only tto call from SimulateStep::act();
 */
void WareHouse::step() {
    vector<Volunteer*> freeCollectors;
    vector<Volunteer*> freeDrivers;

    for (Volunteer* volunteer : volunteers) {
        // Automatically determines whether it's a collector or driver and adds itself only if it's available.
        volunteer->visit(freeCollectors, freeDrivers);
    }

    for(unsigned long i = 0; i < pendingOrders.size(); i++) {
        Order *order = pendingOrders[i];
        OrderStatus orderStatus = order->getStatus();
        if(orderStatus == OrderStatus::PENDING) {
            if (!freeCollectors.empty()) {
                Volunteer *volunteer = freeCollectors.back();
                freeCollectors.pop_back();

                order->setCollectorId(volunteer->getId());
                volunteer->acceptOrder(*order);
                order->setStatus(OrderStatus::COLLECTING);
                inProcessOrders.push_back(order);

                pendingOrders.erase(find(pendingOrders.begin(), pendingOrders.end(), order));
                i--; // since we're updating the vector mid-running.
            }
        }
        else if(orderStatus == OrderStatus::COLLECTING) {
            if (!freeDrivers.empty()) {
                Volunteer *driver = nullptr;
                for(unsigned long j = 0; j < freeDrivers.size() && driver == nullptr; j++) {
                    if(freeDrivers[j]->canTakeOrder(*order)) {
                        driver = freeDrivers[j];
                    }
                }
                if(driver) {
                    order->setDriverId(driver->getId());
                    driver->acceptOrder(*order);
                    order->setStatus(OrderStatus::DELIVERING);
                    inProcessOrders.push_back(order);
                    freeDrivers.erase(find(freeDrivers.begin(), freeDrivers.end(), driver));

                    pendingOrders.erase(find(pendingOrders.begin(), pendingOrders.end(), order));
                    i--; // since we're updating the vector mid-running.
                }
            }
        } else { // That's redundant, but it's here just in case.
            order->setStatus(OrderStatus::COMPLETED);
            pendingOrders.erase(find(pendingOrders.begin(), pendingOrders.end(), order));
            i--; // since we're updating the vector mid-running.
            completedOrders.push_back(order);
        }
    }

    for (unsigned long i = 0; i < volunteers.size(); i++) {
        Volunteer *volunteer = volunteers[i];
        int activeId = volunteer->getActiveOrderId();
        if(activeId != NO_ORDER) {
            volunteer->step();
            if(volunteer->getActiveOrderId() == NO_ORDER) {
                advanceOrder(volunteer->getCompletedOrderId());
                if(!volunteer->hasOrdersLeft()) {
                    delete volunteer;
                    volunteers[i] = nullptr; // just in case, it's redundant.
                }
            }
        }
    }
    volunteers.erase(remove(volunteers.begin(), volunteers.end(), nullptr), volunteers.end());
}

/**
 * Advances the order in the hierarchy with the given id.
 * @param orderId the id of the order to advance.
 */
void WareHouse::advanceOrder(int orderId) {
    unsigned long size = inProcessOrders.size();
    for(unsigned long i = 0; i < size; i++) {
        Order *order = inProcessOrders[i];
        if(order->getId() == orderId) {
            OrderStatus orderStatus = order->getStatus();
            if(orderStatus == OrderStatus::COLLECTING) {
                pendingOrders.push_back(order);
            } else if(orderStatus == OrderStatus::DELIVERING) {
                order->setStatus(OrderStatus::COMPLETED);
                completedOrders.push_back(order);
            }
            inProcessOrders.erase(find(inProcessOrders.begin(), inProcessOrders.end(), order));
            return;
        }
    }
}
