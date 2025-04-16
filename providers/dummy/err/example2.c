//BUILD FAILED


#include <infiniband/verbs.h>
#include <infiniband/driver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const struct verbs_device_ops dummy_ops;
static struct verbs_device *dummy_dev_instance;  // Global instance for standalone device

// Custom context structure
struct dummy_context {
    struct ibv_context ibv_ctx;  // Must be first
};

// Minimal context ops
static void dummy_free_context(struct ibv_context *ibctx)
{
    struct dummy_context *ctx = (struct dummy_context *)ibctx;
    free(ctx);
}

static const struct verbs_context_ops dummy_ctx_ops = {
    .free_context = dummy_free_context,
};

static struct verbs_context *dummy_alloc_context(struct ibv_device *ibdev, int cmd_fd, void *private_data)
{
    struct dummy_context *ctx = calloc(1, sizeof(*ctx));
    if (!ctx) {
        printf("Failed to allocate context\n");
        return NULL;
    }
    // Initialize the ibv_context portion
    ctx->ibv_ctx.device = ibdev;
    ctx->ibv_ctx.cmd_fd = cmd_fd;
    verbs_set_ops(&ctx->ibv_ctx, &dummy_ctx_ops);
    printf("Dummy context allocated for 'dummy0'\n");
    return (struct verbs_context *)&ctx->ibv_ctx;  // Cast to verbs_context
}

static _Bool dummy_match_device(struct verbs_sysfs_dev *sysfs_dev)
{
    printf("Entering dummy_match_device\n");
    // Only match if we haven’t allocated our dummy device yet
    if (!dummy_dev_instance) {
        return true;
    }
    return false;  // Don’t interfere with real devices like rxe0
}

static struct verbs_device *dummy_device_alloc(struct verbs_sysfs_dev *sysfs_dev)
{
    printf("Entering dummy_device_alloc\n");
    if (dummy_dev_instance) {
        printf("Dummy device already allocated\n");
        return NULL;  // Only allocate once
    }

    struct verbs_device *dev = calloc(1, sizeof(*dev));
    if (!dev) {
        printf("Allocation failed in dummy_device_alloc\n");
        return NULL;
    }
    snprintf(dev->device.name, IBV_SYSFS_NAME_MAX, "dummy0");
    snprintf(dev->device.dev_name, IBV_SYSFS_NAME_MAX, "dummy0");
    strcpy(dev->device.dev_path, "");
    strcpy(dev->device.ibdev_path, "");
    dev->device.node_type = IBV_NODE_UNKNOWN;
    dev->device.transport_type = IBV_TRANSPORT_UNKNOWN;
    dev->ops = &dummy_ops;
    dummy_dev_instance = dev;  // Store the instance
    printf("Dummy device 'dummy0' allocated.\n");
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
    .alloc_context = dummy_alloc_context,
};

__attribute__((constructor))
static void dummy_init(void)
{
    printf("Registering dummy driver\n");
    verbs_register_driver(&dummy_ops);
    printf("Registered dummy driver\n");
}
