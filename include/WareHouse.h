#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "ActionFactory.h"

class BaseAction;
class ActionFactory;
class Volunteer;


// Warehouse responsible for Volunteers, Customers Actions, and Orders.

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        // Student defined functions and variables:
        int addVolunteer(Volunteer* volunteer);
        int addCustomer(int type, const string &name, int locationDistance, int maxOrders);
  
        ~WareHouse(); //Destructor
        int getCustomerCount() const;
        int getVolunteerCount() const;
        int getOrderCount() const;
        const vector<Order*> &GetPendingOrders() const;
        const vector<Order*> &GetInProcessOrders() const;
        const vector<Order*> &GetCompletedOrders() const;
        WareHouse(const WareHouse &other);
        WareHouse(WareHouse &&other) noexcept;
        WareHouse& operator=(const WareHouse &other);
        WareHouse& operator=(WareHouse &&other) noexcept;

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs

        ActionFactory actionFactory;
        int orderCounter; //For assigning unique order IDs
        void freeResources();
};