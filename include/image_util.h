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




#ifndef __TIZEN_MEDIA_IMAGE_UTIL_H__
#define __TIZEN_MEDIA_IMAGE_UTIL_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define IMAGE_UTIL_ERROR_CLASS          TIZEN_ERROR_MULTIMEDIA_CLASS | 0x70

/**
 * @file image_util.h
 * @brief This file contains the image util API
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_MODULE
 * @{
 */


typedef enum
{
    IMAGE_UTIL_ERROR_NONE =              TIZEN_ERROR_NONE,													/**< Successful */
    IMAGE_UTIL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,  	/**< Invalid parameter */
    IMAGE_UTIL_ERROR_OUT_OF_MEMORY =     TIZEN_ERROR_OUT_OF_MEMORY,      			/**< Out of memory */
    IMAGE_UTIL_ERROR_NO_SUCH_FILE  = TIZEN_ERROR_NO_SUCH_FILE, 							/**< No such file */
    IMAGE_UTIL_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,  		/**< Internal error */
    IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT = TIZEN_ERROR_NOT_SUPPORT_API /**< Not supported format */
} image_util_error_e;




/**
 * @brief Enumerations of colorspace
 */
typedef enum
{
	IMAGE_UTIL_COLORSPACE_YV12, 				/**< YV12 - YCrCb planar format */
	IMAGE_UTIL_COLORSPACE_YUV422, 			/**< YUV422 - planer */
	IMAGE_UTIL_COLORSPACE_I420, 				/**< YUV420 - planer */
	IMAGE_UTIL_COLORSPACE_NV12, 				/**< NV12- planer */

	IMAGE_UTIL_COLORSPACE_UYVY, 				/**< UYVY - packed */
	IMAGE_UTIL_COLORSPACE_YUYV, 				/**< YUYV - packed */

	IMAGE_UTIL_COLORSPACE_RGB565, 			/**< RGB565, high-byte is Blue */
	IMAGE_UTIL_COLORSPACE_RGB888, 			/**< RGB888, high-byte is Blue */
	IMAGE_UTIL_COLORSPACE_ARGB8888, 	/**< ARGB8888, high-byte is Blue */

	IMAGE_UTIL_COLORSPACE_BGRA8888, 	/**< BGRA8888, high-byte is Alpha */
	IMAGE_UTIL_COLORSPACE_RGBA8888, 	/**< RGBA8888, high-byte is Alpha */
	IMAGE_UTIL_COLORSPACE_BGRX8888, 		/**< BGRX8888, high-byte is X */
	IMAGE_UTIL_COLORSPACE_NV21, 		/**< NV12- planer */
	IMAGE_UTIL_COLORSPACE_NV16, 		/**< NV16- planer */
	IMAGE_UTIL_COLORSPACE_NV61, 		/**< NV61- planer */
}image_util_colorspace_e;


/**
 * @brief Enumerations of rotation
 */
typedef enum
{
	IMAGE_UTIL_ROTATION_NONE = 0, 			/**< None */
    IMAGE_UTIL_ROTATION_90 = 1,              	/**< Rotation 90 degree */
    IMAGE_UTIL_ROTATION_180,             		/**< Rotation 180 degree */
    IMAGE_UTIL_ROTATION_270,             		/**< Rotation 270 degree */
    IMAGE_UTIL_ROTATION_FLIP_HORZ,       /**< Flip horizontal */
    IMAGE_UTIL_ROTATION_FLIP_VERT,       /**< Flip vertical */
} image_util_rotation_e;




/**
 * @brief	Called once for each supported JPEG encode/decode colorspace.
 *
 * @param[in] colorspace         The colorspace
 * @param[in] user_data     The user data passed from the foreach function
 *
 * @return	@c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre		image_util_foreach_supported_jpeg_colorspace() will invokes this callback.
 *
 * @see	image_util_foreach_supported_jpeg_colorspace()
 * @see	image_util_encode_jpeg()
 * @see	image_util_encode_jpeg_to_memory()
 * @see	image_util_decode_jpeg()
 * @see	image_util_decode_jpeg_from_memory()
 */
typedef bool (*image_util_supported_jpeg_colorspace_cb)( image_util_colorspace_e colorspace , void * user_data);

/**
 * @brief Retrieves all supported JPEG encoding/decoding colorspace by invoking a callback function once for each one.
 *
 * @param[in] 	callback    The callback function to invoke
 * @param[in] 	user_data	The user data to be passed to the callback function
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @post	This function invokes image_util_supported_jpeg_colorspace_cb() repeatedly to retrieve each supported jpeg encoding/decoding colorspace
 *
 * @see	image_util_supported_jpeg_colorspace_cb()
 * @see	image_util_encode_jpeg()
 * @see	image_util_encode_jpeg_to_memory()
 * @see	image_util_decode_jpeg()
 * @see	image_util_decode_jpeg_from_memory()
 */
int image_util_foreach_supported_jpeg_colorspace(image_util_supported_jpeg_colorspace_cb callback, void * user_data);

/**
 * @brief Convert the image's colorspace
 *
 * @remarks To get @a dest buffer size uses image_util_calculate_buffer_size()
 *
 * @param[in/out]	dest    The image buffer for result. Must be allocated by you
 * @param[in]	dest_colorspace	The colorspace to be converted
 * @param[in]	src	The source image buffer
 * @param[in]	width	The width of source image
 * @param[in]	height	The height of source image
 * @param[in]	src_colorspace	The colorspace of source image buffer
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_calculate_buffer_size()
 */
int image_util_convert_colorspace( unsigned char * dest , image_util_colorspace_e dest_colorspace , const unsigned char * src ,  int width, int height, image_util_colorspace_e src_colorspace);

/**
 * @brief Calculates the size of image buffer for the specified resolution and colorspace
 *
 * @param[in]	width	The image width
 * @param[in]	height	The image height
 * @param[in]	colorspace	The image colorspace
 * @param[out]	size	The Calculated buffer size
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see	image_util_convert_colorspace()
 * @see	image_util_resize()
 * @see	image_util_rotation()
 */
int image_util_calculate_buffer_size(int width , int height, image_util_colorspace_e colorspace  , unsigned int *size);

/**
 * @brief Resize the image to with the specified destination width and height
 *
 * @remarks Because padding of YUV format, the destination image size can be adjusted.\n
 *
 * @param[in/out]	dest	The image buffer for result. Must be allocated by you
 * @param[in/out]	dest_width	The image width to resize, and resized width
 * @param[in/out]	dest_height	The image height to resize, and resized height
 * @param[in]	src				The image buffer for origin image
 * @param[in]	src_width		The origin image width
 * @param[in]	src_height		The origin image height
 * @param[in]	colorspace		The image colorspace
 *
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_calculate_buffer_size()
 */
int image_util_resize(unsigned char * dest, int *dest_width , int *dest_height, const unsigned char * src, int src_width, int src_height , image_util_colorspace_e colorspace);

/**
 * @brief Rotate the image to with the specified angle in degrees.
 *
 * @remarks Because padding of YUV format, the destination image size can be adjusted.\n
 *  Rotations are supported only in these colorspaces\n
 * #IMAGE_UTIL_COLORSPACE_YV12 \n
 * #IMAGE_UTIL_COLORSPACE_I420 \n
 * #IMAGE_UTIL_COLORSPACE_NV12 \n
 * #IMAGE_UTIL_COLORSPACE_RGB888 \n
 *
 * @param[in/out]	dest	The image buffer for result. Must be allocated by you
 * @param[out]	dest_width The rotated image width
 * @param[out]	dest_height The rotated image height
 * @param[in]	dest_rotation	The angle to rotate
 * @param[in]	src				The image buffer for origin image
 * @param[in]	src_width		The origin image width
 * @param[in]	src_height		The origin image height
 * @param[in]	colorspace		The image colorspace
 *
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_calculate_buffer_size()
 */
int image_util_rotate(unsigned char * dest, int *dest_width, int *dest_height, image_util_rotation_e dest_rotation, const unsigned char * src, int src_width, int src_height, image_util_colorspace_e colorspace);

/**
 * @brief Crop the image to with the specified point and dimension
 *
 * @remarks Because padding of YUV format, the destination image size can be adjusted.\n
 *  Crop is supported only in these colorspaces\n
 * #IMAGE_UTIL_COLORSPACE_YV12 \n
 * #IMAGE_UTIL_COLORSPACE_I420 \n
 * #IMAGE_UTIL_COLORSPACE_RGB888 \n
 * #IMAGE_UTIL_COLORSPACE_RGB565 \n
 * #IMAGE_UTIL_COLORSPACE_ARGB8888\n
 * #IMAGE_UTIL_COLORSPACE_BGRA8888\n
 * #IMAGE_UTIL_COLORSPACE_RGBA8888\n
 * #IMAGE_UTIL_COLORSPACE_BGRX8888\n
 *
 * @param[in/out]	dest	The image buffer for result. Must be allocated by you
 * @param[in]	x The starting x-axis of crop
 * @param[in]	y The starting y-axis of crop
 * @param[in/out]	width  The image width to crop, and cropped width
 * @param[in/out]	height  The image height to crop, and cropped height
 * @param[in]	src				The image buffer for origin image
 * @param[in]	src_width		The origin image width
 * @param[in]	src_height		The origin image height
 * @param[in]	colorspace		The image colorspace
 *
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_calculate_buffer_size()
 */
int image_util_crop(unsigned char * dest, int x , int y, int* width, int *height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace);




/**
 * @brief Decodes jpeg image to the buffer
 *
 * @remarks @a image_buffer must be released with free() by you.
 *
 * @param[in]	path	The image file path
 * @param[in]	colorspace	The decoded image colorspace
 * @param[out]	image_buffer	The image buffer for decoded image
 * @param[out]	width	The image width
 * @param[out]	height	The image height
 * @param[out]	size		The image buffer size
 *
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
 * @retval    #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_supported_jpeg_colorspace_cb()
 * @see	image_util_decode_jpeg_from_memory()
 * @see	image_util_foreach_supported_jpeg_colorspace()
 */
int image_util_decode_jpeg( const char *path , image_util_colorspace_e colorspace, unsigned char ** image_buffer , int *width , int *height , unsigned int *size);

/**
 * @brief Decodes jpeg image(on memory) to the buffer
 *
 * @remarks @a image_buffer must be released with free() by you.
 *
 * @param[in]	jpeg_buffer	The jpeg image buffer
 * @param[in]	jpeg_size		The jpeg image buffer size
 * @param[in]	colorspace	The decoded image colorspace
 * @param[out]	image_buffer	The image buffer for decoded image
 * @param[out]	width	The image width
 * @param[out]	height	The image height
 * @param[out]	size		The image buffer size
 *
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
 * @retval    #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_supported_jpeg_colorspace_cb()
 * @see	image_util_decode_jpeg()
 * @see	image_util_foreach_supported_jpeg_colorspace()
 */
int image_util_decode_jpeg_from_memory( const unsigned char * jpeg_buffer , int jpeg_size , image_util_colorspace_e colorspace, unsigned char ** image_buffer , int *width , int *height , unsigned int *size);

/**
 * @brief Encodes image to the jpeg image
 *
 * @param[in]	buffer	The origin image buffer
 * @param[in]	width	The origin image width
 * @param[in]	height	The origin image height
 * @param[in]	colorspace	The origin image colorspace
 * @param[in]	quality The quality for JPEG image encoding (1 ~ 100)
 * @param[in]	path	The file path to be created
 *
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_NO_SUCH_FILE no such file
 * @retval    #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_supported_jpeg_colorspace_cb()
 * @see image_util_foreach_supported_jpeg_colorspace()
 * @see image_util_encode_jpeg_to_memory()
 */
int image_util_encode_jpeg( const unsigned char *buffer, int width, int height, image_util_colorspace_e colorspace,  int quality, const char *path);

/**
 * @brief Encodes image to the jpeg image
 *
 * @remarks @a jpeg_buffer must be released with free() by you.
 *
 * @param[in]	image_buffer	The origin image buffer
 * @param[in]	width	The image width
 * @param[in]	height	The image height
 * @param[in]	colorspace	The origin image colorspace
 * @param[in]	quality The quality for JPEG image encoding (1 ~ 100)
 * @param[out]	jpeg_buffer	The created jpeg image buffer.  The buffer is created by frameworks
 * @param[out]	jpeg_size		The created jpeg image buffer size
 *
 * @return	  0 on success, otherwise a negative error value.
 * @retval    #IMAGE_UTIL_ERROR_NONE Successful
 * @retval    #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	 #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
 * @retval    #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
 * @retval	 #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see image_util_supported_jpeg_colorspace_cb()
 * @see image_util_foreach_supported_jpeg_colorspace()
 * @see image_util_encode_jpeg()
 */
int image_util_encode_jpeg_to_memory(const unsigned char *image_buffer, int width, int height, image_util_colorspace_e colorspace, int quality,  unsigned char** jpeg_buffer, unsigned int *jpeg_size);




/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_IMAGE_UTIL_H__ */
