/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __TIZEN_MEDIA_IMAGE_UTIL_TYPE_H__
#define __TIZEN_MEDIA_IMAGE_UTIL_TYPE_H__

#include <tizen.h>
#include <media_packet.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define IMAGE_UTIL_ERROR_CLASS          TIZEN_ERROR_IMAGE_UTIL

/**
 * @file image_util_type.h
 * @brief This file contains the image util API
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_MODULE
 * @{
 */

/**
 * @brief Enumeration for error.
 * @since_tizen 2.3
 */
typedef enum
{
	IMAGE_UTIL_ERROR_NONE =              TIZEN_ERROR_NONE,                /**< Successful */
	IMAGE_UTIL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	IMAGE_UTIL_ERROR_OUT_OF_MEMORY =     TIZEN_ERROR_OUT_OF_MEMORY,       /**< Out of memory */
	IMAGE_UTIL_ERROR_NO_SUCH_FILE  = TIZEN_ERROR_NO_SUCH_FILE,            /**< No such file */
	IMAGE_UTIL_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,   /**< Internal error */
	IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT = TIZEN_ERROR_IMAGE_UTIL | 0x01,   /**< Not supported format */
	IMAGE_UTIL_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,   /**< Permission denied  */
	IMAGE_UTIL_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED          /**< Not supported */
}
image_util_error_e;

/**
 * @brief Enumeration for colorspace.
 * @since_tizen 2.3
 */
typedef enum {
    IMAGE_UTIL_COLORSPACE_YV12,     /**< YV12 - YCrCb planar format */
    IMAGE_UTIL_COLORSPACE_YUV422,   /**< YUV422 - planar */
    IMAGE_UTIL_COLORSPACE_I420,     /**< YUV420 - planar */
    IMAGE_UTIL_COLORSPACE_NV12,     /**< NV12- planar */

    IMAGE_UTIL_COLORSPACE_UYVY,     /**< UYVY - packed */
    IMAGE_UTIL_COLORSPACE_YUYV,     /**< YUYV - packed */

    IMAGE_UTIL_COLORSPACE_RGB565,   /**< RGB565, high-byte is Blue */
    IMAGE_UTIL_COLORSPACE_RGB888,   /**< RGB888, high-byte is Blue */
    IMAGE_UTIL_COLORSPACE_ARGB8888, /**< ARGB8888, high-byte is Blue */

    IMAGE_UTIL_COLORSPACE_BGRA8888, /**< BGRA8888, high-byte is Alpha */
    IMAGE_UTIL_COLORSPACE_RGBA8888, /**< RGBA8888, high-byte is Alpha */
    IMAGE_UTIL_COLORSPACE_BGRX8888, /**< BGRX8888, high-byte is X */
    IMAGE_UTIL_COLORSPACE_NV21,     /**< NV12- planar */
    IMAGE_UTIL_COLORSPACE_NV16,     /**< NV16- planar */
    IMAGE_UTIL_COLORSPACE_NV61,     /**< NV61- planar */
} image_util_colorspace_e;

/**
 * @brief Enumeration for scale decoding.
 * @since_tizen 2.4
 */
typedef enum {
    IMAGE_UTIL_DOWNSCALE_1_1, /** 1/1 downscale */
    IMAGE_UTIL_DOWNSCALE_1_2,	/** 1/2 downscale */
    IMAGE_UTIL_DOWNSCALE_1_4,	/** 1/4 downscale */
    IMAGE_UTIL_DOWNSCALE_1_8,	/** 1/8 downscale */
} image_util_scale_e;

/**
 * @brief Enumeration for rotation.
 * @since_tizen 2.3
 */
typedef enum {
    IMAGE_UTIL_ROTATION_NONE = 0,	/**< None */
    IMAGE_UTIL_ROTATION_90,			/**< Rotation 90 degree */
    IMAGE_UTIL_ROTATION_180,			/**< Rotation 180 degree */
    IMAGE_UTIL_ROTATION_270,			/**< Rotation 270 degree */
    IMAGE_UTIL_ROTATION_FLIP_HORZ,	/**< Flip horizontal */
    IMAGE_UTIL_ROTATION_FLIP_VERT,	/**< Flip vertical */
} image_util_rotation_e;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_MODULE
* @brief Image util handle.
* @since_tizen 2.3
*/
typedef struct transformation_s *transformation_h;

/**
* @ingroup CAPI_MEDIA_IMAGE_UTIL_MODULE
* @brief Called when transform is finished just before returning the output.
* @since_tizen 2.3
*
* @remarks You must release the @a dst result using media_packet_destroy().
*
* @param[in] error_code The error code of image util transfrom
* @param[in, out] dst The result buffer of image util transform
* @param[in] user_data The user data passed from the callback registration function
* @pre image_util_transform_run() will invoke this function.
*/
typedef void(*image_util_transform_completed_cb)(media_packet_h *dst, int error_code, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_IMAGE_UTIL_TYPE_H__ */
