#include <infiniband/verbs.h>
#include <infiniband/driver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const struct verbs_device_ops dummy_ops;

static _Bool dummy_match_device(struct verbs_sysfs_dev *sysfs_dev) {
    printf("Checking device: %s\n", sysfs_dev->ibdev_name);
    return strcmp(sysfs_dev->ibdev_name, "rxe1") == 0; // Match only rxe1
}

static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev) {
    printf("Entering dummy_device_alloc for %s\n", sysfs_dev->ibdev_name);
    struct verbs_device *dev = calloc(1, sizeof(*dev));
    if (!dev) {
        printf("Allocation failed in dummy_device_alloc\n");
        return NULL;
    }
    snprintf(dev->device.name, IBV_SYSFS_NAME_MAX, "dummy0"); // Set name to dummy0
    snprintf(dev->device.dev_name, IBV_SYSFS_NAME_MAX, "dummy0");
    snprintf(dev->device.dev_path, IBV_SYSFS_PATH_MAX, "%s", sysfs_dev->ibdev_path);
    snprintf(dev->device.ibdev_path, IBV_SYSFS_PATH_MAX, "%s", sysfs_dev->ibdev_path);
    dev->device.node_type = IBV_NODE_CA;
    dev->device.transport_type = IBV_TRANSPORT_IB;
    dev->ops = &dummy_ops;
    dev->core_support = sysfs_dev->abi_ver;
    printf("Dummy device 'dummy0' allocated at %p\n", dev);
    return dev;
}

static const struct verbs_match_ent dummy_match_table[] = {
    { .kind = VERBS_MATCH_SENTINEL }
};

static const struct verbs_device_ops dummy_ops = {
    .name = "dummy",
    .match_min_abi_version = 1,
    .match_max_abi_version = 2,
    .match_table = dummy_match_table,
    .match_device = dummy_match_device,
    .alloc_device = dummy_device_alloc,
};

__attribute__((constructor))
static void dummy_init(void) {
    printf("Registering dummy driver\n");
    verbs_register_driver(&dummy_ops);
    printf("Registered dummy driver\n");
}
