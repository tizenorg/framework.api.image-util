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

#include <image_util.h>

GMainLoop *g_loop = NULL;

bool test_transform_completed_cb(media_packet_h *dst, image_util_error_e error, void *user_data)
{
	transformation_s *transform = (transformation_s *)user_data;
	printf("test_transform_completed_cb============= [%d]\n", error);

	if(error == MM_ERROR_NONE) {
		printf("completed");
		*dst = transform->dst;
		if(g_loop)
			g_main_loop_quit(g_loop);
	} else {
		printf("[ERROR] complete cb");
		GThread * destroy_thread = g_thread_new(NULL, image_util_destroy, transform);
		g_thread_unref(destroy_thread);
	}

	return TRUE;
}

static int
create_media_packet(media_format_h *packet)
{
	int ret = 0;
	media_format_h fmt;
	if(media_format_create(&fmt) == MEDIA_FORMAT_ERROR_NONE) {
		if(media_format_set_video_mime(fmt, MEDIA_FORMAT_I420) != MEDIA_FORMAT_ERROR_NONE) {
			media_format_unref(fmt);
			printf("[Error] Set - video mime");
			return MM_ERROR_IMAGE_INVALID_VALUE;
		}

		if(media_format_set_video_width(fmt, 320) != MEDIA_FORMAT_ERROR_NONE) {
			media_format_unref(fmt);
			printf("[Error] Set - video width");
			return MM_ERROR_IMAGE_INVALID_VALUE;
		}

		if(media_format_set_video_height(fmt, 240) != MEDIA_FORMAT_ERROR_NONE) {
			media_format_unref(fmt);
			printf("[Error] Set - video height");
			return MM_ERROR_IMAGE_INVALID_VALUE;
		}

		if(media_format_set_video_avg_bps(fmt, 2000000) != MEDIA_FORMAT_ERROR_NONE) {
			media_format_unref(fmt);
			printf("[Error] Set - video avg bps");
			return MM_ERROR_IMAGE_INVALID_VALUE;
		}

		if(media_format_set_video_max_bps(fmt, 15000000) != MEDIA_FORMAT_ERROR_NONE) {
			media_format_unref(fmt);
			printf("[Error] Set - video max bps");
			return MM_ERROR_IMAGE_INVALID_VALUE;
		}

		printf("media_format_set_video_info success! w:320, h:240, MEDIA_FORMAT_I420\n");
	}
	else {
		printf("media_format_create failed...");
	}

	ret = media_packet_create_alloc(fmt, (media_packet_finalize_cb)transform_completed_cb, NULL, packet);
	if(ret == MM_ERROR_NONE) {
		printf("Success - media_packet_create_alloc");
	} else {
		media_format_unref(fmt);
		printf("ERROR - media_packet_create_alloc");
		return ret;
	}
	media_format_unref(fmt);
	return ret;
}

int main(int argc, char ** argv)
{
	transformation_h handle = NULL;
	media_packet_h src;
	int ret = IMAGE_UTIL_ERROR_NONE;

	ret = image_util_create(&handle);
	if(ret != IMAGE_UTIL_ERROR_NONE)
	{
		printf("[%d]error image_util_create [%d]\n", __LINE__, ret);
	}

	unsigned int width = 0;
	unsigned int height = 0;
	int colorspace = 0;
	mm_util_img_rotate_type angle;
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	bool hardware_acceleration = FALSE;

	ret = create_media_packet(&src);
	if(ret == MM_ERROR_NONE) {
		printf("Success - Create_media_packet");
	} else {
		printf("ERROR - Create_media_packet");
		return ret;
	}

	ret = image_util_transform_set_hardware_acceleration(handle, hardware_acceleration);
	if(ret != IMAGE_UTIL_ERROR_NONE)
	{
		printf("[%d]error image_util_set condition [%d]\n", __LINE__, ret);
		return 0;
	}

	if (!strcmp("convert", argv[1])) {
		colorspace = atoi(argv[2]);

		ret = image_util_transform_set_colorspace(handle, colorspace);
		if(ret != IMAGE_UTIL_ERROR_NONE)
		{
			printf("[%d]error image_util_set colorspace [%d]\n", __LINE__, ret);
			return 0;
		}
	}

	if (!strcmp("resize", argv[1])) {
		width = atoi(argv[2]);
		height = atoi(argv[3]);

		ret = image_util_transform_set_resolution(handle, width, height);
		if(ret != IMAGE_UTIL_ERROR_NONE)
		{
			printf("[%d]error image_util_set resolution [%d]\n", __LINE__, ret);
			return 0;
		}
	}

	if (!strcmp("rotate", argv[1])) {
		angle = atoi(argv[2]);

		ret = image_util_transform_set_rotation(handle, angle);
		if(ret != IMAGE_UTIL_ERROR_NONE)
		{
			printf("[%d]error image_util_set colorspace [%d]\n", __LINE__, ret);
			return 0;
		}
	}

	if (!strcmp("crop", argv[1])) {
		start_x = atoi(argv[2]);
		start_y = atoi(argv[3]);
		end_x atoi(argv[4]);
		end_y atoi(argv[5]);

		ret = image_util_transform_set_crop_area(handle, start_x, start_y, end_x, end_y);
		if(ret != IMAGE_UTIL_ERROR_NONE)
		{
			printf("[%d]error image_util_set colorspace [%d]\n", __LINE__, ret);
			return 0;
		}
	}

	ret = image_util_transform_run(handle, src, test_transform_completed_cb, handle);
	if(ret != IMAGE_UTIL_ERROR_NONE)
	{
		printf("[%d]error image_util_transform [%d]\n", __LINE__, ret);
	}

	if(ret != IMAGE_UTIL_ERROR_NONE)
		goto Exit;

	g_loop = g_main_loop_new(NULL, FALSE);

	g_main_loop_run(g_loop);
	g_main_loop_unref(g_loop);

Exit:
	ret = image_util_destroy(handle);
	if(ret != IMAGE_UTIL_ERROR_NONE)
	{
		printf("[%d]error image_util_destroy [%d]\n", __LINE__, ret);
	}
	else
	{
		printf("[%d]Success image_util_destroy [%d]\n", __LINE__, ret);
	}

	if(_util_s != NULL)
		free(_util_s);

	return 0;
}
