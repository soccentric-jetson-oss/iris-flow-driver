// SPDX-License-Identifier: GPL-2.0-only
/*
 * iris_flow_uapi.h - Userspace API for Iris Flow vision driver
 *
 * Defines ioctl numbers and structs for camera/ISP control on
 * NVIDIA Jetson AGX Orin.
 */

#ifndef IRIS_FLOW_UAPI_H
#define IRIS_FLOW_UAPI_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define IRIS_FLOW_MAGIC 0x49

/* Camera stream states */
#define IRIS_FLOW_STREAM_STOPPED 0
#define IRIS_FLOW_STREAM_RUNNING 1
#define IRIS_FLOW_STREAM_ERROR 2

/* Pixel formats */
#define IRIS_FLOW_FMT_RAW8 0
#define IRIS_FLOW_FMT_RAW10 1
#define IRIS_FLOW_FMT_RAW12 2
#define IRIS_FLOW_FMT_YUV422 3
#define IRIS_FLOW_FMT_RGB888 4

/* ISP processing flags */
#define IRIS_FLOW_ISP_AUTO_WB BIT(0)
#define IRIS_FLOW_ISP_AUTO_EXP BIT(1)
#define IRIS_FLOW_ISP_AUTO_FOCUS BIT(2)
#define IRIS_FLOW_ISP_DENOISE BIT(3)
#define IRIS_FLOW_ISP_HDR BIT(4)

struct iris_flow_stream_config
{
    __u32 width;
    __u32 height;
    __u32 pixel_format;
    __u32 fps;
    __u32 isp_flags;
    __u32 num_buffers;
    __u64 reserved[4];
};

struct iris_flow_frame_info
{
    __u32 frame_id;
    __u32 width;
    __u32 height;
    __u32 pixel_format;
    __u32 stride;
    __u32 buffer_size;
    __u64 timestamp_ns;
    __u32 isp_status;
    __u32 reserved[3];
};

struct iris_flow_isp_config
{
    __s32 brightness;
    __s32 contrast;
    __s32 saturation;
    __s32 hue;
    __u32 sharpness;
    __u32 gamma;
    __u32 wb_red_gain;
    __u32 wb_green_gain;
    __u32 wb_blue_gain;
    __u32 flags;
    __u64 reserved[4];
};

#define IRIS_FLOW_IOCTL_START_STREAM _IOW(IRIS_FLOW_MAGIC, 1, struct iris_flow_stream_config)
#define IRIS_FLOW_IOCTL_STOP_STREAM _IO(IRIS_FLOW_MAGIC, 2)
#define IRIS_FLOW_IOCTL_GET_FRAME _IOWR(IRIS_FLOW_MAGIC, 3, struct iris_flow_frame_info)
#define IRIS_FLOW_IOCTL_SET_ISP _IOW(IRIS_FLOW_MAGIC, 4, struct iris_flow_isp_config)
#define IRIS_FLOW_IOCTL_GET_ISP _IOR(IRIS_FLOW_MAGIC, 5, struct iris_flow_isp_config)
#define IRIS_FLOW_IOCTL_GET_STATUS _IOR(IRIS_FLOW_MAGIC, 6, __u32)
#define IRIS_FLOW_IOCTL_SET_HDR _IOW(IRIS_FLOW_MAGIC, 7, __u32)

#endif /* IRIS_FLOW_UAPI_H */

/* USB camera support */
#define IRIS_FLOW_SOURCE_MIPI 0
#define IRIS_FLOW_SOURCE_USB 1
#define IRIS_FLOW_SOURCE_FILE 2

/* Camera tuning parameters */
struct iris_flow_tuning
{
    __s32 calibration_r;
    __s32 calibration_g;
    __s32 calibration_b;
    __u32 color_temp;
    __u32 exposure_target;
    __u32 focus_position;
    __u32 low_light_threshold;
    __u32 reserved[4];
};

/* Pipeline diagnostics */
struct iris_flow_diagnostics
{
    __u32 frame_count;
    __u32 dropped_frames;
    __u32 latency_spikes;
    __u32 sync_errors;
    __u32 buffer_overruns;
    __u32 init_failures;
    __u64 reserved[4];
};

#define IRIS_FLOW_IOCTL_SET_SOURCE _IOW(IRIS_FLOW_MAGIC, 8, __u32)
#define IRIS_FLOW_IOCTL_SET_TUNING _IOW(IRIS_FLOW_MAGIC, 9, struct iris_flow_tuning)
#define IRIS_FLOW_IOCTL_GET_DIAG _IOR(IRIS_FLOW_MAGIC, 10, struct iris_flow_diagnostics)
