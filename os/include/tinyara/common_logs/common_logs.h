typedef enum
{
	/* Memory Operations */
	CMN_LOG_ALLOC_FAIL,
	CMN_LOG_NULL_CHECK_FAIL,
	CMN_LOG_OUT_OF_MEMORY,
	CMN_LOG_MEM_LEAK,
	CMN_LOG_BUFFER_OVERFLOW,

	/* File Operations */
	CMN_LOG_FILE_NFOUND,
	CMN_LOG_FILE_OPEN_ERROR,
	CMN_LOG_FILE_READ_ERROR,
	CMN_LOG_FILE_WRITE_ERROR,
	CMN_LOG_FILE_SEEK_ERROR,
	CMN_LOG_FILE_IOCTL_ERROR,

	/* Network Operations */
	CMN_LOG_CON_TIMEOUT,
	CMN_LOG_CON_REFUSED,
	CMN_LOG_DATA_TRANS_ERROR,
	CMN_LOG_HANDSHAKE_FAILED,
	CMN_LOG_CERTIFICATE_INVALID,

	/* Authentication and Authorization */
	CMN_LOG_INVALID_CREDENTIALS,

	/* Commonly used logs for printing information */
	CMN_LOG_VALUE_OF,
	CMN_LOG_INVALID_VAL,
	CMN_LOG_FAILED_OP,
	CMN_LOG_PERFORMING_OP,
	CMN_LOG_INIT_DONE,
	CMN_LOG_DEINIT_DONE,
	CMN_LOG_START,
	CMN_LOG_END,
	CMN_LOG_INVALID_DATA,
	CMN_LOG_VALID_DATA,

	CMN_LOG_MSG_MAX	

} CLOG_INDEX;

static const char* clog_message_str[CMN_LOG_MSG_MAX] =
{
	/* Memory Operations */
        "Memory allocation failed :",	//CMN_LOG_ALLOC_FAIL,
        "Null pointer check failed :",	//CMN_LOG_NULL_CHECK_FAIL,
        "Out of memory :",		//CMN_LOG_OUT_OF_MEMORY,
        "Potential memory leak :",	//CMN_LOG_MEM_LEAK,
        "Buffer overflow detected :",	//CMN_LOG_BUFFER_OVERFLOW,

	/* File Operations */
        "File not found :",		//CMN_LOG_FILE_NFOUND,
        "Failed to open file :",	//CMN_LOG_FILE_OPEN_ERROR,
        "Failed to read from file :",	//CMN_LOG_FILE_READ_ERROR,
        "Failed to write to file :", 	//CMN_LOG_FILE_WRITE_ERROR,
	"Failed to seek file :",	//CMN_LOG_FILE_SEEK_ERROR,
        "Failed ioctl call to file :", 	//CMN_LOG_FILE_IOCTL_ERROR,

	/* Network Operations */
        "Connection Timeout :",		//CMN_LOG_CON_TIMEOUT,
        "Connection Refused :",		//CMN_LOG_CON_REFUSED,
        "Data Transmission Error :",	//CMN_LOG_DATA_TRANS_ERROR,
        "Handshake failed :",		//CMN_LOG_HANDSHAKE_FAILED,
        "Invalid Certificate :",	//CMN_LOG_CERTIFICATE_INVALID,

	/* Authentication and Authorization */
	"Invalid Credentials :",	//CMN_LOG_INVALID_CREDENTIALS,
	
	/* Commonly used logs for printing information */
	"Value of :",			//CMN_LOG_VALUE_OF,
	"Invalid value of :",		//CMN_LOG_INVALID_VAL,
	"Failed operation :",		//CMN_LOG_FAILED_OP,
	"Performing operation :",	//CMN_LOG_PERFORMING_OP,
	"Init done :",			//CMN_LOG_INIT_DONE,
	"Deinit done :",		//CMN_LOG_DEINIT_DONE,
	"Start of :",			//CMN_LOG_START,
	"End of :",			//CMN_LOG_END,
	"Invalid data in :",		//CMN_LOG_INVALID_DATA
	"Valid data in :",		//CMN_LOG_VALID_DATA
};
