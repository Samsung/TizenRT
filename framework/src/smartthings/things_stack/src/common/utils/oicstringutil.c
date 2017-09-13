#include <stdio.h>
#include <string.h>

#include "oicstringutil.h"
#include "thingsmalloc.h"
#include "thingslogger.h"

#define TAG "[oicstringutil]"

int CompareStrings(const char* src, const char* dst)
{
	if((NULL == src) || (NULL == dst))
		return -1;

	int srcLen = strlen(src);
	int dstLen = strlen(dst);

	if(srcLen != dstLen)
	{
    	return -1;
    }

	return strncmp(src, dst, dstLen);
}

int DuplicateString(const char* src, char** dst )
{
	if(NULL == src)
	{
		return 0;
	}
	else
	{
		int len = strlen(src);
		*dst = (char *) DAMalloc(len+ 1);
		if(NULL == *dst)
		{
			printf("memory allocation failed!\n");
			return 0;
		}

		memset(*dst, 0 , len+1);
		DAStrncpy(*dst, src, len);
	}
	return 1;
}

// [Jay] Memory Leakage Should be check
void ConcatString(char** target, char* attach)
{
    char buf[MAX_BUF_LEN]={0};

    if(attach == NULL)
    {
     return;
    }

    if(*target != NULL)
    {
        DAStrncpy(buf, *target, strlen(*target)+1);
        DAFree(*target);
        *target = NULL;
    }

	if (MAX_BUF_LEN >= (strlen(buf) + strlen(attach)))
	{
    	strncat(buf, attach, MAX_BUF_LEN);
	}
	else
	{
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Something went wrong");
		return;
	}

    *target = (char*)DAMalloc(strlen(buf)+1);
    if (NULL == *target)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, THINGS_MEMORY_ERROR);
        return;
    }
    DAStrncpy(*target, buf, strlen(buf)+1);
}

int HexStringtoInt(const char* hex, int *num)
{
    return sscanf(hex,"%x",num);
}

int GetIDValueFromQuery(char idvalue[], char* inputQuery, int size)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Input query => %s", inputQuery);

    char seperators[] = "?;#&";

    if (NULL == inputQuery)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Input query is NULL");
        return 0;
    }

    if (size > MAX_INPUT_QUERY_LEN)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Input size bigger than maximum query size(%d)", MAX_INPUT_QUERY_LEN);
        return 0;
    }

    /*[Jaehong] (Need to consider trim to remove space in query) */
    // Remove empty space
    char queries[MAX_INPUT_QUERY_LEN + 1] = { 0, };
    int index = 0;
    for (int i = 0; i < strlen(inputQuery); i++) {
        if (*(inputQuery + i) == ' ') continue;
        queries[index] = *(inputQuery + i);
        index++;
    }

    // Get Query start with "id="
    char* idQuery = strstr(queries, "id=");
    if (NULL == idQuery)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "There is no \"id=\" in input query");
        return 0;
    }

    // Seperate idQuery with seperators [?, ;, #]
    for (int i = 0; i < strlen(idQuery); i++) {
        if (*(idQuery + i) == '?' || *(idQuery + i) == ';'
            || *(idQuery + i) == '#' || *(idQuery + i) == '&')
        {
            strtok(idQuery, seperators);
            break;
        }
    }

    // Get id data after "id=" and validation check.
    for (int i = 3, k = 0; i < strlen(idQuery); i++)
    {
        if (idQuery[i] >= '0' && idQuery[i] <= '9')
        {
            idvalue[k++] = idQuery[i];
        }
        else
        {
            idvalue[0] = '\0';
            return 0;
        }
    }

    return 1;
}

char* DAStrcat(char* dest, size_t destSize, const char* src)
{
    if( dest == NULL || src == NULL || destSize == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "dest(0x%X) or src(0x%X) or dest_size=%d is NULL.", dest, src, destSize);
        return NULL;
    }

    if( strlen(src) >= (destSize-strlen(dest)) )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Source Size(%d) is over than Dest-FreeSize(%d).", strlen(src), destSize-strlen(dest)-1 );
        return NULL;
    }

    return strncat(dest, src, strlen(src));
}

char* DAStrncpy (char * destination, const char * source, size_t num)
{
    if(NULL == source)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Input String is NULL");
        return NULL;
    }

    return strncpy(destination, source, num);
}