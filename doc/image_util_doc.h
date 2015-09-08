/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_MEDIA_IMAGE_UTIL_DOC_H__
#define __TIZEN_MEDIA_IMAGE_UTIL_DOC_H__

/**
 * @file image_util_doc.h
 * @brief This file contains high level documentation of the Multimedia Service.
 */

/**
 * @defgroup CAPI_MEDIA_IMAGE_UTIL_MODULE Image Util
 * @brief  The @ref CAPI_MEDIA_IMAGE_UTIL_MODULE  APIs provides functions for encoding and decoding of images .
 * @ingroup CAPI_MEDIA_FRAMEWORK
 *
 * @section CAPI_MEDIA_IMAGE_UTIL_MODULE_HEADER Required Header
 *   \#include <image_util.h>
 *
 * @section CAPI_MEDIA_IMAGE_UTIL_MODULE_OVERVIEW Overview
 *
 * Image Util API provides support for encoding and decoding of images. The API allows :
 *- Encoding/Decoding of a JPEG image.
 *- Encoding/Decoding of a JPEG image to/from memory.
 *- Image transformation(Colorspace conversion/Resize/Rotate/Crop) .
 *
 * The decoding of a jpeg image located at a given path, to a buffer can be done by calling #image_util_decode_jpeg() function and decoding of a jpeg image on memory
 * can be done by calling #image_util_decode_jpeg_from_memory(). Similarly, for encoding #image_util_encode_jpeg() and #image_util_encode_jpeg_to_memory() functions can be called
 * to encode an image buffer to a jpeg image and a jpeg buffer respectively. The API also allows to transform including convert the colorspace (#image_util_colorspace_e) of an image by calling
 * #image_util_transform_run() function.
 *
 * @subsection CAPI_MEDIA_IMAGE_UTIL_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>image_util_foreach_supported_jpeg_colorspace()</td>
 *        <td>image_util_supported_jpeg_colorspace_cb()</td>
 *        <td> Supported JPEG encoding/decoding colorspace</td>
 *     </tr>
 *</table></div>
 */

#endif // __TIZEN_MEDIA_IMAGE_UTIL_DOC_H__
