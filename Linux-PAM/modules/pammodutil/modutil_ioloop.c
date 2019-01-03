/*
 * $Id: modutil_ioloop.c,v 1.2 2004/09/24 09:18:22 kukuk Exp $
 *
 * These functions provides common methods for ensure a complete read or
 * write occurs. It handles EINTR and partial read/write returns.
 */

#include <unistd.h>
#include <errno.h>

#include <security/pam_modules.h>
#include "include/security/_pam_modutil.h"

int _pammodutil_read(int fd, char *buffer, int count)
{
       int block, offset = 0;

       while (count > 0) {
               block = read(fd, &buffer[offset], count);

               if (block < 0) {
                       if (errno == EINTR) continue;
                       return block;
               }
               if (block == 0) return offset;

               offset += block;
               count -= block;
       }

       return offset;
}

int _pammodutil_write(int fd, const char *buffer, int count)
{
       int block, offset = 0;

       while (count > 0) {
               block = write(fd, &buffer[offset], count);

               if (block < 0) {
                       if (errno == EINTR) continue;
                       return block;
               }
               if (block == 0) return offset;

               offset += block;
               count -= block;
       }

       return offset;
}
