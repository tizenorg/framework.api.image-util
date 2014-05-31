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



#define API_NAME_IMAGEUTIL_COLORSPACE "image_util_colorspace"
#define API_NAME_IMAGEUTIL_COLOR_CONVERT "image_util_color_convert"
#define API_NAME_IMAGEUTIL_BUFFER_SIZE "image_util_buffer_size"
#define API_NAME_IMAGEUTIL_TRANSFORM "image_util_color_transform"
#define API_NAME_IMAGEUTIL_CROP "image_util_color_crop"

#define SAMPLE_FILENAME "sample.jpg"

#define LAST_COLORSPACE		IMAGE_UTIL_COLORSPACE_BGRX8888

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

//Retrieves all supported JPEG encoding/decoding colorspace by invoking a callback function once for each one.
static void utc_image_util_supported_jpeg_colorspace_p(void);   // how many colorspaces are available
static void utc_image_util_supported_jpeg_colorspace_2_p(void); // does ,,foreach'' return pass?
static void utc_image_util_supported_jpeg_colorspace_3_p(void); // does ,,foreach'' return pass for all cases?

//Convert the image's colorspace.
static void utc_image_util_convert_colorspace_p(void);
static void utc_image_util_convert_colorspace_1_n(void);
static void utc_image_util_convert_colorspace_2_n(void);

//Calculates the size of image buffer for the specified resolution and colorspace.
static void utc_image_util_calculate_bufsize_result_p(void);
static void utc_image_util_calculate_bufsize_result_1_n(void);
static void utc_image_util_calculate_bufsize_result_2_n(void);
static void utc_image_util_calculate_bufsize_result_3_n(void);
static void utc_image_util_calculate_bufsize_result_4_p(void);
static void utc_image_util_calculate_bufsize_result_5_p(void);


//Rotates the image to with the angle in degrees.
static void utc_image_util_file_rotate_p(void);
static void utc_image_util_file_rotate_2_p(void);
static void utc_image_util_file_rotate_3_p(void);

//Resizes the image to with the angle in degrees.
static void utc_image_util_file_resize_p(void);
static void utc_image_util_file_resize_2_p(void);
static void utc_image_util_file_resize_3_n(void);

//Crop the image to with the angle in degrees.
static void utc_image_util_file_crop_p(void);
static void utc_image_util_file_crop_n(void);

static bool jpeg_colorspace_cb( image_util_colorspace_e colorspace, void * user_data);
static bool jpeg_colorspace_quit_cb( image_util_colorspace_e colorspace, void * user_data);
static bool jpeg_colorspace_size_cb( image_util_colorspace_e colorspace, void * user_data);


//available transforms
#define AT_COUTN 4
const int avail_trans[AT_COUTN] =
{
	IMAGE_UTIL_COLORSPACE_YV12,
	IMAGE_UTIL_COLORSPACE_I420,
	IMAGE_UTIL_COLORSPACE_NV12,
	IMAGE_UTIL_COLORSPACE_RGB888,
};



enum
{
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};




struct tet_testlist tet_testlist[] = {
	{ utc_image_util_supported_jpeg_colorspace_p, 1},
	{ utc_image_util_supported_jpeg_colorspace_2_p, 2},
	{ utc_image_util_supported_jpeg_colorspace_3_p, 3},
	{ utc_image_util_calculate_bufsize_result_p, 4},
	{ utc_image_util_calculate_bufsize_result_1_n, 5},
	{ utc_image_util_calculate_bufsize_result_2_n, 6},
	{ utc_image_util_calculate_bufsize_result_3_n, 7},
	{ utc_image_util_calculate_bufsize_result_4_p, 8},
	{ utc_image_util_calculate_bufsize_result_5_p, 9},
	{ utc_image_util_convert_colorspace_p, 10},
	{ utc_image_util_convert_colorspace_1_n, 11},
	{ utc_image_util_convert_colorspace_2_n, 12},
	{ utc_image_util_file_rotate_p, 13},
	{ utc_image_util_file_rotate_2_p, 14},
	{ utc_image_util_file_rotate_3_p, 15},
	{ utc_image_util_file_resize_p, 16},
	{ utc_image_util_file_resize_2_p, 17},
	{ utc_image_util_file_resize_3_n, 18},
	{ utc_image_util_file_crop_p, 19},
	{ utc_image_util_file_crop_n, 20},
	{ NULL, 0},
};


static void startup(void)
{
	// TC start
}

static void cleanup(void)
{
	// TC end
}



static bool jpeg_colorspace_cb( image_util_colorspace_e colorspace, void * user_data)
{
	return true;
}


static bool jpeg_colorspace_quit_cb( image_util_colorspace_e colorspace, void * user_data)
{
	int last = *((int*)user_data);
	return (int)colorspace == last  ?  false : true;
}



static bool jpeg_colorspace_size_cb( image_util_colorspace_e colorspace, void * user_data)
{
	int * result = (int*)user_data;

	const int W = 480, H = 320; // these values are taken from test file - sample.jpg
	int width = 0, height = 0;
	int size_decode = 0;
	unsigned int size_calc = 0;
	unsigned char * img_buf = 0;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_buf, &width, &height, &size_decode );

	// prepare buffers
	image_util_calculate_buffer_size( W, H, colorspace, &size_calc );

	free( img_buf );

	*result = (size_decode == size_calc);

	// if false then callbacks stop
	return *result;
}





/**
 * @brief how many colorspaces are available
 */
static void utc_image_util_supported_jpeg_colorspace_p(void)
{
	int pointer = 0;
	int ret = image_util_foreach_supported_jpeg_colorspace( NULL , (void*)(&pointer) );
	dts_check_eq( API_NAME_IMAGEUTIL_COLORSPACE, ret, IMAGE_UTIL_ERROR_INVALID_PARAMETER );
}







/**
 * @brief does ,,foreach'' return pass?
 */
static void utc_image_util_supported_jpeg_colorspace_2_p(void)
{
	int err = image_util_foreach_supported_jpeg_colorspace( jpeg_colorspace_cb , (void*)NULL );
	dts_check_eq( API_NAME_IMAGEUTIL_COLORSPACE, err, IMAGE_UTIL_ERROR_NONE );
}







/**
 * @brief does ,,foreach'' return pass for all cases?
 */
static void utc_image_util_supported_jpeg_colorspace_3_p(void)
{
    int err;
    char buf[100] = { '\0' };
    snprintf( buf, 100, "%s", API_NAME_IMAGEUTIL_COLORSPACE );

    // parameter i decides when loop of callbacks should break
    int i;
    for( i = 0; i < LAST_COLORSPACE; ++i )
    {
		err = image_util_foreach_supported_jpeg_colorspace( jpeg_colorspace_quit_cb , (void*)(&i) );
		if( err != 0 )
		{
			sprintf( buf, " [error: colorspace_2_p on position %d]", i );
			break;
		}
	}

	dts_check_eq( buf, err, 0 );
}






/**
 * @brief check the buffer size calculation with correct parameters
 */
static void utc_image_util_calculate_bufsize_result_p(void)
{
	int width = 100, height = 20;
	unsigned int size = 0;

	int err = image_util_calculate_buffer_size( width, height, IMAGE_UTIL_COLORSPACE_RGB888, &size );
	dts_check_eq( API_NAME_IMAGEUTIL_BUFFER_SIZE, err, 0 );
}





/**
 * @brief check the buffer size calculation with uncorrect parameters
 */
static void utc_image_util_calculate_bufsize_result_1_n(void)
{
	int width = 100, height = 20;
	unsigned int size = 0;
	const int colorspace = IMAGE_UTIL_COLORSPACE_BGRX8888 + 1; // after last one

	int err = image_util_calculate_buffer_size( width, height, colorspace, &size );
	dts_check_ne( API_NAME_IMAGEUTIL_BUFFER_SIZE, err, 0 );
}






/**
 * @brief check the buffer size calculation with uncorrect parameters
 */
static void utc_image_util_calculate_bufsize_result_2_n(void)
{
	int width = -100, height = 20; // wrong parameter
	unsigned int size = 0;
	const int colorspace = IMAGE_UTIL_COLORSPACE_BGRX8888;

	int err = image_util_calculate_buffer_size( width, height, colorspace, &size );
	dts_check_ne( API_NAME_IMAGEUTIL_BUFFER_SIZE, err, 0 );
}






/**
 * @brief check the buffer size calculation with uncorrect parameters
 */
static void utc_image_util_calculate_bufsize_result_3_n(void)
{
	int width = 0, height = 0; // wrong parameter
	unsigned int size = 0;
	const int colorspace = IMAGE_UTIL_COLORSPACE_BGRX8888;

	int err = image_util_calculate_buffer_size( width, height, colorspace, &size );
	dts_check_ne( API_NAME_IMAGEUTIL_BUFFER_SIZE, err, 0 );
}





/**
 * @brief do the buffer size calculation and check if
 *        this is the same like with decode_jpeg function
 */
static void utc_image_util_calculate_bufsize_result_4_p(void)
{
	const int W = 480, H = 320; // these values are taken from original file sample.jpg
	int width = 0, height = 0;
	int size_decode = 0;
	unsigned int size_calc = 0;
	unsigned char * img_buf = 0;
	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_RGB888;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_buf, &width, &height, &size_decode );

	// prepare buffers
	image_util_calculate_buffer_size( W, H, colorspace, &size_calc );

	free( img_buf );

	dts_check_eq( API_NAME_IMAGEUTIL_BUFFER_SIZE, (int)size_calc, size_decode );
}




/**
 * @brief do the buffer size calculation for all colorspaces
 *        this is similar like ..._calculate_bufsize_result_4_p
 */
static void utc_image_util_calculate_bufsize_result_5_p(void)
{
	int result = -1;

	// do the loop for all colorspaces and check bufsize there inside
	image_util_foreach_supported_jpeg_colorspace( jpeg_colorspace_size_cb, (void*)(&result) );

	// result is taken from inside the colorspace callbacks loop,
	// there is compared buffor size
	// result takes true or false
	dts_check_eq( API_NAME_IMAGEUTIL_BUFFER_SIZE, result, 1 );
}


/**
 * @brief check if color conversion returns positive
 */
static void utc_image_util_convert_colorspace_p(void)
{
	int width = 0, height = 0;
	unsigned int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;
	const image_util_colorspace_e cs_target = IMAGE_UTIL_COLORSPACE_RGB888;
	const image_util_colorspace_e cs_source = IMAGE_UTIL_COLORSPACE_YV12;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, cs_source, &img_source, &width, &height, &size_decode );
	image_util_calculate_buffer_size(width, height, cs_target , &size_decode);
	img_target = malloc( size_decode );

	// do conversion
	int ret = image_util_convert_colorspace( img_target, cs_target,
												img_source, width, height, cs_source );
    free( img_target );
    free( img_source );

    dts_check_eq( API_NAME_IMAGEUTIL_COLOR_CONVERT, ret, IMAGE_UTIL_ERROR_NONE );
}






/**
 * @brief check if color conversion function has the verification of input parameters
 */
static void utc_image_util_convert_colorspace_1_n(void)
{
	int width = 0, height = 0;
	unsigned char * img_target = 0; // NULL pointer!
	unsigned char * img_source = 0; // NULL pointer!
	const image_util_colorspace_e cs_target = IMAGE_UTIL_COLORSPACE_RGB888;
	const image_util_colorspace_e cs_source = IMAGE_UTIL_COLORSPACE_YV12;

	// do conversion
	int ret = image_util_convert_colorspace( img_target, cs_target,
												img_source, width, height, cs_source );

    dts_check_ne( API_NAME_IMAGEUTIL_COLOR_CONVERT, ret, IMAGE_UTIL_ERROR_NONE );
}



/**
 * @brief check if color conversion has verification of input parameters
 */
static void utc_image_util_convert_colorspace_2_n(void)
{
	int width = 0, height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;
	const image_util_colorspace_e cs_target = IMAGE_UTIL_COLORSPACE_BGRX8888 + 1; // out of the scope!
	const image_util_colorspace_e cs_source = IMAGE_UTIL_COLORSPACE_YV12;
	
	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, cs_source, &img_source, &width, &height, &size_decode );

	// do conversion
	int ret = image_util_convert_colorspace( img_target, cs_target,
												img_source, width, height, cs_source );
    free( img_target );
    free( img_source );

    dts_check_ne( API_NAME_IMAGEUTIL_COLOR_CONVERT, ret, IMAGE_UTIL_ERROR_NONE );
}




/**
 * @brief Simple use of rotation
 */
static void utc_image_util_file_rotate_p(void)
{
	int width = 0, height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	image_util_rotation_e dest_rotation = IMAGE_UTIL_ROTATION_90;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_RGB888;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );

	image_util_calculate_buffer_size(width, height, colorspace , &size_decode);
	
	img_target = malloc( size_decode );

	// do rotation
	int ret = image_util_rotate( img_target, &width, &height, dest_rotation,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );

    dts_check_eq( API_NAME_IMAGEUTIL_TRANSFORM, ret, IMAGE_UTIL_ERROR_NONE );
}





/**
 * @brief Image transformation for all rotation types
 */
static void utc_image_util_file_rotate_2_p(void)
{
	int width = 0, height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	image_util_rotation_e dest_rotation = IMAGE_UTIL_ROTATION_NONE;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_RGB888;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );

	image_util_calculate_buffer_size(width, height, colorspace , &size_decode);

	img_target = malloc( size_decode );

	// do rotation
	int ret = image_util_rotate( img_target, &width, &height, dest_rotation,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );

    dts_check_eq( API_NAME_IMAGEUTIL_TRANSFORM, ret, IMAGE_UTIL_ERROR_NONE );
}





/**
 * @brief Image rotation for all available colorspaces
 */
static void utc_image_util_file_rotate_3_p(void)
{
	int width = 0, height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	image_util_rotation_e dest_rotation = IMAGE_UTIL_ROTATION_FLIP_VERT;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_RGB888;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );

	image_util_calculate_buffer_size(width, height, colorspace , &size_decode);
	
	img_target = malloc( size_decode );

	// do rotation
	int ret = image_util_rotate( img_target, &width, &height, dest_rotation,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );



    dts_check_eq( API_NAME_IMAGEUTIL_TRANSFORM, ret, IMAGE_UTIL_ERROR_NONE );
}




/**
 * @brief Simple use of resize
 */
static void utc_image_util_file_resize_p(void)
{
	int width = 0, height = 0;
	int resized_width = 0, resized_height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_RGB888;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );

	resized_width = width / 2;
	resized_height = height /2;
	image_util_calculate_buffer_size(resized_width, resized_height, colorspace , &size_decode);

	img_target = malloc( size_decode );

	// do rotation
	int ret = image_util_resize( img_target, &resized_width, &resized_height,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );

    dts_check_eq( API_NAME_IMAGEUTIL_TRANSFORM, ret, IMAGE_UTIL_ERROR_NONE );
}




/**
 * @brief Simple use of resize
 */
static void utc_image_util_file_resize_2_p(void)
{
	int width = 0, height = 0;
	int resized_width = 0, resized_height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_YV12;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );

	resized_width = width / 2;
	resized_height = height /2;
	image_util_calculate_buffer_size(resized_width, resized_height, colorspace , &size_decode);

	img_target = malloc( size_decode );

	// do rotation
	int ret = image_util_resize( img_target, &resized_width, &resized_height,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );

    dts_check_eq( API_NAME_IMAGEUTIL_TRANSFORM, ret, IMAGE_UTIL_ERROR_NONE );
}




/**
 * @brief check if color conversion has verification of resized value
 */
static void utc_image_util_file_resize_3_n(void)
{
	int width = 0, height = 0;
	int resized_width = 0, resized_height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_RGB888;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );

	resized_width = 0;
	resized_height = 0;
	image_util_calculate_buffer_size(resized_width, resized_height, colorspace , &size_decode);

	img_target = malloc( size_decode );

	// do rotation
	int ret = image_util_resize( img_target, &resized_width, &resized_height,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );

    dts_check_ne( API_NAME_IMAGEUTIL_TRANSFORM, ret, IMAGE_UTIL_ERROR_NONE );
}




/**
 * @brief Simple use of crop
 */
static void utc_image_util_file_crop_p(void)
{
	int width = 0, height = 0;
	int crop_start_x = 10, crop_start_y = 10, crop_dest_width = 100, crop_dest_height = 100;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_YV12;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );

	image_util_calculate_buffer_size(crop_dest_width, crop_dest_height, colorspace , &size_decode);

	img_target = malloc( size_decode );

	// do crop
	int ret = image_util_crop(img_target, crop_start_x, crop_start_y, &crop_dest_width, &crop_dest_height,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );

    dts_check_eq( API_NAME_IMAGEUTIL_CROP, ret, IMAGE_UTIL_ERROR_NONE );
}





/**
 * @brief check if color conversion has verification of cropped value
 */
static void utc_image_util_file_crop_n(void)
{
	int width = 0, height = 0;
	int crop_start_x = 10, crop_start_y = 10, crop_dest_width = 0, crop_dest_height = 0;
	int size_decode = 0;
	unsigned char * img_target = 0;
	unsigned char * img_source = 0;

	const image_util_colorspace_e colorspace = IMAGE_UTIL_COLORSPACE_YV12;

	// load jpeg sample file
	image_util_decode_jpeg( SAMPLE_FILENAME, colorspace, &img_source, &width, &height, &size_decode );
	crop_dest_width = width + crop_start_x;
	crop_dest_height = height + crop_start_y;
	image_util_calculate_buffer_size(crop_dest_width, crop_dest_height, colorspace , &size_decode);

	img_target = malloc( size_decode );

	// do crop
	int ret = image_util_crop(img_target, crop_start_x, crop_start_y, &crop_dest_width, &crop_dest_height,
										img_source, width, height, colorspace );

    free( img_target );
    free( img_source );

    dts_check_ne( API_NAME_IMAGEUTIL_CROP, ret, IMAGE_UTIL_ERROR_NONE );
}