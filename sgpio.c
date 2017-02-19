/*
 * Simple GPIO Linux wrappers
 * File: "sgpio.c"
 */
//----------------------------------------------------------------------------
#include "sgpio.h"    // `sgpio_t`
#include <errno.h>    // errno
#include <string.h>   // strlen()
#include <sys/poll.h> // poll()
//----------------------------------------------------------------------------
// write `size` bytes to stream `fd` from `buf` at once
int sgpio_write(int fd, const char *buf, int size)
{
  int retv, cnt = 0;
  char *ptr = (char*) buf;

  while (size > 0)
  {
    retv = write(fd, (void*) ptr, size);
    if (retv < 0)
    {
      if (errno == EINTR)
        continue;
      else
      {
        SGPIO_DBG("write(%d) return %d in sgpio_write()", size, retv);
        return SGPIO_ERR_WRITE;
      }
    }
    ptr  += retv;
    cnt  += retv;
    size -= retv;
  }
  return cnt;
}
//----------------------------------------------------------------------------
// write num to /sys/class/gpio/export file
int sgpio_export(int num)
{
  static char fname[] = SGPIO_MAIN_PATH "export";
  char str[SGPIO_STR_MAX];
  int fd, retv, str_size;
  
  fd = open(fname, O_WRONLY | O_TRUNC);

  if (fd < 0)
  {
    SGPIO_DBG("can't open '%s' file in sgpio_export(%d)", fname, num);
    return SGPIO_ERR_EXPORT;
  }

  snprintf(str, sizeof(str), "%d", num);
  str_size = strlen(str);

  retv = sgpio_write(fd, str, str_size);
  close(fd);

  if (retv != str_size)
  {
    SGPIO_DBG("can't write to '%s' file in sgpio_export(%d)", fname, num);
    return SGPIO_ERR_EXPORT;
  }

  return SGPIO_ERR_NONE;
}
//----------------------------------------------------------------------------
// write num to /sys/class/gpio/unexport file
int sgpio_unexport(int num)
{
  static char fname[] = SGPIO_MAIN_PATH "unexport";
  char str[SGPIO_STR_MAX];
  int fd, retv, str_size;
  
  fd = open(fname, O_WRONLY | O_TRUNC);

  if (fd < 0)
  {
    SGPIO_DBG("can't open '%s' file in sgpio_unexport(%d)", fname, num);
    return SGPIO_ERR_UNEXPORT;
  }

  snprintf(str, sizeof(str), "%d", num);
  str_size = strlen(str);

  retv = sgpio_write(fd, str, str_size);
  close(fd);

  if (retv != str_size)
  {
    SGPIO_DBG("can't write to '%s' file in sgpio_unexport(%d)", fname, num);
    return SGPIO_ERR_UNEXPORT;
  }

  return SGPIO_ERR_NONE;
}
//----------------------------------------------------------------------------
// set GPIO direction mode
int sgpio_set_dir(sgpio_t *self, int dir)
{
  static char str_in[]  = SGPIO_STR_IN;
  static char str_out[] = SGPIO_STR_OUT;
  char *str, fname[SGPIO_PATH_MAX];
  int fd, str_size, retv;

  if (self->fd > 0)
  {
    close(self->fd);
    self->fd = -1;
  }

  snprintf(fname, sizeof(fname),
           SGPIO_MAIN_PATH "gpio%d/direction", self->num);

  fd = open(fname, O_WRONLY | O_TRUNC);

  if (fd < 0)
  {
    SGPIO_DBG("can't open '%s' in sgpio_set_dir(%d, %d)",
              fname, self->num, dir);
    return SGPIO_ERR_OPEN_DIR; 
  }

  if (dir == SGPIO_DIR_OUT)
    str = str_out;
  else // if (dir == SGPIO_DIR_IN)
  {
    dir = SGPIO_DIR_IN;
    str = str_in;
  }

  str_size = strlen(str);

  retv = sgpio_write(fd, str, str_size);
  close(fd);
  if (retv != str_size)
  {
    SGPIO_DBG("can't write '%s' to '%s' file in sgpio_set_dir(%d, %d)",
              str, fname, self->num, dir);
    return SGPIO_ERR_SET_DIR;
  }
  
  snprintf(fname, sizeof(fname),
           SGPIO_MAIN_PATH "gpio%d/value", self->num);
  
  if      (dir == SGPIO_DIR_IN)
    fd = open(fname, O_RDONLY);
  else if (dir == SGPIO_DIR_OUT)
    fd = open(fname, O_RDWR | O_TRUNC);

  if (fd < 0)
  {
    SGPIO_DBG("can't open '%s' "
              "in sgpio_set_dir(%d, %d)", fname, self->num, dir);
    return SGPIO_ERR_OPEN_VAL; 
  }

  self->fd  = fd;
  self->dir = dir;

  return SGPIO_ERR_NONE;
}
//----------------------------------------------------------------------------
// set GPIO edge mode
int sgpio_set_edge(sgpio_t *self, int edge)
{
  static char str_none[]    = SGPIO_STR_NONE;
  static char str_rising[]  = SGPIO_STR_RISING;
  static char str_falling[] = SGPIO_STR_FALLING;
  static char str_both[]    = SGPIO_STR_BOTH;
  char *str, fname[SGPIO_PATH_MAX];
  int fd, str_size, retv;

  snprintf(fname, sizeof(fname),
           SGPIO_MAIN_PATH "gpio%d/edge", self->num);

  fd = open(fname, O_WRONLY | O_TRUNC);

  if (fd < 0)
  {
    SGPIO_DBG("can't open '%s' in sgpio_set_edge(%d, %d)",
              fname, self->num, edge);
    return SGPIO_ERR_OPEN_EDGE; 
  }

  if      (edge == SGPIO_EDGE_RISING)
    str = str_rising;
  else if (edge == SGPIO_EDGE_FALLING)
    str = str_falling;
  else if (edge == SGPIO_EDGE_BOTH)
    str = str_both;
  else // if (edge == SGPIO_EDGE_NONE) 
  {
    edge = SGPIO_EDGE_NONE;
    str = str_none;
  }

  str_size = strlen(str);

  retv = sgpio_write(fd, str, str_size);
  close(fd);
  if (retv != str_size)
  {
    SGPIO_DBG("can't write '%s' to '%s' file in sgpio_set_edge(%d, %d)",
              str, fname, self->num, edge);
    return SGPIO_ERR_SET_EDGE;
  }
  
  self->edge = edge;
  return SGPIO_ERR_NONE;
}
//----------------------------------------------------------------------------
// get value (return 0 or 1 or error code < 0)
int sgpio_get_val(sgpio_t *self)
{
  char c;
  int retv;

  if (self->fd < 0)
  {
    SGPIO_DBG("unset mode in sgpio_get(%d)", self->num);
    return SGPIO_ERR_UNSET_DIR;
  }

  retv = lseek(self->fd, 0, SEEK_SET);
  if (retv != 0)
  {
    SGPIO_DBG("lseek(0) return %d in sgpio_get(%d)", retv, self->num);
    return SGPIO_ERR_LSEEK;
  }

  retv = read(self->fd, &c, sizeof(char));
  if (retv != sizeof(char))
  {
    SGPIO_DBG("read(%d) return %d in sgpio_get(%d)",
              (int) sizeof(char), retv, self->num);
    return SGPIO_ERR_GET;
  }

  return (c == '0') ? 0 : 1;
}
//----------------------------------------------------------------------------
// set value (return 0 or 1 or error code < 0)
int sgpio_set_val(sgpio_t *self, int val)
{
  char c = val ? '1' : '0';
  int retv;

  if (self->fd < 0)
  {
    SGPIO_DBG("unset mode in sgpio_set(%d)", self->num);
    return SGPIO_ERR_UNSET_DIR;
  }

  retv = lseek(self->fd, 0, SEEK_SET);
  if (retv != 0)
  {
    SGPIO_DBG("lseek(0) return %d in sgpio_set(%d)", retv, self->num);
    return SGPIO_ERR_LSEEK;
  }
  
  retv = write(self->fd, &c, sizeof(char));
  if (retv != sizeof(char))
  {
    SGPIO_DBG("write(%d) return %d in sgpio_set(%d)",
              (int) sizeof(char), retv, self->num);
    return SGPIO_ERR_SET;
  }

  return SGPIO_ERR_NONE;
}
//----------------------------------------------------------------------------
// pool wraper for non block read (return 0:false, 1:true, <0:error code)
// msec - timeout in ms
// if sigmask!=0 ignore interrupt by signals
int sgpio_poll_ex(const sgpio_t *self, int msec, int sigmask)
{
  struct pollfd fds[1];
  int retv;

  while (1)
  {
    fds->fd      = self->fd;
    fds->events  = POLLIN | POLLPRI | POLLERR;
    fds->revents = 0;

    retv = poll(fds, 1, msec);
    if (retv < 0)
    {
      if (errno == EINTR)
      { // interrupt by signal
        if (sigmask)
          continue;
        else
          return 0;
      }
      return SGPIO_ERR_POOL1; // error #1
    }

    //if (fds->revents & POLLHUP)
    //  continue;

    break;
  }

  if (retv > 0)
  {
    //if (fds->revents & (POLLERR | POLLHUP | POLLNVAL))
    if (fds->revents & (POLLERR | POLLNVAL))
      return SGPIO_ERR_POOL2; // error #2

    if (fds->revents & (POLLIN | POLLPRI))
      return 1; // may non block read
  }

  return 0; // empty
}
//----------------------------------------------------------------------------
const char *sgpio_errors[] = {
  "success",
  "can't write fo file",
  "can't export",
  "can't unexport",
  "can't open 'direction' file",
  "can't open 'edge' file",
  "can't write to 'direction' file",
  "can't write to 'edge' file",
  "can't open 'value' file",
  "direction is unset",
  "lseek(0) return non zero",
  "read(1) return not a one in sgpio_get_val()",
  "write(1) return not a one in sgpio_set_val()",
  "pool() return error #1",
  "pool() return error #2",
};
static const char *sgpio_error_unknown = "unknown error";
//----------------------------------------------------------------------------
// return SGPIO error string
const char *sgpio_error_str(int err)
{
  err = SGPIO_ERROR_INDEX(err);
  return (err >= 0 && err < SGPIO_ERROR_NUM) ?
         (char*) sgpio_errors[err] :
         (char*) sgpio_error_unknown;
}
//----------------------------------------------------------------------------
/*** end of "sgpio.c" file ***/

