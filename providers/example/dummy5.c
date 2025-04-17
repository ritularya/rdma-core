//WORKS FINE, twice allocation
#include <infiniband/verbs.h>
#include <infiniband/driver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const struct verbs_device_ops dummy_ops;

static _Bool dummy_match_device(struct verbs_sysfs_dev *sysfs_dev)
{
 	printf("Entering dummy_match_device\n");
	// Always return true to force allocation of our dummy device
	return true;
	printf("Exiting dummy_match_device\n");
}
/*
static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev)
{

	printf("Entering dummy_device_alloc\n");
	struct verbs_device *dev = calloc(1, sizeof(*dev));
	if (!dev) {
		printf("Allocation failed in dummy_device_alloc\n");
		return NULL;
	}
	// Create a unique dummy device
	snprintf(dev->device.name, IBV_SYSFS_NAME_MAX, "dummy0");
	snprintf(dev->device.dev_name, IBV_SYSFS_NAME_MAX, "dummy0");
	strcpy(dev->device.dev_path, "");
	strcpy(dev->device.ibdev_path, "");

//	dev->device.dev_path = strdup("");
//	dev->device.ibdev_path = strdup("");
	dev->device.node_type = IBV_NODE_UNKNOWN;
	dev->device.transport_type = IBV_TRANSPORT_UNKNOWN;
	dev->ops = &dummy_ops;
	printf("Dummy device 'dummy0' allocated.\n");
	return dev;
}
*/

static struct verbs_context *dummy_alloc_context(struct ibv_device *device, int cmd_fd, void *private_data)
{
    printf("Entering dummy_alloc_context\n");
    return NULL; // Minimal stub for now
}


static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev)
{
    printf("Entering dummy_device_alloc\n");
    struct verbs_device *dev = calloc(1, sizeof(*dev));
    if (!dev) {
        printf("Allocation failed in dummy_device_alloc\n");
        return NULL;
    }
    snprintf(dev->device.name, IBV_SYSFS_NAME_MAX, "dummy0");
    snprintf(dev->device.dev_name, IBV_SYSFS_NAME_MAX, "dummy0");
    // Use sysfs_dev paths to align with expectations, but keep name as dummy0
    snprintf(dev->device.dev_path, IBV_SYSFS_PATH_MAX, "%s", sysfs_dev->ibdev_path);
    snprintf(dev->device.ibdev_path, IBV_SYSFS_PATH_MAX, "%s", sysfs_dev->ibdev_path);
    dev->device.node_type = IBV_NODE_CA; // A common type, more specific than UNKNOWN
    dev->device.transport_type = IBV_TRANSPORT_IB; // Pretend it’s InfiniBand
    dev->ops = &dummy_ops;
    // Optional: Set ABI version to match sysfs_dev
    dev->core_support = sysfs_dev->abi_ver;
    printf("Dummy device 'dummy0' allocated at %p\n", dev);
    return dev;
}

static const struct verbs_match_ent dummy_match_table[] = {
	{
		.kind = VERBS_MATCH_SENTINEL }

};

static const struct verbs_device_ops dummy_ops = {
	.name = "dummy",
	.match_min_abi_version = 1,
	.match_max_abi_version = 2,
	.match_table = dummy_match_table,
	.match_device = dummy_match_device,
	.alloc_device = dummy_device_alloc,
	.alloc_context = dummy_alloc_context,

};

__attribute__((constructor))
static void dummy_init(void)
{

	printf("Registering dummy driver\n");
	verbs_register_driver(&dummy_ops);
	printf("Registered dummy driver\n");
}
