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


#include <stdio.h>
#include <tet_api.h>
#include <image_util.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

#define SAMPLE_JPEG "sample.jpg"
#define WRONG_PATH ""
#define OUTPUT_JPEG "test_output.jpg"

image_util_colorspace_e SUPPORTED_COLORSPACE;
image_util_colorspace_e NOT_SUPPORTED_COLORSPACE;
image_util_colorspace_e FIRST_COLORSPACE;
image_util_colorspace_e LAST_COLORSPACE;

/**
 * @brief Global struct for raw image data
 */
struct{
    unsigned char *buffer;
    int size;
    int w;
    int h;
}raw_image = {NULL, 0, 0, 0};


/**
 * @brief Global struct for jpeg image
 */
struct{
    unsigned char *buffer;
    int size;
}jpeg_image = {NULL, 0};


#define API_NAME_IMAGE_UTIL_DECODE_JPEG "image_util_decode_jpeg"
#define API_NAME_IMAGE_UTIL_ENCODE_JPEG "image_util_encode_jpeg"
#define API_NAME_IMAGE_UTIL_DECODE_JPEG_FROM_MEMORY "image_util_decode_jpeg_from_memory"
#define API_NAME_IMAGE_UTIL_ENCODE_JPEG_TO_MEMORY "image_util_encode_jpeg_to_memory"

static void utc_image_util_decode_jpeg_n_1(void);
static void utc_image_util_decode_jpeg_n_2(void);
static void utc_image_util_decode_jpeg_n_3(void);
static void utc_image_util_decode_jpeg_n_4(void);
static void utc_image_util_decode_jpeg_n_5(void);
static void utc_image_util_decode_jpeg_p(void);
static void utc_image_util_decode_jpeg_from_memory_n_1(void);
static void utc_image_util_decode_jpeg_from_memory_n_2(void);
static void utc_image_util_decode_jpeg_from_memory_n_3(void);
static void utc_image_util_decode_jpeg_from_memory_n_4(void);
static void utc_image_util_decode_jpeg_from_memory_p(void);
static void utc_image_util_encode_jpeg_n_1(void);
static void utc_image_util_encode_jpeg_n_2(void);
static void utc_image_util_encode_jpeg_n_3(void);
static void utc_image_util_encode_jpeg_n_4(void);
static void utc_image_util_encode_jpeg_n_5(void);
static void utc_image_util_encode_jpeg_p(void);
static void utc_image_util_encode_jpeg_to_memory_n_1(void);
static void utc_image_util_encode_jpeg_to_memory_n_2(void);
static void utc_image_util_encode_jpeg_to_memory_n_3(void);
static void utc_image_util_encode_jpeg_to_memory_n_4(void);
static void utc_image_util_encode_jpeg_to_memory_p(void);

enum
{
    POSITIVE_TC_IDX = 0x01,
    NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
/**
 *  image_util_decode_jpeg
 */
    { utc_image_util_decode_jpeg_n_1, 1 },
    { utc_image_util_decode_jpeg_n_2, 2 },
    { utc_image_util_decode_jpeg_n_3, 3 },
    { utc_image_util_decode_jpeg_n_4, 4 },
    { utc_image_util_decode_jpeg_n_5, 5 },
    { utc_image_util_decode_jpeg_p, 6},

/**
 *  image_util_encode_jpeg
 */
    { utc_image_util_encode_jpeg_n_1, 7 },
    { utc_image_util_encode_jpeg_n_2, 8 },
    { utc_image_util_encode_jpeg_n_3, 9 },
    { utc_image_util_encode_jpeg_n_4, 10 },
    { utc_image_util_encode_jpeg_n_5, 11 },
    { utc_image_util_encode_jpeg_p, 12},

/**
 *  image_util_encode_jpeg_to_memory
 */
    { utc_image_util_encode_jpeg_to_memory_n_1, 13 },
    { utc_image_util_encode_jpeg_to_memory_n_2, 14 },
    { utc_image_util_encode_jpeg_to_memory_n_3, 15 },
    { utc_image_util_encode_jpeg_to_memory_n_4, 16 },
    { utc_image_util_encode_jpeg_to_memory_p, 17 },

/**
 *  image_util_decode_jpeg_from_memory
 */
    { utc_image_util_decode_jpeg_from_memory_n_1, 18 },
    { utc_image_util_decode_jpeg_from_memory_n_2, 19 },
    { utc_image_util_decode_jpeg_from_memory_n_3, 20 },// SIGSEGV from api
    { utc_image_util_decode_jpeg_from_memory_n_4, 21 },
    { utc_image_util_decode_jpeg_from_memory_p, 22 }, // SIGSEGV from api
    { NULL, 0 },
};

/**
 * @brief Callback to find first supported colorspace
 */
static bool find_first_supported_colorspace_cb( image_util_colorspace_e colorspace, void * user_data)
{
    *(int*)user_data = colorspace;
    return false;
}

static void startup(void)
{
    /* start of TC */
    tet_printf("\n TC start");
    // set first not supportes colorspace
    image_util_foreach_supported_jpeg_colorspace( find_first_supported_colorspace_cb , (void*)(&SUPPORTED_COLORSPACE) );

    // set oher colorspaces
    NOT_SUPPORTED_COLORSPACE = IMAGE_UTIL_COLORSPACE_YUV422; //TODO: FIND NOT SUPPORTED FORMAT
    FIRST_COLORSPACE = IMAGE_UTIL_COLORSPACE_YV12;
    LAST_COLORSPACE = IMAGE_UTIL_COLORSPACE_BGRX8888;

    // prepare buffers for raw and jpeg images
    if(image_util_decode_jpeg(SAMPLE_JPEG, SUPPORTED_COLORSPACE, &raw_image.buffer, &raw_image.w, &raw_image.h, &raw_image.size) == IMAGE_UTIL_ERROR_NONE){
        tet_printf("\n raw_image initialization OK");
        if(image_util_encode_jpeg_to_memory(raw_image.buffer, raw_image.w , raw_image.h , SUPPORTED_COLORSPACE, 100, &jpeg_image.buffer, &jpeg_image.size) == IMAGE_UTIL_ERROR_NONE)
            tet_printf("\n jpeg_image initialization OK");
        else
            tet_printf("\n jpeg_image initialization FAILED");
    }
    else
       tet_printf("\n raw_image initialization FAILED");
}

static void cleanup(void)
{
    /* end of TC */
    free(raw_image.buffer);
    free(jpeg_image.buffer);
    tet_printf("\n TC end");
}

/**
 * @brief Negative test case of image_util_decode_jpeg(). Invalid path or image_buffer parameters;
 */
static void utc_image_util_decode_jpeg_n_1(void)
{
    int r;
    int w, h;
    int size;

    r = image_util_decode_jpeg(NULL, SUPPORTED_COLORSPACE, NULL, &w, &h, &size);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_decode_jpeg(). Parameter colorspace over the range.
 */
static void utc_image_util_decode_jpeg_n_2(void)
{
    int r;
    int w, h;
    int size;
    unsigned char *buffer = NULL;

    r = image_util_decode_jpeg(SAMPLE_JPEG, LAST_COLORSPACE + 1, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_decode_jpeg(). Parameter colorspace below the range.
 */
static void utc_image_util_decode_jpeg_n_3(void)
{
    int r;
    int w, h;
    int size;
    unsigned char *buffer = NULL;

    r = image_util_decode_jpeg(SAMPLE_JPEG, FIRST_COLORSPACE - 1, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_decode_jpeg(). Not supported format.
 */
static void utc_image_util_decode_jpeg_n_4(void)
{
    int r;
    int w, h;
    int size;
    unsigned char *buffer = NULL;

    r = image_util_decode_jpeg(SAMPLE_JPEG, NOT_SUPPORTED_COLORSPACE, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG, r, IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT);
}

/**
 * @brief Negative test case of image_util_decode_jpeg(). Wrong image file path. (Incomplete documentation!)
 */
static void utc_image_util_decode_jpeg_n_5(void)
{
    int r;
    int w, h;
    int size;
    unsigned char *buffer = NULL;

    r = image_util_decode_jpeg(WRONG_PATH, SUPPORTED_COLORSPACE, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG, r, IMAGE_UTIL_ERROR_NO_SUCH_FILE);
}


/**
 * @brief Positive test case of image_util_decode_jpeg(). Al parameters OK, Success expected.
 */
static void utc_image_util_decode_jpeg_p(void)
{
    int r;
    unsigned char *buffer = NULL;
    int w, h;
    int size;

    r = image_util_decode_jpeg(SAMPLE_JPEG, SUPPORTED_COLORSPACE, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG, r, IMAGE_UTIL_ERROR_NONE);
}


/**
 * @brief Negative test case of image_util_decode_jpeg_from_memory(). Invalid jpeg_buffer or image_buffer parameters;
 */
static void utc_image_util_decode_jpeg_from_memory_n_1(void)
{
    int r;
    int w, h;
    int size;

    r = image_util_decode_jpeg_from_memory(NULL, jpeg_image.size, SUPPORTED_COLORSPACE, NULL, &w, &h, &size);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG_FROM_MEMORY, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_decode_jpeg_from_memory(). Parameter colorspace below the range.
 */
static void utc_image_util_decode_jpeg_from_memory_n_2(void)
{
    int r;
    unsigned char *buffer = NULL;
    int w, h;
    int size;

    r = image_util_decode_jpeg_from_memory(jpeg_image.buffer, jpeg_image.size, FIRST_COLORSPACE -1, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG_FROM_MEMORY, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_decode_jpeg_from_memory(). Parameter colorspace over the range.
 */
static void utc_image_util_decode_jpeg_from_memory_n_3(void)
{
    int r;
    unsigned char *buffer = NULL;
    int w, h;
    int size;

    r = image_util_decode_jpeg_from_memory(jpeg_image.buffer, jpeg_image.size, LAST_COLORSPACE + 1, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG_FROM_MEMORY, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_decode_jpeg_from_memory(). Not supported format.
 */
static void utc_image_util_decode_jpeg_from_memory_n_4(void)
{
    int r;
    unsigned char *buffer = NULL;
    int w, h;
    int size;

    r = image_util_decode_jpeg_from_memory(jpeg_image.buffer, jpeg_image.size, NOT_SUPPORTED_COLORSPACE, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG_FROM_MEMORY, r, IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT);
}


/**
 * @brief Positive test image_util_decode_jpeg_from_memory(). Al parameters OK, Success expected.
 */
static void utc_image_util_decode_jpeg_from_memory_p(void)
{
    int r;
    unsigned char *buffer = NULL;
    int w, h;
    int size;

    r = image_util_decode_jpeg_from_memory(jpeg_image.buffer, jpeg_image.size, SUPPORTED_COLORSPACE, &buffer, &w, &h, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_DECODE_JPEG_FROM_MEMORY, r, IMAGE_UTIL_ERROR_NONE);
}
/**
 * @brief Negative test case of image_util_encode_jpeg(). Invalid path or buffer parameters;
 */
static void utc_image_util_encode_jpeg_n_1(void)
{
    int r;

    r = image_util_encode_jpeg(NULL, raw_image.w , raw_image.h , SUPPORTED_COLORSPACE, 100, NULL);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_encode_jpeg(). Parameter colorspace over the range.
 */
static void utc_image_util_encode_jpeg_n_2(void)
{
    int r;

    r = image_util_encode_jpeg(raw_image.buffer, raw_image.w , raw_image.h , LAST_COLORSPACE + 1, 100, OUTPUT_JPEG);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}
/**
 * @brief Negative test case of image_util_encode_jpeg(). Parameter colorspace below the range.
 */
static void utc_image_util_encode_jpeg_n_3(void)
{
    int r;

    r = image_util_encode_jpeg(raw_image.buffer, raw_image.w , raw_image.h , FIRST_COLORSPACE -1, 100, OUTPUT_JPEG);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_encode_jpeg(). Not supported format.
 */
static void utc_image_util_encode_jpeg_n_4(void)
{
    int r;

    r = image_util_encode_jpeg(raw_image.buffer, raw_image.w , raw_image.h , NOT_SUPPORTED_COLORSPACE, 100, OUTPUT_JPEG);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG, r, IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT);
}


/**
 * @brief Negative test case of image_util_encode_jpeg(). Wrong image file path.
 */
static void utc_image_util_encode_jpeg_n_5(void)
{
    int r;

    r = image_util_encode_jpeg(raw_image.buffer, raw_image.w , raw_image.h , SUPPORTED_COLORSPACE, 100, WRONG_PATH);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG, r, IMAGE_UTIL_ERROR_NO_SUCH_FILE);
}


/**
 * @brief Positive test case of image_util_encode_jpeg(). Al parameters OK, Success expected.
 */
static void utc_image_util_encode_jpeg_p(void)
{
    int r;

    r = image_util_encode_jpeg(raw_image.buffer, raw_image.w , raw_image.h , SUPPORTED_COLORSPACE, 100, OUTPUT_JPEG);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG, r, IMAGE_UTIL_ERROR_NONE);
}

/**
 * @brief Negative test case of image_util_encode_jpeg_to_memory(). Invalid path or image_buffer or jpeg_buffer parameters;
 */
static void utc_image_util_encode_jpeg_to_memory_n_1(void)
{
    int r;
    int size;

    r = image_util_encode_jpeg_to_memory(NULL, 5 , 5 , SUPPORTED_COLORSPACE, 100, NULL, &size);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG_TO_MEMORY, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_encode_jpeg_to_memory(). Parameter colorspace over the range.
 */
static void utc_image_util_encode_jpeg_to_memory_n_2(void)
{
    int r;
    unsigned char *buffer = NULL;
    int size;

    r = image_util_encode_jpeg_to_memory(raw_image.buffer, raw_image.w , raw_image.h , FIRST_COLORSPACE - 1, 100, &buffer, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG_TO_MEMORY, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_encode_jpeg_to_memory(). Parameter colorspace below the range.
 */
static void utc_image_util_encode_jpeg_to_memory_n_3(void)
{
    int r;
    unsigned char *buffer = NULL;
    int size;

    r = image_util_encode_jpeg_to_memory(raw_image.buffer, raw_image.w , raw_image.h , LAST_COLORSPACE + 1, 100, &buffer, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG_TO_MEMORY, r, IMAGE_UTIL_ERROR_INVALID_PARAMETER);
}

/**
 * @brief Negative test case of image_util_encode_jpeg_to_memory(). Not supported format.
 */
static void utc_image_util_encode_jpeg_to_memory_n_4(void)
{
    int r;
    unsigned char *buffer = NULL;
    int size;

    r = image_util_encode_jpeg_to_memory(raw_image.buffer, raw_image.w , raw_image.h , NOT_SUPPORTED_COLORSPACE, 100, &buffer, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG_TO_MEMORY, r, IMAGE_UTIL_ERROR_NOT_SUPPORTED_FORMAT);
}

/**
 * @brief Positive test case of image_util_encode_jpeg_to_memory(). Al parameters OK, Success expected.
 */
static void utc_image_util_encode_jpeg_to_memory_p(void)
{
    int r;
    unsigned char *buffer = NULL;
    int size;

    r = image_util_encode_jpeg_to_memory(raw_image.buffer, raw_image.w , raw_image.h , SUPPORTED_COLORSPACE, 100, &buffer, &size);
    free(buffer);
    dts_check_eq(API_NAME_IMAGE_UTIL_ENCODE_JPEG_TO_MEMORY, r, IMAGE_UTIL_ERROR_NONE);
}
