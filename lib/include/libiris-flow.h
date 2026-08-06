// SPDX-License-Identifier: GPL-2.0-only
/*
 * libiris-flow.h - Public API for Iris Flow vision library
 */

#ifndef LIBIRIS_FLOW_H
#define LIBIRIS_FLOW_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct iris_flow_handle;

    struct iris_flow_handle* iris_flow_open(const char* device_path);
    void iris_flow_close(struct iris_flow_handle* h);

    int iris_flow_start_stream(struct iris_flow_handle* h, uint32_t width, uint32_t height,
                               uint32_t format, uint32_t fps);
    int iris_flow_stop_stream(struct iris_flow_handle* h);
    int iris_flow_get_frame(struct iris_flow_handle* h, uint32_t* frame_id, uint64_t* timestamp_ns);
    int iris_flow_set_isp(struct iris_flow_handle* h, int32_t brightness, int32_t contrast,
                          int32_t saturation);
    int iris_flow_get_status(struct iris_flow_handle* h, uint32_t* status);

    const char* iris_flow_strerror(int errnum);

#ifdef __cplusplus
}
#endif

#endif /* LIBIRIS_FLOW_H */
