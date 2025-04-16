#include <infiniband/verbs.h>
#include <stdlib.h>
#include <stdio.h>

// Static dummy device structure
static struct ibv_device dummy_dev = {
    .name = "dummy0",
    .dev_name = "dummy0",
    .dev_path = NULL,
    .transport_type = IBV_TRANSPORT_UNKNOWN,
    .node_type = IBV_NODE_UNKNOWN
};

// Function to return the dummy device list
static struct ibv_device **dummy_get_device_list(int *num_devices) {
    struct ibv_device **list = calloc(1, sizeof(struct ibv_device *));
    if (!list) {
        perror("Failed to allocate device list");
        return NULL;
    }
    list[0] = &dummy_dev;
    if (num_devices) *num_devices = 1;
    printf("Hello World! Dummy device 'dummy0' detected.\n");
    return list;
}

// Dummy driver structure
static struct ibv_driver dummy_driver = {
    .name = "dummy",
    .get_devices = dummy_get_device_list
};

// Register the driver when libibverbs loads
__attribute__((constructor))
static void dummy_register(void) {
    ibv_register_driver(&dummy_driver);
    printf("Dummy driver registered.\n");
}
