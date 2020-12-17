#include <tinyara/config.h>
#include <sys/types.h>
#include <debug.h>
#include <tinyara/otp.h>
#undef dbg
#define dbg printf

static int on_write(struct otp_lowerhalf_s *dev, uint32_t address, uint8_t *data, uint32_t length)
{
	dbg("%s(%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	return 0;
}
static int on_read(struct otp_lowerhalf_s *dev, uint32_t address, uint8_t *data, uint32_t length)
{
	dbg("%s(%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	return 0;
}
static int on_lock(struct otp_lowerhalf_s *dev)
{
	dbg("%s(%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	return 0;
}

static struct otp_ops_s g_on_ops = {
	on_write,
	on_read,
	on_lock,
};

struct otp_lowerhalf_s g_on_lower = {
	&g_on_ops,
	NULL,
	NULL,
};

int create_otpnull(void)
{
	otp_register(&g_on_lower);
	return 0;
}

int destroy_otpnull(void)
{
	otp_unregister();
	return 0;
}

