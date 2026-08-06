// SPDX-License-Identifier: GPL-2.0-only
/*
 * libiris-flow.c - Userspace library for Iris Flow driver
 */

#include "libiris-flow.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IRIS_FLOW_MAGIC 0x49
#define IRIS_FLOW_IOCTL_START_STREAM _IOW(IRIS_FLOW_MAGIC, 1, uint32_t[4])
#define IRIS_FLOW_IOCTL_STOP_STREAM _IO(IRIS_FLOW_MAGIC, 2)
#define IRIS_FLOW_IOCTL_GET_FRAME _IOWR(IRIS_FLOW_MAGIC, 3, uint32_t[4])
#define IRIS_FLOW_IOCTL_SET_ISP _IOW(IRIS_FLOW_MAGIC, 4, int32_t[3])
#define IRIS_FLOW_IOCTL_GET_STATUS _IOR(IRIS_FLOW_MAGIC, 6, uint32_t)

struct iris_flow_handle
{
    int fd;
    pthread_mutex_t lock;
};

struct iris_flow_handle* iris_flow_open(const char* device_path)
{
    if (!device_path)
    {
        errno = EINVAL;
        return NULL;
    }
    int fd = open(device_path, O_RDWR);
    if (fd < 0)
        return NULL;
    struct iris_flow_handle* h = calloc(1, sizeof(*h));
    if (!h)
    {
        close(fd);
        return NULL;
    }
    h->fd = fd;
    pthread_mutex_init(&h->lock, NULL);
    return h;
}

void iris_flow_close(struct iris_flow_handle* h)
{
    if (!h)
        return;
    pthread_mutex_destroy(&h->lock);
    close(h->fd);
    free(h);
}

int iris_flow_start_stream(struct iris_flow_handle* h, uint32_t width, uint32_t height,
                           uint32_t format, uint32_t fps)
{
    if (!h)
        return -EINVAL;
    uint32_t args[4] = {width, height, format, fps};
    pthread_mutex_lock(&h->lock);
    int ret = ioctl(h->fd, IRIS_FLOW_IOCTL_START_STREAM, args);
    if (ret < 0)
        ret = -errno;
    pthread_mutex_unlock(&h->lock);
    return ret;
}

int iris_flow_stop_stream(struct iris_flow_handle* h)
{
    if (!h)
        return -EINVAL;
    pthread_mutex_lock(&h->lock);
    int ret = ioctl(h->fd, IRIS_FLOW_IOCTL_STOP_STREAM);
    if (ret < 0)
        ret = -errno;
    pthread_mutex_unlock(&h->lock);
    return ret;
}

int iris_flow_get_frame(struct iris_flow_handle* h, uint32_t* frame_id, uint64_t* timestamp_ns)
{
    if (!h)
        return -EINVAL;
    uint32_t info[4] = {0};
    pthread_mutex_lock(&h->lock);
    int ret = ioctl(h->fd, IRIS_FLOW_IOCTL_GET_FRAME, info);
    if (ret < 0)
        ret = -errno;
    else
    {
        if (frame_id)
            *frame_id = info[0];
        if (timestamp_ns)
            *timestamp_ns = info[3];
    }
    pthread_mutex_unlock(&h->lock);
    return ret;
}

int iris_flow_set_isp(struct iris_flow_handle* h, int32_t brightness, int32_t contrast,
                      int32_t saturation)
{
    if (!h)
        return -EINVAL;
    int32_t args[3] = {brightness, contrast, saturation};
    pthread_mutex_lock(&h->lock);
    int ret = ioctl(h->fd, IRIS_FLOW_IOCTL_SET_ISP, args);
    if (ret < 0)
        ret = -errno;
    pthread_mutex_unlock(&h->lock);
    return ret;
}

int iris_flow_get_status(struct iris_flow_handle* h, uint32_t* status)
{
    if (!h || !status)
        return -EINVAL;
    pthread_mutex_lock(&h->lock);
    int ret = ioctl(h->fd, IRIS_FLOW_IOCTL_GET_STATUS, status);
    if (ret < 0)
        ret = -errno;
    pthread_mutex_unlock(&h->lock);
    return ret;
}

const char* iris_flow_strerror(int errnum)
{
    switch (errnum)
    {
    case 0:
        return "Success";
    case -EINVAL:
        return "Invalid argument";
    case -EFAULT:
        return "Bad address";
    case -ENOTTY:
        return "Invalid ioctl";
    default:
        return strerror(-errnum);
    }
}
