#include <iostream>
#include "Action.h"
#include "Order.h"
#include "Volunteer.h"

// TODO make sure the rule of 5 is actually implemented.
// Constructors, destructors, and method implementations for BaseAction
/**
 * Status is initialized as error until act is done.
 */
BaseAction::BaseAction() : status(ActionStatus::ERROR){
    errorMsg = "Not yet acted";
}

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
    for (int i = 0;i<numOfSteps;i++)
        wareHouse.step();
    complete();
}

/**
 * @return "step <number_of_steps> <status>"
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
        const Customer& customer = wareHouse.getCustomer(customerId);
        if (!customer.canMakeOrder())
            error("Cannot place this order");
        int customerDistance = wareHouse.getCustomer(customerId).getCustomerDistance();
        int orderID = wareHouse.getOrderCount()+1; //TODO make sure order counter is updated
        Order* order = new Order(orderID ,customerId, customerDistance);
        wareHouse.addOrder(order);
        order = nullptr;
        complete();
    }
    // Customer doesn't exist
    catch (const std::exception&) {
        error("Cannot place this order");
    }
}

/**
 * @return "order <customer_id> <status>"
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
 * @return "customer <customer_name> <customer_type> <customer_distance> <max_orders> <status>"
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
    }
}

PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

/**
 * @return "orderStatus <order_id> <status>"
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
                std::cout << "Error retrieving order details for OrderID: " << orderId << std::endl;
            }
        }

        // Print the number of orders the customer can still place
        std::cout << "numOrdersLeft: " << (customer.getMaxOrders() - customer.getNumOrders()) << std::endl;

        complete();
    } catch (const std::exception& ex) {
        error("Customer doesn’t exist");
    }
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

/**
 * @return "customerStatus <customer_id> <status>"
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
    }
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

/**
 * @return volunteerStatus <volunteer_id> <status>
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
 * <action_n_name> <action_n_args> <action_n_status>
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
 * @return log <status>
 */
string PrintActionsLog::toString() const {
    return "log "+getStatusString();
}

// TODO finish everything below this line.

//  Implementations for Close
Close::Close() {

}

void Close::act(WareHouse &wareHouse) {

}

Close *Close::clone() const {
    return nullptr;
}

string Close::toString() const {
    return std::string();
}

//  Implementations for BackupWareHouse
BackupWareHouse::BackupWareHouse() {

}

void BackupWareHouse::act(WareHouse &wareHouse) {

}

BackupWareHouse *BackupWareHouse::clone() const {
    return nullptr;
}

string BackupWareHouse::toString() const {
    return std::string();
}

//  Implementations for RestoreWareHouse
RestoreWareHouse::RestoreWareHouse() {

}

void RestoreWareHouse::act(WareHouse &wareHouse) {

}

RestoreWareHouse *RestoreWareHouse::clone() const {
    return nullptr;
}

string RestoreWareHouse::toString() const {
    return std::string();
}
