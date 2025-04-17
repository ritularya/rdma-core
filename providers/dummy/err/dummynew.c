//BUIld failed


#include <infiniband/verbs.h>   //added these for struct ibv_driver
				//and  also for ibv_Register_driver()
				// but they are not directly available in it
#include <infiniband/driver.h>  //added this
#include <stdio.h>
#include <stdlib.h>



// Driver operations
static const struct verbs_device_ops dummy_ops = {
	.name = "dummy",
	.match_min_abi_version = 1,
	.match_max_abi_version = 1,
	.alloc_device = dummy_device_alloc
};


static void dummy_uninit_device(struct verbs_device *verbs_device)
{
	struct dummy_device *dev = to_rdev(&verbs_device->device);

	free(dev);
}


// Minimal device allocation (required for verbs_device)
static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev)
{
	printf("Trying drivers for sysfs_dev: %p\n", sysfs_dev);
	struct dummy_device *dev;
	dev = calloc(1, sizeof(*dev));
	if (!dev) {
		return NULL;
	}
	//copying static device info
	dev->device = dummy_dev.device;  // Assign the static device
	printf("Hello World! Dummy device 'dummy0' allocated.\n");
	return dev;
}

// Static dummy device structure
//static struct ibv_device dummy_dev = {
static struct verbs_device dummy_dev = {
	.device = {
		.name = "dummy0",
		.dev_name = "dummy0",
//		.dev_path = NULL,
//		.ibdev_path = NULL,
		.dev_path = "",
		.ibdev_path = "",
		.node_type = IBV_NODE_UNKNOWN,
		.transport_type = IBV_TRANSPORT_UNKNOWN
	}
};

/*
// Return a list with just dummy0
//static struct ibv_device **dummy_get_devices(int *num_devices) {
static struct ibv_device **dummy_get_devices(struct verbs_context *context) {
	struct ibv_device **list = calloc(2, sizeof(struct ibv_device *));
	if (!list) {
//		if (num_devices) *num_devices = 0;
		return NULL;
	}
//	list[0] = &dummy_dev;
	list[0] = &dummy_dev.device;
	list[1] = NULL; // NULL-terminated list
//	if (num_devices) *num_devices = 1;
	printf("Hello World! Dummy device 'dummy0' registered.\n");
	return list;
}


// Driver structure
//static struct ibv_driver dummy_driver = {
static const struct verbs_device_ops dummy_ops = {
	.name = "dummy",
	.match_min_abi_version = 1,
	.match_max_abi_version = 1,
//	.alloc_context = dummy_get_devices
//	.get_devices = dummy_get_devices,
	.alloc_device = dummy_get_devices
//	.alloc_device = dummy_device_alloc

//Will add more ops like .init_device later if needed
};
*/
// Register the driver when the library loads
__attribute__((constructor))
static void dummy_init(void) {
//	ibv_register_driver(&dummy_driver);
	verbs_register_driver(&dummy_ops);
}
