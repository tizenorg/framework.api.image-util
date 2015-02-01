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

#ifndef __TIZEN_MEDIA_IMAGE_UTIL_INTERNAL_H__
#define __TIZEN_MEDIA_IMAGE_UTIL_INTERNAL_H__

#include <image_util_type.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_MODULE
 * @{
 */

/**
* @internal
* @brief Converts the image's colorspace.
* @since_tizen 2.3
*
* @remarks You must get the @a dest buffer size using image_util_transform_calculate_buffer_size().
*
* @param[in,out] dest The image buffer for result \n
*                     Must be allocated by the user
* @param[in] dest_colorspace The colorspace to be converted
* @param[in] src The source image buffer
* @param[in] width The width of the source image
* @param[in] height The height of the source image
* @param[in] src_colorspace The colorspace of the source image buffer
*
* @return %c 0 on success
*            otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_convert_colorspace( unsigned char * dest , image_util_colorspace_e dest_colorspace , const unsigned char * src ,  int width, int height, image_util_colorspace_e src_colorspace);

/**
* @internal
* @brief Resizes the image to the specified destination width and height.
* @since_tizen 2.3
*
* @remarks Because of YUV format padding, the destination image size can be adjusted.
*
* @param[in,out] dest The image buffer for result \n
*                     Must be allocated by the user.
* @param[in,out] dest_width The image width to resize, and resized width
* @param[in,out] dest_height The image height to resize, and resized height
* @param[in] src The image buffer for the original image
* @param[in] src_width The original image width
* @param[in] src_height The original image height
* @param[in] colorspace The image colorspace
*
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
*/
int image_util_resize(unsigned char * dest, int *dest_width , int *dest_height, const unsigned char * src, int src_width, int src_height , image_util_colorspace_e colorspace);

/**
* @internal
* @brief Rotates the image to the specified angle given in degrees.
* @since_tizen 2.3
*
* @remarks Because of YUV format padding, the destination image size can be adjusted.
* Rotations are supported only in these color spaces\n
* #IMAGE_UTIL_COLORSPACE_YV12\n
* #IMAGE_UTIL_COLORSPACE_I420\n
* #IMAGE_UTIL_COLORSPACE_NV12\n
* #IMAGE_UTIL_COLORSPACE_RGB888\n
* #IMAGE_UTIL_COLORSPACE_RGB565\n
* #IMAGE_UTIL_COLORSPACE_ARGB8888\n
* #IMAGE_UTIL_COLORSPACE_BGRA8888\n
* #IMAGE_UTIL_COLORSPACE_RGBA8888\n
* #IMAGE_UTIL_COLORSPACE_BGRX8888\n.
*
* @param[in,out] dest The image buffer for result \n
*                     Must be allocated by the user.
* @param[out] dest_width The rotated image width
* @param[out] dest_height The rotated image height
* @param[in] dest_rotation The angle to rotate
* @param[in] src The image buffer for the original image
* @param[in] src_width The original image width
* @param[in] src_height The original image height
* @param[in] colorspace The image colorspace
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
*/
int image_util_rotate(unsigned char * dest, int *dest_width, int *dest_height, image_util_rotation_e dest_rotation, const unsigned char * src, int src_width, int src_height, image_util_colorspace_e colorspace);

/**
* @internal
* @brief Crops the image to the specified point and dimension.
* @since_tizen 2.3
*
* @remarks Because of YUV format padding, the destination image size can be adjusted.
* Crop is supported only in these colorspaces\n
* #IMAGE_UTIL_COLORSPACE_YV12 \n
* #IMAGE_UTIL_COLORSPACE_I420 \n
* #IMAGE_UTIL_COLORSPACE_RGB888 \n
* #IMAGE_UTIL_COLORSPACE_RGB565 \n
* #IMAGE_UTIL_COLORSPACE_ARGB8888\n
* #IMAGE_UTIL_COLORSPACE_BGRA8888\n
* #IMAGE_UTIL_COLORSPACE_RGBA8888\n
* #IMAGE_UTIL_COLORSPACE_BGRX8888\n.
*
* @param[in,out] dest The image buffer for result. Must be allocated by you
* @param[in] x The starting x-axis of crop
* @param[in] y The starting y-axis of crop
* @param[in/out] width  The image width to crop, and cropped width
* @param[in/out] height  The image height to crop, and cropped height
* @param[in] src The image buffer for original image
* @param[in] src_width The original image width
* @param[in] src_height The original image height
* @param[in] colorspace The image colorspace
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_calculate_buffer_size()
*/
int image_util_crop(unsigned char * dest, int x , int y, int* width, int *height, const unsigned char *src, int src_width, int src_height, image_util_colorspace_e colorspace);



/**
 * @}
 */



#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_IMAGE_UTIL_INTERNAL_H__ */
