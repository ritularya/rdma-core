#include <infiniband/verbs.h>
#include "driver.h"  // Internal header for ibv_driver
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Static dummy device structure
static struct ibv_device dummy_dev = {
    .name = "dummy0",
    .dev_name = "dummy0",
    .dev_path = "",
    .transport_type = IBV_TRANSPORT_UNKNOWN,
    .node_type = IBV_NODE_UNKNOWN
};

// Allocate a dummy device (minimal implementation)
static struct ibv_device *dummy_alloc_device(struct ibv_driver *driver,
                                             struct ibv_device_attr_ex *attr) {
    printf("Hello World! Dummy device 'dummy0' allocated.\n");
    return &dummy_dev;  // Return the static device for simplicity
}

// Free the dummy device (no-op since it’s static)
static void dummy_free_device(struct ibv_device *device) {
    printf("Dummy device 'dummy0' freed (no-op).\n");
    // No free needed for static device
}

// Dummy driver structure
static struct ibv_driver dummy_driver = {
    .name = "dummy",
    .dev_ops = {},  // Empty for now, no specific ops needed
    .alloc_device = dummy_alloc_device,
    .free_device = dummy_free_device,
    .get_device = NULL,  // Optional, not needed for basic detection
    .entry = TAILQ_HEAD_INITIALIZER(dummy_driver.entry)  // Initialize the tailq entry
};

// Register the driver when libibverbs loads
__attribute__((constructor))
static void dummy_register(void) {
    ibv_register_driver(&dummy_driver);
    printf("Dummy driver registered.\n");
}
