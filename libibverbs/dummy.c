#include <infiniband/verbs.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Static dummy device structure
static struct ibv_device dummy_dev = {
    .name = "dummy0",
    .dev_name = "dummy0",
    .dev_path = "",
    .ibdev_path = "",
    .node_type = IBV_NODE_UNKNOWN,
    .transport_type = IBV_TRANSPORT_UNKNOWN,
//    .device = NULL
};

// Function to get the dummy device
struct ibv_device *get_dummy_device(void) {
    printf("Hello World! Dummy device 'dummy0' detected.\n");
    return &dummy_dev;
}
