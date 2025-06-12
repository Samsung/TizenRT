#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <semaphore.h> 

void STUB_notsupport_void(void)
{
  set_errno(ENOTSUP);
  return;
}

int STUB_notsupport_int(void)
{
  set_errno(ENOTSUP);
  return ERROR;
}

FAR sem_t *STUB_notsupport_ptr(void)
{
  set_errno(ENOTSUP);
  return (FAR sem_t *)ERROR;
}
