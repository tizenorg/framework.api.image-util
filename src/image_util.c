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

#define LOG_TAG "CAPI_MEDIA_IMAGE_UTIL"
#include <dlog.h>

#include <mm_util_imgp.h>
#include <mm_util_jpeg.h>
#include <image_util.h>
#include <image_util_internal.h>
#include <image_util_private.h>
#include <mm.h>
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_UTIL_STRING_VALID(str)	\
	((str != NULL && strlen(str) > 0) ? true : false)

#define IMAGE_UTIL_SAFE_FREE(src)      { if(src) {free(src); src = NULL;}}

static int _convert_colorspace_tbl[] = {
	MM_UTIL_IMG_FMT_YUV420 , 		/* IMAGE_UTIL_COLORSPACE_YUV420 */
	MM_UTIL_IMG_FMT_YUV422 , 		/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_IMG_FMT_I420,  			/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_IMG_FMT_NV12, 			/* IMAGE_UTIL_COLORSPACE_NV12 */
	MM_UTIL_IMG_FMT_UYVY, 			/* IMAGE_UTIL_COLORSPACE_UYVY */
	MM_UTIL_IMG_FMT_YUYV,				/* IMAGE_UTIL_COLORSPACE_YUYV */
	MM_UTIL_IMG_FMT_RGB565,			/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_IMG_FMT_RGB888, 		/* IMAGE_UTIL_COLORSPACE_RGB888 */
	MM_UTIL_IMG_FMT_ARGB8888, 	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	MM_UTIL_IMG_FMT_BGRA8888, 	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_IMG_FMT_RGBA8888, 	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	MM_UTIL_IMG_FMT_BGRX8888, 	/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	MM_UTIL_JPEG_FMT_NV21, 		/* IMAGE_UTIL_COLORSPACE_NV12 */
	MM_UTIL_JPEG_FMT_NV16, 		/* IMAGE_UTIL_COLORSPACE_NV16 */
	MM_UTIL_JPEG_FMT_NV61, 		/* IMAGE_UTIL_COLORSPACE_NV61 */
};


static int _convert_encode_colorspace_tbl[] = {
	MM_UTIL_JPEG_FMT_YUV420						,	/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_JPEG_FMT_YUV420						,	/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_JPEG_FMT_NV12						,	/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_UYVY */
	MM_UTIL_JPEG_FMT_YUYV						,	/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1											,	/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_JPEG_FMT_RGB888						,	/* IMAGE_UTIL_COLORSPACE_RGB888 */
	MM_UTIL_JPEG_FMT_ARGB8888					,	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	MM_UTIL_JPEG_FMT_BGRA8888					,	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	MM_UTIL_JPEG_FMT_RGBA8888					,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1											,	/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	MM_UTIL_JPEG_FMT_NV21						,	/* IMAGE_UTIL_COLORSPACE_NV21 */
	MM_UTIL_JPEG_FMT_NV16						,	/* IMAGE_UTIL_COLORSPACE_NV16 */
	MM_UTIL_JPEG_FMT_NV61						,	/* IMAGE_UTIL_COLORSPACE_NV61 */
};



static int _convert_image_util_error_code(const char *func, int code){
	int ret = IMAGE_UTIL_ERROR_INVALID_OPERATION;
	char *errorstr = NULL;
	switch(code)
	{
		case MM_ERROR_NONE:
			ret = IMAGE_UTIL_ERROR_NONE;
			errorstr = "ERROR_NONE";
			break;
		case MM_ERROR_IMAGE_FILEOPEN :
		case IMAGE_UTIL_ERROR_NO_SUCH_FILE :
			ret = IMAGE_UTIL_ERROR_NO_SUCH_FILE;
			errorstr = "NO_SUCH_FILE";
			break;

		case MM_ERROR_IMAGE_INTERNAL :
			ret = IMAGE_UTIL_ERROR_INVALID_OPERATION;
			errorstr = "INVALID_OPERATION";
			break;
		case IMAGE_UTIL_ERROR_INVALID_PARAMETER:
		case MM_ERROR_NO_DECODED_DATA:
		case MM_ERROR_IMAGE_INVALID_VALUE:
			ret = IMAGE_UTIL_ERROR_INVALID_PARAMETER;
			errorstr = "INVALID_PARAMETER";
			break;
		case MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT:
		case MM_ERROR_IMAGE_DEVICE_NOT_SUPPORT:
			ret = IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
			errorstr = "NOT_SUPPORTED_FORMAT";
			break;


		default:
			ret = IMAGE_UTIL_ERROR_INVALID_OPERATION;
			errorstr = "INVALID_OPERATION";

	}

	LOGD( "[%s] %s(0x%08x)",func, errorstr, ret);
	return ret;
}

static image_util_error_e _image_util_error_convert(int error)
{
	if(error == MM_ERROR_NONE)
	{
		LOGD("Error None");
		return IMAGE_UTIL_ERROR_NONE;
	}
	else if(error == MM_ERROR_IMAGE_INVALID_VALUE)
	{
		LOGE("INVALID_PARAMETER(0x%08x)", IMAGE_UTIL_ERROR_INVALID_PARAMETER);
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	else if(error == MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT)
	{
		LOGE("NOT_SUPPORTED_FORMAT(0x%08x)", IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT);
		return IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT;
	}

	LOGE("INVALID_OPERATION(0x%08x)", error);
	return IMAGE_UTIL_ERROR_INVALID_OPERATION;
}

static void _image_util_transform_completed_cb(media_packet_h *dst, int error, void *user_data)
{
	int error_value = IMAGE_UTIL_ERROR_NONE;
	image_util_cb_s *_util_cb = (image_util_cb_s *)user_data;

	if((_util_cb != NULL) && (_util_cb->image_processing_completed_cb != NULL))
	{
		error_value = _image_util_error_convert(error);
		_util_cb->image_processing_completed_cb(dst, error_value, _util_cb->user_data);
	}

	return;
}

static int _image_util_create_transform_handle(transformation_s *handle)
{
	int ret = MM_ERROR_NONE;
	MMHandleType image_h;

	ret = mm_util_create(&image_h);

	if(ret == MM_ERROR_NONE)
	{
		handle->image_h = image_h;
	}
	else
	{
		LOGE("Error in mm_util_create");
	}

	return ret;
}

static bool _image_util_check_resolution(int width, int height)
{
	if(width <= 0)
	{
		LOGE("invalid width [%d]", width);
		return false;
	}

	if(height <= 0)
	{
		LOGE("invalid height [%d]", height);
		return false;
	}

	return true;
}

int _image_util_check_transcode_is_completed(transformation_s * handle, bool *is_completed)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if(handle && handle->image_h)
	{
		ret = mm_transform_is_completed(handle->image_h, is_completed);
	}
	else
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	ret = _convert_image_util_error_code(__func__, ret);

	return ret;
}

int image_util_foreach_supported_jpeg_colorspace(image_util_supported_jpeg_colorspace_cb callback, void * user_data)
{
	int i = 0;

	if( callback == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	for( i = sizeof(_convert_encode_colorspace_tbl)/sizeof(int) -1; i >= 0; i--) { /* RGB has higher precedence than YUV */
		if ( _convert_encode_colorspace_tbl[i] != -1)
			if( false == callback(i , user_data) )
				return IMAGE_UTIL_ERROR_NONE;

	}

	return IMAGE_UTIL_ERROR_NONE;
}

int image_util_transform_create(transformation_h *handle)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	LOGD("image_util_create");

	if(handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	transformation_s *_handle = (transformation_s*)calloc(1,sizeof(transformation_s));
	if(_handle == NULL)
	{
		LOGE("OUT_OF_MEMORY(0x%08x)", IMAGE_UTIL_ERROR_OUT_OF_MEMORY);
		return IMAGE_UTIL_ERROR_OUT_OF_MEMORY;
	}

	_handle->colorspace = -1;
	_handle->_util_cb = NULL;
	_handle->image_h = 0;
	_handle->hardware_acceleration = false;
	_handle->set_convert = false;
	_handle->set_resize  = false;
	_handle->set_rotate = false;
	_handle->set_crop  = false;

	ret = _image_util_create_transform_handle(_handle);
	if(ret != MM_ERROR_NONE)
	{
		LOGE("INVALID_OPERATION");
		IMAGE_UTIL_SAFE_FREE(_handle);
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*handle = (transformation_h)_handle;

	return ret;
}

int  image_util_transform_set_hardware_acceleration(transformation_h handle, bool mode)
{
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("Set hardware_acceleration %d", mode);

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(mode == true) {
		LOGE("hardware acceleration is not supported");
		return IMAGE_UTIL_ERROR_NOT_SUPPORTED;
	}

	if(mm_util_set_hardware_acceleration(_handle->image_h, mode) == MM_ERROR_NONE)
	{
		LOGD("Set hardware_acceleration %d", mode);
		_handle->hardware_acceleration = mode;
	} else {
		LOGE("Error - Set hardware_acceleration");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	return IMAGE_UTIL_ERROR_NONE;
}

int image_util_transform_set_colorspace(transformation_h handle, image_util_colorspace_e colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("Set colorspace_convert_info [%d]", colorspace);

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	ret = mm_util_set_colorspace_convert(_handle->image_h, colorspace);
	if(ret != MM_ERROR_NONE)
	{
		LOGE("IMAGE_UTIL_ERROR_INVALID_OPERATION(0x%08x)", IMAGE_UTIL_ERROR_INVALID_OPERATION);
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	_handle->colorspace = colorspace;
	_handle->set_convert = true;

	return ret;
}

int image_util_transform_set_resolution(transformation_h handle, unsigned int width, unsigned int height)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("Set resize_info w[%d] h[%d]", width, height);

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(_handle->set_crop)
	{
		LOGE("Crop and Resize can't do at the same time");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	if(_image_util_check_resolution(width, height)) {
		ret = mm_util_set_resolution(_handle->image_h, width, height);
		if(ret != MM_ERROR_NONE) {
			LOGE("IMAGE_UTIL_ERROR_INVALID_OPERATION(0x%08x)", IMAGE_UTIL_ERROR_INVALID_OPERATION);
			return IMAGE_UTIL_ERROR_INVALID_OPERATION;
		}
		_handle->width = width;
		_handle->height = height;
		_handle->set_resize  = true;
	}
	else
	{
		LOGE("INVALID_PARAMETER");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_util_transform_set_rotation(transformation_h handle, image_util_rotation_e rotation)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("Set rotate_info [%d]", rotation);

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	ret = mm_util_set_rotation(_handle->image_h, rotation);
	if(ret != MM_ERROR_NONE)
	{
		LOGE("IMAGE_UTIL_ERROR_INVALID_OPERATION(0x%08x)", IMAGE_UTIL_ERROR_INVALID_OPERATION);
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}
	_handle->rotation = rotation;
	_handle->set_rotate = true;

	return ret;
}

int image_util_transform_set_crop_area(transformation_h handle, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;
	int dest_width;
	int dest_height;

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(_handle->set_resize)
	{
		LOGE("Crop and Resize can't do at the same time");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	dest_width = end_x - start_x;
	dest_height = end_y - start_y;

	LOGD("Set crop_info x[%d] y[%d] w[%d] h[%d]", start_x, start_y, dest_width, dest_height);

	if(_image_util_check_resolution(dest_width, dest_height))
	{
		ret = mm_util_set_crop_area(_handle->image_h, start_x, start_y, end_x, end_y);
		if(ret != MM_ERROR_NONE) {
			LOGE("IMAGE_UTIL_ERROR_INVALID_OPERATION(0x%08x)", IMAGE_UTIL_ERROR_INVALID_OPERATION);
			return IMAGE_UTIL_ERROR_INVALID_OPERATION;
		}
		_handle->start_x = start_x;
		_handle->start_y = start_y;
		_handle->end_x = end_x;
		_handle->end_y = end_y;
		_handle->set_crop  = true;
	}
	else
	{
		LOGE("INVALID_PARAMETER");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_util_transform_get_colorspace(transformation_h handle, image_util_colorspace_e *colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("Get colorspace_convert_info [%d]", colorspace);

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(!colorspace)
	{
		LOGE("colorspace area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*colorspace = _handle->colorspace;
	return ret;
}

int image_util_transform_get_resolution(transformation_h handle, unsigned int *width, unsigned int *height)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("Set resize_info w[%d] h[%d]", width, height);

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(!width || !height)
	{
		LOGE("resolution area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*width = _handle->width;
	*height = _handle->height;

	return ret;
}

int image_util_transform_get_rotation(transformation_h handle, image_util_rotation_e *rotation)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("Set rotate_info [%d]", rotation);

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(!rotation)
	{
		LOGE("rotation area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*rotation = _handle->rotation;

	return ret;
}

int image_util_transform_get_crop_area(transformation_h handle, unsigned int *start_x, unsigned int *start_y, unsigned int *end_x, unsigned int *end_y)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(_handle->set_resize)
	{
		LOGE("Crop and Resize can't do at the same time");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	if(!start_x || !start_y || !end_x ||!end_y)
	{
		LOGE("crop area parameter error");
		return IMAGE_UTIL_ERROR_INVALID_OPERATION;
	}

	*start_x = _handle->start_x;
	*start_y = _handle->start_y;
	*end_x = _handle->end_x;
	*end_y = _handle->end_y;

	return ret;
}

int image_util_transform_run(transformation_h handle, media_packet_h src, image_util_transform_completed_cb completed_cb, void *user_data)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("image_util_transform");

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(completed_cb)
	{
		_handle->_util_cb = (image_util_cb_s*)calloc(1, sizeof(image_util_cb_s));
		if(!_handle->_util_cb) {
			LOGE("Out of memory");
			return IMAGE_UTIL_ERROR_OUT_OF_MEMORY;
		}

		_handle->_util_cb->user_data = user_data;
		_handle->_util_cb->image_processing_completed_cb = completed_cb;
	}
	else
	{
		LOGE("INVALID_PARAMETER[completed_cb] (0x%08x)", IMAGE_UTIL_ERROR_INVALID_PARAMETER);
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	if(_handle->_util_cb && src && (_handle->set_convert || _handle->set_resize || _handle->set_rotate || _handle->set_crop))
	{
		ret = mm_util_transform(_handle->image_h, src, (mm_util_completed_callback)_image_util_transform_completed_cb, (void *)_handle->_util_cb);
	}
	else
	{
		LOGE("INVALID_PARAMETER[transform] (0x%08x)", IMAGE_UTIL_ERROR_INVALID_PARAMETER);
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}
	return _convert_image_util_error_code(__func__, ret);
}

int image_util_transform_destroy(transformation_h handle)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	transformation_s *_handle = (transformation_s*)handle;

	LOGD("image_util_destroy");

	if(_handle == NULL)
	{
		LOGE("Invalid Handle");
		return IMAGE_UTIL_ERROR_INVALID_PARAMETER;
	}

	ret = mm_util_destroy(_handle->image_h);

	IMAGE_UTIL_SAFE_FREE(_handle->_util_cb);
	IMAGE_UTIL_SAFE_FREE(_handle);

	return ret;
}

int image_util_convert_colorspace( unsigned char * dest , image_util_colorspace_e dest_colorspace , const unsigned char * src ,  int width, int height, image_util_colorspace_e src_colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( dest == NULL || src == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( dest_colorspace < 0 || dest_colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( src_colorspace < 0 || src_colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);


	ret = mm_util_convert_colorspace(src, width, height, _convert_colorspace_tbl[src_colorspace] , dest, _convert_colorspace_tbl[dest_colorspace]);

	return _convert_image_util_error_code(__func__, ret);
}


int image_util_calculate_buffer_size(int width , int height, image_util_colorspace_e colorspace  ,unsigned int *size)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( colorspace < 0 || width <= 0 || height <= 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int) || size == NULL)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);

	ret = mm_util_get_image_size(_convert_colorspace_tbl[colorspace], width, height, size);
	return _convert_image_util_error_code(__func__, ret);
}

int image_util_resize(unsigned char * dest, int *dest_width , int *dest_height, const unsigned char * src, int src_width, int src_height , image_util_colorspace_e colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( dest == NULL || src == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( dest_width == NULL || dest_height == NULL)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);

	if( *dest_width <= 0 || *dest_height <= 0 )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);

	unsigned int dest_w, dest_h;
	dest_w = *dest_width;
	dest_h = *dest_height;
	ret = mm_util_resize_image(src, src_width, src_height, _convert_colorspace_tbl[colorspace], dest,&dest_w, &dest_h);
	if( ret == 0){
		*dest_width = dest_w;
		*dest_height = dest_h;
	}

	return _convert_image_util_error_code(__func__, ret);
}

int image_util_rotate(unsigned char * dest, int *dest_width, int *dest_height, image_util_rotation_e dest_rotation, const unsigned char * src, int src_width, int src_height, image_util_colorspace_e colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( dest == NULL || src == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( dest_rotation < 0 || dest_rotation > IMAGE_UTIL_ROTATION_FLIP_VERT )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( dest_width == NULL || dest_height == NULL)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);

	unsigned int dest_w, dest_h;
	ret = mm_util_rotate_image(src, src_width, src_height, _convert_colorspace_tbl[colorspace], dest,&dest_w, &dest_h, dest_rotation);
	if( ret == 0){
		*dest_width = dest_w;
		*dest_height = dest_h;
	}
	return _convert_image_util_error_code(__func__, ret);
}

int image_util_crop(unsigned char * dest, int x , int y, int* width, int* height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( dest == NULL || src == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( width == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( src_width <= x  || src_height <= y || src_width < x+*width || src_height< y+*height)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);

	unsigned int dest_w, dest_h;
	dest_w = *width;
	dest_h = *height;
	ret = mm_util_crop_image( src, src_width, src_height, _convert_colorspace_tbl[colorspace], x, y, &dest_w, &dest_h, dest);
	if( ret == 0){
		*width = dest_w;
		*height = dest_h;
	}

	return _convert_image_util_error_code(__func__, ret);
}

int image_util_decode_jpeg( const char *path , image_util_colorspace_e colorspace, unsigned char ** image_buffer , int *width , int *height , unsigned int *size)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( path == NULL || image_buffer == NULL || size == NULL)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if(strlen (path) == 0)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_NO_SUCH_FILE);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( _convert_encode_colorspace_tbl[colorspace] == -1 )
		return _convert_image_util_error_code(__func__, MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT);

	mm_util_jpeg_yuv_data decoded = { 0, 0, 0, NULL};

	ret = mm_util_decode_from_jpeg_file(&decoded, (char*)path, _convert_encode_colorspace_tbl[colorspace]);
	if( ret == 0 ){
		*image_buffer = decoded.data;
		if(width)
			*width = decoded.width;
		if(height)
			*height = decoded.height;
		if(size)
			*size = decoded.size;
	}
	return _convert_image_util_error_code(__func__, ret);
}

int image_util_decode_jpeg_from_memory( const unsigned char * jpeg_buffer , int jpeg_size , image_util_colorspace_e colorspace, unsigned char ** image_buffer , int *width , int *height , unsigned int *size)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( jpeg_buffer == NULL || image_buffer == NULL || size == NULL)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( _convert_encode_colorspace_tbl[colorspace] == -1 )
		return _convert_image_util_error_code(__func__, MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT);

	mm_util_jpeg_yuv_data decoded;

	ret = mm_util_decode_from_jpeg_memory(&decoded , jpeg_buffer, jpeg_size, _convert_encode_colorspace_tbl[colorspace] );

	if( ret == 0 ){
		*image_buffer = decoded.data;
		if(width)
			*width = decoded.width;
		if(height)
			*height = decoded.height;
		if(size)
			*size = decoded.size;
	}

	return _convert_image_util_error_code(__func__, ret);
}

int image_util_encode_jpeg( const unsigned char *buffer, int width, int height, image_util_colorspace_e colorspace,  int quality, const char *path)
{
	int ret = IMAGE_UTIL_ERROR_NONE;

	if( path == NULL || buffer == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if(strlen (path) == 0)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_NO_SUCH_FILE);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( _convert_encode_colorspace_tbl[colorspace] == -1 )
		return _convert_image_util_error_code(__func__, MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT);

	ret = mm_util_jpeg_encode_to_file(path, buffer, width, height, _convert_encode_colorspace_tbl[colorspace], quality);
	return _convert_image_util_error_code(__func__, ret);
}

int image_util_encode_jpeg_to_memory( const unsigned char *image_buffer, int width, int height, image_util_colorspace_e colorspace, int quality,  unsigned char** jpeg_buffer, unsigned int *jpeg_size)
{
	int ret = IMAGE_UTIL_ERROR_NONE;
	int isize;

	if( jpeg_buffer == NULL || image_buffer == NULL || jpeg_size == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( _convert_encode_colorspace_tbl[colorspace] == -1 )
		return _convert_image_util_error_code(__func__, MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT);

	ret = mm_util_jpeg_encode_to_memory( (void**)jpeg_buffer, &isize, image_buffer, width, height, _convert_encode_colorspace_tbl[colorspace] , quality );
	if( ret == 0 )
		*jpeg_size = isize;

	return _convert_image_util_error_code(__func__, ret);
}
