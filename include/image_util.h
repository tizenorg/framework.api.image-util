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

#include <image_util_type.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file image_util.h
 * @brief This file contains the image util API.
 */

/**
 * @addtogroup CAPI_MEDIA_IMAGE_UTIL_MODULE
 * @{
 */

/**
* @brief Creates a handle to image util transform.
* @since_tizen 2.3
*
* @details This function creates a handle to image util transform.
*
* @remarks You must release the @a image util handle using image_util_transform_destroy().
*
* @param[out] handle A handle to image util transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY Out of memory
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_destroy()
*
*/
int image_util_transform_create(transformation_h *handle);

/**
* @brief Sets the image util's accurate mode.
* @since_tizen 2.3
*
* @details This function set if you use hardware accerlation or not.
*
* @remarks The value returned will be IMAGE_UTIL_ERROR_NOT_SUPPORTED, if H/W acceleration doesn't support on the device.
*
* @param[in] handle The handle to image util
* @param [in] mode Set @c true, user can use the hardware acceleration\n
*                               otherwise set @c false if user can only software image processing
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED The application does not have the hardware acceleration
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_hardware_acceleration(transformation_h handle, bool mode);

/**
* @brief Sets the information of the converting
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in] colorspace The colorspace of the image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_colorspace(transformation_h handle, image_util_colorspace_e colorspace);

/**
* @brief Sets the information of the resizing
* @since_tizen 2.3
*
* @remarks image_util_transform_set_resolution and image_util_transform_set_crop_area can't do that at the same time.
*
* @param[in] handle The handle to image util transform
* @param[in] width The width of image buffer
* @param[in] height The height of image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_resolution(transformation_h handle, unsigned int width, unsigned int height);

/**
* @brief Sets the information of the rotating
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in] rotation The rotation value of image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_rotation(transformation_h handle, image_util_rotation_e rotation);

/**
* @brief Sets the information of the cropping
* @since_tizen 2.3
*
* @remarks image_util_transform_set_resolution and image_util_transform_set_crop_area can't do that at the same time.
*
* @param[in] handle The handle to image util transform
* @param[in] start_x The start x position of cropped image buffer
* @param[in] start_y The start y position of cropped image buffer
* @param[in] end_x The end x position of cropped image buffer
* @param[in] end_y The end y position of cropped image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().
*
* @see image_util_transform_create()
* @see image_util_transform_run()
* @see image_util_transform_destroy()
*/
int image_util_transform_set_crop_area(transformation_h handle, unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y);

/**
* @brief Gets the colorspace of the image buffer
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] colorspace The colorspace of the image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Not supported format
*
* @pre image_util_transform_create().\n
            image_util_transform_set_crop_area().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_colorspace(transformation_h handle, image_util_colorspace_e *colorspace);

/**
* @brief Gets the resolution of the image buffer
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] width The width of source image buffer
* @param[in, out] height The height of source image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().\n
            image_util_transform_set_resolution().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_resolution(transformation_h handle, unsigned int *width, unsigned int *height);

/**
* @brief Gets the information of the rotating
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] rotation The rotation value of image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().\n
            image_util_transform_set_rotation().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_rotation(transformation_h handle, image_util_rotation_e *rotation);

/**
* @brief Gets the information of the cropping
* @since_tizen 2.3
*
* @param[in] handle The handle to image util transform
* @param[in, out] start_x The start x position of cropped source image buffer
* @param[in, out] start_y The start y position of cropped source image buffer
* @param[in, out] end_x The end x position of cropped source image buffer
* @param[in, out] end_y The end y position of cropped source image buffer
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @pre image_util_transform_create().\n
            image_util_transform_set_crop_area().
*
* @see image_util_transform_create()
* @see image_util_transform_destroy()
*/
int image_util_transform_get_crop_area(transformation_h handle, unsigned int *start_x, unsigned int *start_y, unsigned int *end_x, unsigned int *end_y);

/**
* @brief Transform the image for given image util handle.
* @since_tizen 2.3
*
* @remarks If H/W acceleration is not set, transformation is done via S/W acceleration.
*
* @details The function execute asynchronously, which contains complete callback \n
*          If you set more than two transforming, the order of running is crop or resolution, colorspace converting, rotaion. \n
*
* @param[in] handle The handle of transform
* @param[in] src The handle to image util transform
* @param[in] callback The callback function to be invoked
* @param[in] user_data The user data to be passed to the callback function
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_create()
* @see image_util_transform_set_hardware_acceleration
* @see image_util_transform_destroy()
*/
int image_util_transform_run(transformation_h handle, media_packet_h src, image_util_transform_completed_cb callback, void *user_data);

/**
* @brief Destroys a handle to image util.
* @since_tizen 2.3
*
* @details The function frees all resources related to the image util handle. The image util
*               handle no longer can be used to perform any operation. A new image util handle
*               has to be created before the next usage.
*
* @param[in] handle The handle to image util transform
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_create()
*
*/
int image_util_transform_destroy(transformation_h handle);

/**
* @brief Called once for each supported JPEG encode/decode colorspace.
* @since_tizen 2.3
*
* @param[in] colorspace The colorspace
* @param[in] user_data The user data passed from the foreach function
*
* @return @c true to continue with the next iteration of the loop, \n
*         otherwise @c false to break out of the loop
*
* @pre image_util_foreach_supported_jpeg_colorspace() invokes this callback.
*
* @see image_util_foreach_supported_jpeg_colorspace()
* @see image_util_encode_jpeg()
* @see image_util_encode_jpeg_to_memory()
* @see image_util_decode_jpeg()
* @see image_util_decode_jpeg_from_memory()
*/
typedef bool(*image_util_supported_jpeg_colorspace_cb)(image_util_colorspace_e colorspace, void *user_data);

/**
* @brief Retrieves all supported JPEG encoding/decoding colorspace by invoking a callback function once for each one.
* @since_tizen 2.3
*
* @param[in] callback The callback function to invoke
* @param[in] user_data The user data to be passed to the callback function
* @return 0 on success,
*         otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
* @post This function invokes image_util_supported_jpeg_colorspace_cb() repeatedly to retrieve each supported JPEG encoding/decoding colorspace.
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_encode_jpeg()
* @see image_util_encode_jpeg_to_memory()
* @see image_util_decode_jpeg()
* @see image_util_decode_jpeg_from_memory()
*/
int image_util_foreach_supported_jpeg_colorspace(image_util_supported_jpeg_colorspace_cb callback, void *user_data);

/**
* @brief Calculates the size of the image buffer for the specified resolution and colorspace.
* @since_tizen 2.3
*
* @param[in] width The image width
* @param[in] height The image height
* @param[in] colorspace The image colorspace
* @param[out] size The Calculated buffer size
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_transform_run()
* @see image_util_decode_jpeg()
* @see image_util_decode_jpeg_from_memory()
* @see image_util_encode_jpeg()
* @see image_util_encode_jpeg_to_memory()
*/
int image_util_calculate_buffer_size(int width, int height, image_util_colorspace_e colorspace , unsigned int *size);

/**
* @brief Decodes the JPEG image to the buffer.
* @since_tizen 2.3
*
* @remarks You must release @a image_buffer using free().\n
*                 http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*                 http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.\n
*                 If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                 the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] path The image file path
* @param[in] colorspace The decoded image colorspace
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg_from_memory()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg(const char *path, image_util_colorspace_e colorspace, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @brief Decodes the JPEG image(in memory) to the buffer.
* @since_tizen 2.3
*
* @remarks You must release @a image_buffer using free().\n
*                  If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                  the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] jpeg_buffer The JPEG image buffer
* @param[in] jpeg_size The JPEG image buffer size
* @param[in] colorspace The decoded image colorspace
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg_from_memory(const unsigned char *jpeg_buffer, int jpeg_size, image_util_colorspace_e colorspace, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @brief Decodes the JPEG image to the buffer with downscale decoding option.
* @since_tizen 2.4
*
* @remarks You must release @a image_buffer using free().\n
*                 http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*                 http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.\n
*                 If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                 the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] path The image file path
* @param[in] colorspace The decoded image colorspace
* @param[in] downscale The downscale value
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return @c 0 on success,
*               otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg_from_memory()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg_with_downscale(const char *path, image_util_colorspace_e colorspace, image_util_scale_e downscale, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @brief Decodes the JPEG image(in memory) to the buffer with downscale decoding option.
* @since_tizen 2.4
*
* @remarks You must release @a image_buffer using free().\n
*                   If you decode the JPEG image which has odd numbered width or height to YUV colorspace, \n
*                   the width or height of decoded data will be rounded down to even numbered width or height.
*
* @param[in] jpeg_buffer The JPEG image buffer
* @param[in] jpeg_size The JPEG image buffer size
* @param[in] colorspace The decoded image colorspace
* @param[in] downscale The downscale value
* @param[out] image_buffer The image buffer for the decoded image
* @param[out] width The image width
* @param[out] height The image height
* @param[out] size The image buffer size
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_decode_jpeg()
* @see image_util_foreach_supported_jpeg_colorspace()
*/
int image_util_decode_jpeg_from_memory_with_downscale(const unsigned char *jpeg_buffer, int jpeg_size, image_util_colorspace_e colorspace, image_util_scale_e downscale, unsigned char **image_buffer, int *width, int *height, unsigned int *size);

/**
* @brief Encodes the image to the JPEG image.
* @since_tizen 2.3
*
* @remarks http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.\n
*                 http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
*
* @param[in] buffer The original image buffer
* @param[in] width The original image width
* @param[in] height The original image height
* @param[in] colorspace The original image colorspace
* @param[in] quality The quality for JPEG image encoding(1 ~ 100)
* @param[in] path The file path to be created
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_NO_SUCH_FILE No such file
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_foreach_supported_jpeg_colorspace()
* @see image_util_encode_jpeg_to_memory()
*/
int image_util_encode_jpeg(const unsigned char *buffer, int width, int height, image_util_colorspace_e colorspace,  int quality, const char *path);

/**
* @brief Encodes the image to the JPEG image
* @since_tizen 2.3
*
* @remarks You must release @a jpeg_buffer using free().
*
* @param[in] image_buffer The original image buffer
* @param[in] width The image width
* @param[in] height The image height
* @param[in] colorspace The original image colorspace
* @param[in] quality The quality for JPEG image encoding(1 ~ 100)
* @param[out] jpeg_buffer The created JPEG image buffer \n
*                         The buffer is created by frameworks
* @param[out] jpeg_size The created jpeg image buffer size
*
* @return 0 on success,
*           otherwise a negative error value
*
* @retval #IMAGE_UTIL_ERROR_NONE Successful
* @retval #IMAGE_UTIL_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #IMAGE_UTIL_ERROR_OUT_OF_MEMORY out of memory
* @retval #IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT Format not supported
* @retval #IMAGE_UTIL_ERROR_INVALID_OPERATION Invalid operation
* @retval #IMAGE_UTIL_ERROR_PERMISSION_DENIED The application does not have the privilege to call this funtion
*
* @see image_util_supported_jpeg_colorspace_cb()
* @see image_util_foreach_supported_jpeg_colorspace()
* @see image_util_encode_jpeg()
*/
int image_util_encode_jpeg_to_memory(const unsigned char *image_buffer, int width, int height, image_util_colorspace_e colorspace, int quality,  unsigned char **jpeg_buffer, unsigned int *jpeg_size);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_IMAGE_UTIL_H__ */
