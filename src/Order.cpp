#include <stdexcept>
#include "Order.h"

/**
 * Constructor
 * @param id - order identifier
 * @param customerId - customer identifier
 * @param distance - distance of the customer
 */
Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance) {
    status = OrderStatus::PENDING;
    collectorId = NO_VOLUNTEER;
    driverId = NO_VOLUNTEER;
}

/**
 * @returns the order's id
 */
int Order::getId() const {
    return id;
}

/**
 * @returns the order's customer id
 */
int Order::getCustomerId() const {
    return customerId;
}

/**
 * Sets the order's status
 * @param status - order status to be set
 */
void Order::setStatus(OrderStatus status) {
    this->status = status;
}

/**
 * Sets the order's collector id
 * @param collectorId - collector id to be set
 */
void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

/**
 * Sets the order's driver id
 * @param driverId - driver id to be set
 */
void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

/**
 * @returns the order's collector id
 */
int Order::getCollectorId() const {
    return collectorId;
}

/**
 * @returns the order's driver id
 */
int Order::getDriverId() const {
    return driverId;
}

/**
 * @returns the order's status
 */
OrderStatus Order::getStatus() const {
    return status;
}

/**
 * @returns a string descriptor of the order
 */
const string Order::toString() const {
    //TODO
    throw std::logic_error("Unimplemented method");
}
