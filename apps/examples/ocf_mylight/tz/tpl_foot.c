
struct zonelist {
	char *name;
	unsigned char *data;
	unsigned int *len;
};

static struct zonelist _zonelist[] = {zone};

int ocf_mylight_tz_setup(const char *tzname, unsigned char *data,
		unsigned int len)
{
	size_t nwrite;
	FILE *fp;
	char fullname[255];

	snprintf(fullname, 255, ":%s/%s", CONFIG_LIBC_TZDIR, tzname);

	fp = fopen(fullname + 1, "w");
	if (!fp) {
		printf("fopen(%s) failed.\n", fullname + 1);
		return -1;
	}

	nwrite = fwrite(data, 1, len, fp);
	if (nwrite != len) {
		printf("fwrite result %d mismatch with %d\n", nwrite, len);
		fclose(fp);
		return -1;
	}

	fclose(fp);

	setenv("TZ", fullname, 1);
	tzset();

	printf("timezone setup success. (TZ=%s)\n", getenv("TZ"));

	return 0;
}

int ocf_mylight_tz_select(const char *tzname)
{
	int i;

	if (!tzname)
		return -1;

	for (i = 0; i < sizeof(_zonelist) / sizeof(struct zonelist); i++) {
		if (!strcmp(tzname, _zonelist[i].name)) {
			return ocf_mylight_tz_setup(tzname, _zonelist[i].data,
					*_zonelist[i].len);
		}
	}

	printf("can't find '%s' data\n", tzname);

	return -1;
}
