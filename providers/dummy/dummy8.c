#include <infiniband/verbs.h>
#include <infiniband/driver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct ibv_device dummy_dev = {
    .name = "dummy0",
    .dev_name = "dummy0",
    .dev_path = "", // No kernel path needed
    .ibdev_path = "", // No kernel path needed
    .node_type = IBV_NODE_CA,
    .transport_type = IBV_TRANSPORT_UNKNOWN
};

static struct verbs_device *dummy_alloc_device(void) {
    struct verbs_device *vdev = calloc(1, sizeof(*vdev));
    if (!vdev) {
        printf("Failed to allocate dummy device\n");
        return NULL;
    }

    // Copy static device info into the allocated structure
    memcpy(vdev, &dummy_dev, sizeof(dummy_dev));
    printf("Dummy device 'dummy0' allocated at %p\n", vdev);
    return vdev;
}

static void dummy_free_device(struct verbs_device *vdev) {
    printf("Freeing dummy device at %p\n", vdev);
    free(vdev);
}

static const struct verbs_device_ops dummy_ops = {
    .name = "dummy",
    .match_min_abi_version = 1,
    .match_max_abi_version = 2,
    .alloc_device = dummy_alloc_device,
    .free_device = dummy_free_device
};

__attribute__((constructor))
static void dummy_init(void) {
    printf("Registering dummy driver\n");
    verbs_register_driver(&dummy_ops);
    printf("Registered dummy driver\n");
}
