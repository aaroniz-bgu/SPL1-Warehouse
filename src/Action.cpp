#include <iostream>
#include "../include/Action.h"
#include "../include/Volunteer.h"

// TODO make sure the rule of 5 is actually implemented.
// Constructors, destructors, and method implementations for BaseAction
/**
 * Status is initialized as error until act is done.
 * If an error occurs it will print the error.
 */
BaseAction::BaseAction() :
errorMsg("Not yet acted"),
status(ActionStatus::ERROR)
{ }

/**
 * returns the status.
 * @return status, COMPLETED = 0 if completed, ERROR = 1 if there was an error
 */
ActionStatus BaseAction::getStatus() const {
    return status;
}

/**
 * Changes the action status to COMPLETED = 0.
 */
void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

/**
 * Saves the errorMsg to the action, also changes the status of the action or ERROR = 1.
 * @param errorMsg A description of the error that occurred.
 */
void BaseAction::error(string errorMsg) {
    BaseAction::errorMsg = errorMsg; //I don't know why this wasn't a move function
    status = ActionStatus::ERROR;
}

/**
 * returns the error msg that caused the action to fail.
 * @return errorMsg.
 */
string BaseAction::getErrorMsg() const {
    return errorMsg;
}

/**
 * Gets a string version of the action status.
 * @return string "COMPLETED" if status is completed, or "ERROR" if status is error.
 */
string BaseAction::getStatusString() const {
    if (status == ActionStatus::COMPLETED)
        return "COMPLETED";
    else
        return "ERROR";
}

// Implementations for SimulateStep
/**
 * Makes a new SimulateStep action
 * @param numOfSteps - number of steps to be made. Assumes this is a positive number.
 */
SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) { }

/**
 * Performs the amount of steps as specified when making the action on the given warehouse. The completes the action.
 * @param wareHouse - warehouse to perform the steps on.
 */
 // TODO  check to make sure that this is what needs to happen.
void SimulateStep::act(WareHouse &wareHouse) {
    for (int i = 0;i<numOfSteps;i++) {
        wareHouse.step();
    }
    complete();
}

/**
 * @return "step (number_of_steps) (status)"
 */
string SimulateStep::toString() const {
    return "step "+to_string(numOfSteps)+" "+getStatusString();
}

/**
 * clones the SimulateStep action
 * @return SimulateStep object that's a copy of the current one.
 */
SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

// Implementations for AddOrder
/**
 * Creates a new AddOrder action
 * @param id - id of the customer making the order.
 */
AddOrder::AddOrder(int id) : BaseAction(), customerId(id) { }

/**
 * Adds a new order to the warehouse with the given customer id. The order id is given automatically.
 * An error will occur if the customer doesn't exist or the customer has reached their max orders already.
 * @param wareHouse - wareHouse to add the order to.
 */
void AddOrder::act(WareHouse &wareHouse) {
    try {
        Customer &customer = wareHouse.getCustomer(customerId);
        if (!customer.canMakeOrder()) {
            error("Cannot place this order");
            std::cout << getErrorMsg() << std::endl;
        }
        int customerDistance = customer.getCustomerDistance();
        int orderID = wareHouse.getOrderCount();
        Order* order = new Order(orderID ,customerId, customerDistance);
        customer.addOrder(orderID);
        wareHouse.addOrder(order);
        complete();
    }
    // Customer doesn't exist
    catch (const std::exception&) {
        error("Cannot place this order");
        std::cout << getErrorMsg() << std::endl;
    }
}

/**
 * @return "order (customer_id) (status)"
 */
string AddOrder::toString() const {
    return "order "+to_string(customerId)+" "+getStatusString();
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}

// Implementations for AddCustomer
/**
 * Creates a AddCustomer action
 * @param customerName
 * @param customerType - assumes it's either "soldier" or "civilian".
 * @param distance - assumes it's a positive number
 * @param maxOrders - assumes it's a positive number
 */
AddCustomer::AddCustomer (string customerName, string customerType, int distance, int maxOrders)
        : BaseAction(), customerName(customerName),
        customerType(customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian),
        distance(distance), maxOrders(maxOrders) { }

/**
 * This action creates a new customer and stores them in the warehouse
 * @param wareHouse
 */
void AddCustomer::act(WareHouse &wareHouse) {
    wareHouse.addCustomer(customerType == CustomerType::Soldier ? 0 : 1, customerName, distance, maxOrders);
    complete();
}

AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}

/**
 * @return "customer (customer_name) (customer_type) (customer_distance) (max_orders) (status)"
 */
string AddCustomer::toString() const {
    return "customer "+customerName+" "+(customerType == CustomerType::Soldier ? "soldier" : "civilian")+
    " "+to_string(distance)+" "+to_string(maxOrders)+" "+getStatusString();
}

//  Implementations for PrintOrderStatus
/**
 * This action prints an information on a given order, includes its status, the
 * volunteers that are associated with it and the customer who place it.
 * @param id - order id
 */
PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id) { }

/**
 * Finds the order according to the orderId, then prints its toString. Will change it's status to error if
 * the order doesn't exist.
 * @param wareHouse
 */
void PrintOrderStatus::act(WareHouse &wareHouse) {
    try {
        Order& order = wareHouse.getOrder(orderId);
        std::cout << order.toString() << std::endl;
        complete();
    }
    catch (const std::exception& ex) {
        error("Order doesn’t exist");
        std::cout << getErrorMsg() << std::endl;
    }
}

PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

/**
 * @return "orderStatus (order_id) (status)"
 */
string PrintOrderStatus::toString() const {
    return "orderStatus "+to_string(orderId)+" "+getStatusString();
}

/**
 * This action prints all information about a particular customer, includes
 * their ID, their orders status, and how many orders they may still place.
 * @param customerId
 */
PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId) { }

/**
 * Prints the customers id, details of each of their orders and then the amount of orders they can still place.
 * @param wareHouse
 */
void PrintCustomerStatus::act(WareHouse &wareHouse) {
    try {
        Customer& customer = wareHouse.getCustomer(customerId);

        std::cout << "CustomerID: " << customer.getId() << std::endl;
        // print details for each order associated with the customer
        for (int orderId : customer.getOrdersIds()) {
            try {
                Order& order = wareHouse.getOrder(orderId);
                std::cout << "OrderID: " << orderId << std::endl;
                std::cout << "OrderStatus: ";
                switch (order.getStatus()) {
                    case OrderStatus::PENDING:
                        std::cout << "Pending";
                        break;
                    case OrderStatus::COLLECTING:
                        std::cout << "Collecting";
                        break;
                    case OrderStatus::DELIVERING:
                        std::cout << "Delivering";
                        break;
                    case OrderStatus::COMPLETED:
                        std::cout << "Completed";
                        break;
                    default: // Only if an error occurs, shouldn't ever happen.
                        std::cout << "Unknown";
                }
                std::cout << std::endl;
            } catch (const std::exception& ex) {
                // Should not happen, serious error if it does.
                std::cout << "Error retrieving order details for OrderID: " << orderId << std::endl;
            }
        }
        // Print the number of orders the customer can still place
        std::cout << "numOrdersLeft: " << (customer.getMaxOrders() - customer.getNumOrders()) << std::endl;
        complete();
    } catch (const std::exception& ex) {
        error("Customer doesn’t exist");
        std::cout << getErrorMsg() << std::endl;
    }
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

/**
 * @return "customerStatus (customer_id) (status)"
 */
string PrintCustomerStatus::toString() const {
    return "customerStatus "+to_string(customerId)+" "+getStatusString();
}


//  Implementations for PrintVolunteerStatus
/**
 * This action prints all information about a particular volunteer, includes
 * their ID, if they are busy or not, the order ID they are currently processing, how much time is left until the
 * processing of the order is completed, and how many orders they still may handle.
 * @param id
 */
PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), VolunteerId(id) { }

/**
 * Does what the action it's supposed to, will change status to error if the volunteer doesn't exist.
 * @param wareHouse
 */
void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    try {
        Volunteer& volunteer = wareHouse.getVolunteer(VolunteerId);
        std::cout << volunteer.toString() << std::endl;
        complete();
    }
    catch (const std::exception& ex) {
        error("Volunteer doesn’t exist");
        std::cout << getErrorMsg() << std::endl;
    }
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

/**
 * @return volunteerStatus (volunteer_id) (status)
 */
string PrintVolunteerStatus::toString() const {
    return "volunteerStatus "+to_string(VolunteerId)+" "+getStatusString();
}

//  Implementations for PrintActionsLog
/**
 * Prints all the actions that were performed by the user (excluding current log action),
 * from the first action to the last action.
 */
PrintActionsLog::PrintActionsLog() : BaseAction() { }

/**
 * Prints each of the actions up to this action not including in order with the following syntax:
 * (action_n_name) (action_n_args) (action_n_status)
 * @param wareHouse
 */
void PrintActionsLog::act(WareHouse &wareHouse) {
    const vector<BaseAction*>& actions = wareHouse.getActions();
    for (const BaseAction* action : actions) {
        std::cout << action->toString() << std::endl;
    }
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

/**
 * @return log (status)
 */
string PrintActionsLog::toString() const {
    return "log "+getStatusString();
}


//  Implementations for Close
/**
 *  This action prints all orders with their status. Then, it closes the warehouse – changes
 *  its isOpen status, exits the loop and finishes the program.
 */
Close::Close() : BaseAction() { }

/**
 * gets an order and prints it in the following format:
 * OrderID: (order_n_id) , CustomerID:(customer_n_id) , OrderStatus: (order_n_status)
 * @param order - the order to be printed
 */
void printOrderDetails(const Order* order) {
    std::cout << "OrderID: " << order->getId()
              << " , CustomerID: " << order->getCustomerId()
              << " , OrderStatus: ";
    switch (order->getStatus()) {
        case OrderStatus::PENDING:
            std::cout << "Pending";
            break;
        case OrderStatus::COLLECTING:
            std::cout << "Collecting";
            break;
        case OrderStatus::DELIVERING:
            std::cout << "Delivering";
            break;
        case OrderStatus::COMPLETED:
            std::cout << "Completed";
            break;
        default:
            std::cout << "Unknown";
    }
    std::cout << std::endl;
};

/**
 * Prints all orders with their status, closes the warehouse and exits the loop.
 */
void Close::act(WareHouse &wareHouse) {
    for (const Order* order : wareHouse.getPendingOrders()) {
        if (order) printOrderDetails(order); // if (order) is to check that it's not a nullptr. (though it shouldn't be)
    }
    for (const Order* order : wareHouse.getInProcessOrders()) {
        if (order) printOrderDetails(order);
    }
    for (const Order* order : wareHouse.getCompletedOrders()) {
        if (order) printOrderDetails(order);
    }

    wareHouse.close();
    complete();
}

Close *Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "close "+getStatusString();
}

//  Implementations for BackupWareHouse
/**
 * Saves all warehouse information (customers, volunteers, orders, and
 * actions history) in a global variable called “backup”. The program can keep only one backup: If
 * it's called multiple times, the latest warehouse’s status will be stored and overwrite the
 * previous one.
 */
BackupWareHouse::BackupWareHouse() : BaseAction() { }

/**
 * Saves all warehouse information (customers, volunteers, orders, and
 * actions history) in the global variable called “backup”. Erasing the previous backup if there was one.
 * @param wareHouse
 */
void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }

    // Creates the new backup (copying the warehouse)
    backup = new WareHouse(wareHouse);

    complete();
}

BackupWareHouse *BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

/**
 * @return "backup (status)"
 */
string BackupWareHouse::toString() const {
    return "backup "+getStatusString();
}

//  Implementations for RestoreWareHouse
/**
 * Restore the backed-up warehouse status and overwrite the current
 * warehouse status (warehouse itself, customers, volunteers, orders, and actions history)
 * will be an error if there is no backup
 */
RestoreWareHouse::RestoreWareHouse() : BaseAction() { }

/**
 * Restores the backed-up warehouse, overwriting the current warehouse if they are different.
 * Will be an error if there is no backup.
 * @param wareHouse to be overwritten
 */
void RestoreWareHouse::act(WareHouse &wareHouse) {
    // Check if backup exists
    if (backup == nullptr) {
        error("No backup available");
        std::cout << getErrorMsg() << std::endl;
    }
    else {
        // Overwrite the current warehouse state with the backup with the = operator, should erase current warehouse data.
        wareHouse = *backup;
        complete();
    }
}

RestoreWareHouse *RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

/**
 * @return "restore (status)"
 */
string RestoreWareHouse::toString() const {
    return "restore "+getStatusString();
}

/*  Implementations for AddVolunteer - this class is purely for the initialization with the config file.
 *  These actions should not be added to the actionlog ever.
*/

/**
 * Creates a AddVolunteer action for adding a volunteer that's a collector,
 * if it's not a limited driver, set the maxOrders field to -1
 *
 * THIS ACTION SHOULD NOT BE ADDED TO THE ACTIONLOG.
 * @param name
 * @param coolDown
 * @param maxOrders - default -1, if it's limited add here.
 */
AddVolunteer::AddVolunteer(string name, int coolDown, int maxOrders) :
    BaseAction(),
    name(name),
    cooldown(coolDown),
    maxDistance(-1),
    distance_per_step(-1),
    maxOrders(maxOrders),
    type(VolunteerType::Collector)
{ }


/**
 * Creates a AddVolunteer action for adding a volunteer that's a driver,
 * if it's not a limited driver, set the maxOrders field to -1
 *
 * THIS ACTION SHOULD NOT BE ADDED TO THE ACTIONLOG.
 * @param name
 * @param maxDistance
 * @param distance_per_step
 * @param maxOrders - default -1, if it's limited add here.
 */
AddVolunteer::AddVolunteer(string name, int maxDistance, int distance_per_step, int maxOrders) :
    BaseAction(),
    name(name),
    cooldown(-1),
    maxDistance(maxDistance),
    distance_per_step(distance_per_step),
    maxOrders(maxOrders),
    type(VolunteerType::Driver) { }


/**
 * Adds the volunteer to the warehouse according to the parameters,
 * limited if maxOrders!=-1 and according to the VolunteerType (Collector or Driver).
 * @param wareHouse
 */
void AddVolunteer::act(WareHouse &wareHouse) {
    Volunteer* volunteer;
    int id = wareHouse.getVolunteerCount();
    // If given an incorrect maxOrders
    if (!(maxOrders==-1||maxOrders>0))
        return;
    switch(type){
        case VolunteerType::Collector: {
            if (maxOrders == -1)
                volunteer = new CollectorVolunteer(id, name, cooldown);
            else
                volunteer = new LimitedCollectorVolunteer(id, name, cooldown, maxOrders);
            break;
        }
        case VolunteerType::Driver: {
            if (maxOrders == -1)
                volunteer = new DriverVolunteer(id, name, maxDistance, distance_per_step);
            else
                volunteer = new LimitedDriverVolunteer(id, name, maxDistance, distance_per_step, maxOrders);
            break;
        }
    }
    wareHouse.addVolunteer(volunteer);
    complete();
}

AddVolunteer *AddVolunteer::clone() const {
    return new AddVolunteer(*this);
}

/**
 * @return "volunteer (volunteer_role) (volunteer_coolDown)/(volunteer_maxDistance) (distance_per_step)(for drivers only) (volunteer_maxOrders)(if limited)"
 */
string AddVolunteer::toString() const {
    string str = "volunteer ";

    if (type == VolunteerType::Collector) {
        str += "collector "+to_string(cooldown);
    }
    else if (type == VolunteerType::Driver) {
        str += "driver "+ to_string(maxDistance)+" "+to_string(distance_per_step);
    }
    else {
        return "error getting volunteer string";
    }
    if (maxOrders!=-1)
        str += " "+to_string(maxOrders);
    return str;
}



