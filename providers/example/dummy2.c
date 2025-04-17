//WORKING, dummy device allocated twice
#include <infiniband/verbs.h>
#include <infiniband/driver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  // For _Bool

static const struct verbs_device_ops dummy_ops;
//because of this, correct allocation
static _Bool dummy_match_device(struct verbs_sysfs_dev *sysfs_dev)
{
        return strcmp(sysfs_dev->ibdev_name, "rxe_dummy0") == 0;
}

/*
static _Bool dummy_match_device(struct verbs_sysfs_dev *sysfs_dev)
{
	return true;  // Always match any device
}
*/
static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev)
{
	struct verbs_device *dev = calloc(1, sizeof(*dev));
	if (!dev) {
		return NULL;
	}
	strcpy(dev->device.name, "dummy0");
	strcpy(dev->device.dev_name, "dummy0");
	strcpy(dev->device.dev_path, "");
	strcpy(dev->device.ibdev_path, "");
	dev->device.node_type = IBV_NODE_UNKNOWN;
	dev->device.transport_type = IBV_TRANSPORT_UNKNOWN;
	dev->ops = &dummy_ops;
	printf("Dummy device 'dummy0' allocated.\n");
	return dev;
}

static const struct verbs_match_ent dummy_match_table[] = {
	{ .kind = VERBS_MATCH_SENTINEL }  // Minimal table with sentinel
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
static void dummy_init(void)
{
	printf("Registering dummy driver\n");
	verbs_register_driver(&dummy_ops);
}
