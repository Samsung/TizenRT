#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef __TIZENRT__
#include <poll.h>
#include <sys/boardctl.h>
#endif

//#include "ocf_mylight.h"
#include "iotivity_common.h"

void ocf_mylight_recovery_default(void)
{
	ocf_mylight_reset_svr();
	ocf_mylight_reset_introspection();
}

void ocf_mylight_remove_db(void)
{
	if (unlink(SVR_DB_PATH) == 0)
		MSG("Removed SVR DB");
	else
		DBG("Failed to remove SVR DB");

	if (unlink(DEVICE_DB_PATH) == 0)
		MSG("Removed DEVICE PROPERTIES DB");
	else
		DBG("Failed to remove DEVICE PROPERTIES DB");

	if (unlink(INTROSPECTION_DB_PATH) == 0)
		MSG("Removed DEVICE PROPERTIES DB");
	else
		DBG("Failed to remove DEVICE PROPERTIES DB");
}

static void toggle_light_status(void)
{
	int power = 0;
	power = sec_get_power(0);
	power += 5;
	sec_set_power(0, power);
}

#ifdef __TIZENRT__

static pthread_t _tid;

static void *loop_input(void *user_data)
{
	struct pollfd fds[2];
	int buttons[2];
	int timeout = -1;
	int nfds;
	char buf[4];

	/* SW702 */
	fds[0].fd = open("/dev/gpio42", O_RDWR);
	fds[0].events = POLLIN;

	/* SW703 */
	fds[1].fd = open("/dev/gpio44", O_RDWR);
	fds[1].events = POLLIN;

	while (1) {
		nfds = poll(fds, 2, timeout);
		if (nfds == -1) {
			DBG("poll() failed. (%s)", strerror(errno));
			break;
		}

		if (nfds == 0) {
			DBG("longpress timeout");
			timeout = -1;

			ocf_mylight_remove_db();
			ocf_mylight_recovery_default();

			boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
		} else {
			if (timeout != -1) {
				DBG("- wait longpress: canceled");
				timeout = -1;
			}
		}

		if (fds[0].revents & POLLIN) {

			lseek(fds[0].fd, 0, SEEK_SET);
			read(fds[0].fd, buf, sizeof(buf));

			if (buf[0] == '0') {
				DBG("SW702 Button pressed");
			} else if (buf[0] == '1') {
				DBG("SW702 Button released");
				toggle_light_status();
			}
		}

		if (fds[1].revents & POLLIN) {
			lseek(fds[1].fd, 0, SEEK_SET);
			read(fds[1].fd, buf, sizeof(buf));

			if (buf[0] == '0') {
				DBG("SW703 Button pressed");

				timeout = 3000;
				DBG("- wait longpress (%d msecs)", timeout);
			} else if (buf[0] == '1') {
				DBG("SW703 Button released");
			}
		}
	}

	close(fds[0].fd);
	close(fds[1].fd);

	return NULL;
}

int ocf_mylight_userinput_init(void)
{
	pthread_create(&_tid, NULL, loop_input, NULL);

	return 0;
}

#else

static void on_signal_1(int signo)
{
	MSG("SIGUSR1(%d) - toggle /a/light/0", signo);

	toggle_light_status();
}

static void on_signal_2(int signo)
{
	MSG("SIGUSR2(%d) - Reset OTM", signo);

	ocf_mylight_remove_db();
	ocf_mylight_recovery_default();

	MSG("Program exited");
	exit(0);
}

int ocf_mylight_userinput_init(void)
{
	struct sigaction usr1;
	struct sigaction usr2;

	usr1.sa_handler = on_signal_1;
	sigemptyset(&usr1.sa_mask);
	usr1.sa_flags = 0;

	if (sigaction(SIGUSR1, &usr1, 0) == -1) {
		DBG("sigaction failed. (errno=%d)", errno);
		return -1;
	}

	usr2.sa_handler = on_signal_2;
	sigemptyset(&usr2.sa_mask);
	usr2.sa_flags = 0;

	if (sigaction(SIGUSR2, &usr2, 0) == -1) {
		DBG("sigaction failed. (errno=%d)", errno);
		return -1;
	}

	MSG("Control /a/light/0 resource by signal");
	MSG(" - 'kill -USR1 %d': toggle the /a/light/0", getpid());
	MSG(" - 'kill -USR2 %d': reset OTM", getpid());

	return 0;
}
#endif
