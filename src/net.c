// Copyright (C) 2013 - Will Glozer.  All rights reserved.

#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "net.h"

/*
 * A generic protection in case you include this
 * from multiple files
 */
#ifndef _KERNEL_FASTOPEN
#define _KERNEL_FASTOPEN

/* conditional define for TCP_FASTOPEN */
#ifndef TCP_FASTOPEN
#define TCP_FASTOPEN   23
#endif

/* conditional define for MSG_FASTOPEN */
#ifndef MSG_FASTOPEN
#define MSG_FASTOPEN   0x20000000
#endif

#endif

status sock_connect(connection *c, char *host) {
    return OK;
}

status sock_close(connection *c) {
    return OK;
}

status sock_read(connection *c, size_t *n) {
    ssize_t r = read(c->fd, c->buf, sizeof(c->buf));
    *n = (size_t) r;
    return r >= 0 ? OK : ERROR;
}

status sock_write(connection *c, char *buf, size_t len, size_t *n) {
    ssize_t r;
    r = send(c->fd, buf, len, 0);
    if (r < 0) {
        switch (errno) {
            case EAGAIN: return RETRY;
            default:     return ERROR;
        }
    }
/*    if ((r = write(c->fd, buf, len)) == -1) {
        switch (errno) {
            case EAGAIN: return RETRY;
            default:     return ERROR;
        }
    }*/
    *n = (size_t) r;
    return OK;
}

size_t sock_readable(connection *c) {
    int n, rc;
    rc = ioctl(c->fd, FIONREAD, &n);
    return rc == -1 ? 0 : n;
}
