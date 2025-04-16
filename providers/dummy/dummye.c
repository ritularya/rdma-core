#include <infiniband/verbs.h>
#include <infiniband/driver.h>
#include <stdio.h>
#include <stdlib.h>

static struct verbs_device *dummy_alloc_device(struct verbs_sysfs_dev *sysfs_dev)
{
    struct verbs_device *dev = calloc(1, sizeof(*dev));
    if (!dev) {
        fprintf(stderr, "Failed to allocate dummy device\n");
        return NULL;
    }
    // Minimal initialization; name is set by ibverbs_get_device_list
    dev->device.node_type = IBV_NODE_UNKNOWN;
    dev->device.transport_type = IBV_TRANSPORT_UNKNOWN;
    printf("Allocated dummy device at %p\n", dev);
    return dev;
}

static void dummy_free_device(struct verbs_device *dev)
{
    if (dev) {
        printf("Freeing dummy device at %p\n", dev);
        free(dev);
    }
}

static const struct verbs_device_ops dummy_ops = {
    .name = "dummy",
    .match_min_abi_version = 1,
    .match_max_abi_version = 2,
    .alloc_device = dummy_alloc_device,
    .free_device = dummy_free_device,
};

__attribute__((constructor))
static void dummy_init(void)
{
    printf("Registering virtual dummy device 'dummy0'\n");
    ibverbs_register_virtual_device("dummy0", &dummy_ops);
}
