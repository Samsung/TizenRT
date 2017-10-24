
static int _save_json_cbor(unsigned char *data, unsigned int len,
		const char *json_path, const char *cbor_path)
{
	FILE *fp;
	size_t nwrite;

	fp = fopen(json_path, "w");
	if (!fp) {
		printf("fopen(%s) failed.\n", json_path);
		return -1;
	}

	nwrite = fwrite(data, 1, len, fp);
	if (nwrite != len) {
		printf("fwrite result %zd mismatch with %d\n", nwrite, len);
		fclose(fp);
		return -1;
	}

	fclose(fp);

	printf("Reset '%s' success\n", json_path);

	ConvertJSONFileToCBORFile(json_path, cbor_path);

	return 0;
}

int ocf_mylight_reset_svr(void)
{
	return _save_json_cbor(default_svr_db_json, default_svr_db_json_len,
			SVR_JSON_PATH, SVR_DB_PATH);
}

int ocf_mylight_reset_introspection(void)
{
	return _save_json_cbor(introspection_json, introspection_json_len,
			INTROSPECTION_JSON_PATH, INTROSPECTION_DB_PATH);
}

