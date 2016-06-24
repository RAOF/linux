#include <linux/module.h>
#include <drm/drmP.h>
#include <drm/drm_gem.h>

#define DRIVER_NAME	"chaosmonkey"
#define DRIVER_DESC	"Chaos Monkey testing KMS driver"
#define DRIVER_DATE	"20160624"
#define DRIVER_MAJOR	1
#define DRIVER_MINOR	0


static const struct drm_ioctl_desc chaos_ioctls[] = {
};

static const struct file_operations chaos_driver_fops = {
	.owner		= THIS_MODULE,
	.open		= drm_open,
	.mmap		= drm_gem_mmap,
	.poll		= drm_poll,
	.read		= drm_read,
	.unlocked_ioctl	= drm_ioctl,
	.release	= drm_release,
};

static struct drm_driver chaos_driver = {
	.driver_features	= DRIVER_MODESET,
	.ioctls			= chaos_ioctls,
	.fops			= &chaos_driver_fops,
	.name			= DRIVER_NAME,
	.desc			= DRIVER_DESC,
	.date			= DRIVER_DATE,
	.major			= DRIVER_MAJOR,
	.minor			= DRIVER_MINOR,
};

struct drm_device *chaos_device;

static int __init chaos_unleash(void)
{
	int ret;

	chaos_device = drm_dev_alloc(&chaos_driver, NULL);
	if (!chaos_device) {
		ret = -ENOMEM;
		goto out;
	}

	drm_dev_set_unique(chaos_device, "chaosmonkey");

	ret = drm_dev_register(chaos_device, 0);

	if (ret)
		goto out_unref;

	return 0;

out_unref:
	drm_dev_unref(chaos_device);
out:
	return ret;
}

static void __exit chaos_restrain(void)
{
	drm_dev_unregister(chaos_device);
	drm_dev_unref(chaos_device);
}

module_init(chaos_unleash);
module_exit(chaos_restrain);

MODULE_AUTHOR("Canonical Inc.");
MODULE_DESCRIPTION(MODULE_DESC);
MODULE_LICENSE("GPL and additional rights");
