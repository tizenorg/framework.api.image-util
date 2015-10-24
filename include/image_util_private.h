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

#ifndef __TIZEN_MEDIA_IMAGE_UTIL_PRIVATE_H__
#define __TIZEN_MEDIA_IMAGE_UTIL_PRIVATE_H__

#include <image_util_type.h>
#include <dlog.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_MEDIA_IMAGE_UTIL"

#define FONT_COLOR_RESET    "\033[0m"
#define FONT_COLOR_RED      "\033[31m"

#define image_util_debug(fmt, arg...) do { \
		LOGD(FONT_COLOR_RESET""fmt"", ##arg);     \
	} while (0)

#define image_util_error(fmt, arg...) do { \
		LOGE(FONT_COLOR_RED""fmt"", ##arg);     \
	} while (0)

#define image_util_retm_if(expr, fmt, arg...) do { \
		if(expr) { \
			LOGE(FONT_COLOR_RED""fmt"", ##arg);     \
			return; \
		} \
	} while (0)

#define image_util_retvm_if(expr, val, fmt, arg...) do { \
		if(expr) { \
			LOGE(FONT_COLOR_RED""fmt"", ##arg);     \
			return (val); \
		} \
	} while (0)

typedef struct {
	void *user_data;
	media_packet_h dst;
	image_util_transform_completed_cb image_processing_completed_cb;
} image_util_cb_s;

typedef struct {
	media_packet_h src;
	media_packet_h dst;
	image_util_colorspace_e colorspace;
	image_util_rotation_e dest_rotation;
	bool hardware_acceleration;
	MMHandleType image_h;
	image_util_cb_s *_util_cb;
	unsigned int width;
	unsigned int height;
	image_util_rotation_e rotation;
	unsigned int start_x;
	unsigned int start_y;
	unsigned int end_x;
	unsigned int end_y;
	bool set_convert;
	bool set_resize;
	bool set_rotate;
	bool set_crop;
} transformation_s;


/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_IMAGE_UTIL_PRIVATE_H__ */
