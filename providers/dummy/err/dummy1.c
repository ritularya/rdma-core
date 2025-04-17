// GETTING SEGMENTATION FAULT IN THIS CODE


#include <infiniband/verbs.h>
#include <infiniband/driver.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //for bool


static const struct verbs_device_ops dummy_ops;


static _Bool dummy_match_device(struct verbs_sysfs_dev *sysfs_dev) {
    return strcmp(sysfs_dev->ibdev_name, "rxe1") == 0;
}

/*//added new
static _Bool dummy_match_device(struct verbs_sysfs_dev *sysfs_dev)
{
	return true;  // Always match
}
//*/
static struct verbs_device dummy_dev = {
	.device = {
		.name = "dummy0",
		.dev_name = "dummy0",
		.dev_path = "",
		.ibdev_path = "",
		.node_type = IBV_NODE_UNKNOWN,
		.transport_type = IBV_TRANSPORT_UNKNOWN
	},
//	.ops = &dummy_ops,
	.ops = NULL,
//	.sz = sizeof(struct verbs_device)
};

static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev)
{
	struct verbs_device *dev = calloc(1, sizeof(*dev));
	if (!dev) {
		return NULL;
	}
	*dev = dummy_dev;
//	dev->device = dummy_dev.device;
	dev->ops = &dummy_ops;
	printf("Hello World! Dummy device 'dummy0' allocated.\n");
	return dev;
//	return &dummy_dev;
}

//////////////////////////////
/*
static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev)
{
    struct verbs_device *dev = calloc(1, sizeof(*dev));
    if (!dev) {
        return NULL;
    }
    dev->device.name = "dummy0";
    dev->device.dev_name = "dummy0";
    dev->device.dev_path = "";
    dev->device.ibdev_path = "";
    dev->device.node_type = IBV_NODE_UNKNOWN;
    dev->device.transport_type = IBV_TRANSPORT_UNKNOWN;
    dev->ops = &dummy_ops;
    printf("Hello World! Dummy device 'dummy0' allocated.\n");
    return dev;
}

///////////////////
*/
static const struct verbs_device_ops dummy_ops = {
	.name = "dummy",
	.match_min_abi_version = 1,
	.match_max_abi_version = 1,
	.match_device = dummy_match_device,  // Add this line
	.alloc_device = dummy_device_alloc
};



__attribute__((constructor))
static void dummy_init(void)
{
	printf("Registering dummy driver\n");
	verbs_register_driver(&dummy_ops);
}
