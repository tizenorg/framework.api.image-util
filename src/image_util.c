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
#include <mm.h>
#include <stdio.h>

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
};


static int _convert_encode_colorspace_tbl[] = {
	MM_UTIL_JPEG_FMT_YUV420						 ,	/* IMAGE_UTIL_COLORSPACE_YUV420 */
	-1											 ,	/* IMAGE_UTIL_COLORSPACE_YUV422 */
	MM_UTIL_JPEG_FMT_YUV420						 ,	/* IMAGE_UTIL_COLORSPACE_I420 */
	MM_UTIL_JPEG_FMT_NV12						 ,	/* IMAGE_UTIL_COLORSPACE_NV12 */
	-1											 ,	/* IMAGE_UTIL_COLORSPACE_UYVY */
	MM_UTIL_JPEG_FMT_YUYV						 ,	/* IMAGE_UTIL_COLORSPACE_YUYV */
	-1											 ,	/* IMAGE_UTIL_COLORSPACE_RGB565 */
	MM_UTIL_JPEG_FMT_RGB888						 ,	/* IMAGE_UTIL_COLORSPACE_RGB888 */
	-1											 ,	/* IMAGE_UTIL_COLORSPACE_ARGB8888 */
	-1											 ,	/* IMAGE_UTIL_COLORSPACE_BGRA8888 */
	-1											 ,	/* IMAGE_UTIL_COLORSPACE_RGBA8888 */
	-1											 ,	/* IMAGE_UTIL_COLORSPACE_BGRX8888 */
	MM_UTIL_JPEG_FMT_NV21						 ,	/* IMAGE_UTIL_COLORSPACE_NV21 */
	MM_UTIL_JPEG_FMT_NV16						 ,	/* IMAGE_UTIL_COLORSPACE_NV16 */
	MM_UTIL_JPEG_FMT_NV61						 ,	/* IMAGE_UTIL_COLORSPACE_NV61 */
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

	LOGE( "[%s] %s(0x%08x)",func, errorstr, ret);
	return ret;
}


int image_util_foreach_supported_jpeg_colorspace(image_util_supported_jpeg_colorspace_cb callback, void * user_data){
	if( callback == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	int i;
	for( i = sizeof(_convert_encode_colorspace_tbl)/sizeof(int) -1; i >= 0; i--) { /* RGB has higher precedence than YUV */
		if ( _convert_encode_colorspace_tbl[i] != -1)
			if( false == callback(i , user_data) )
				return IMAGE_UTIL_ERROR_NONE;

	}
	return IMAGE_UTIL_ERROR_NONE;
}

int image_util_convert_colorspace( unsigned char * dest , image_util_colorspace_e dest_colorspace , const unsigned char * src ,  int width, int height, image_util_colorspace_e src_colorspace){

	int ret;
	if( dest == NULL || src == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( dest_colorspace < 0 || dest_colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( src_colorspace < 0 || src_colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);


	ret = mm_util_convert_colorspace( src , width,height, _convert_colorspace_tbl[src_colorspace] , dest, _convert_colorspace_tbl[dest_colorspace] );

	return _convert_image_util_error_code(__func__, ret);
}


int image_util_calculate_buffer_size(int width , int height, image_util_colorspace_e colorspace  ,unsigned int *size){
	int ret;
	if( colorspace < 0 || width <= 0 || height <= 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int) || size == NULL)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);

	ret = mm_util_get_image_size(_convert_colorspace_tbl[colorspace], width, height, size);
	return _convert_image_util_error_code(__func__, ret);
}

int image_util_resize(unsigned char * dest, int *dest_width , int *dest_height, const unsigned char * src, int src_width, int src_height , image_util_colorspace_e colorspace){
	int ret;
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

int image_util_rotate(unsigned char * dest, int *dest_width, int *dest_height, image_util_rotation_e dest_rotation, const unsigned char * src, int src_width, int src_height, image_util_colorspace_e colorspace){
	int ret;
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

int image_util_crop(unsigned char * dest, int x , int y, int* width, int* height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace){
	int ret;
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

int image_util_decode_jpeg( const char *path , image_util_colorspace_e colorspace, unsigned char ** image_buffer , int *width , int *height , unsigned int *size){
	int ret;

	if( path == NULL || image_buffer == NULL || size == NULL)
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( _convert_encode_colorspace_tbl[colorspace] == -1 )
		return _convert_image_util_error_code(__func__, MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT);

	mm_util_jpeg_yuv_data decoded;

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

int image_util_decode_jpeg_from_memory( const unsigned char * jpeg_buffer , int jpeg_size , image_util_colorspace_e colorspace, unsigned char ** image_buffer , int *width , int *height , unsigned int *size){
	int ret;
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

int image_util_encode_jpeg( const unsigned char *buffer, int width, int height, image_util_colorspace_e colorspace,  int quality, const char *path){
	int ret;
	if( path == NULL || buffer == NULL )
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( colorspace < 0 || colorspace >= sizeof(_convert_colorspace_tbl)/sizeof(int))
		return _convert_image_util_error_code(__func__, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
	if( _convert_encode_colorspace_tbl[colorspace] == -1 )
		return _convert_image_util_error_code(__func__, MM_ERROR_IMAGE_NOT_SUPPORT_FORMAT);

	ret = mm_util_jpeg_encode_to_file(path, buffer, width, height, _convert_encode_colorspace_tbl[colorspace], quality);
	return _convert_image_util_error_code(__func__, ret);
}

int image_util_encode_jpeg_to_memory( const unsigned char *image_buffer, int width, int height, image_util_colorspace_e colorspace, int quality,  unsigned char** jpeg_buffer, unsigned int *jpeg_size){
	int ret;
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

