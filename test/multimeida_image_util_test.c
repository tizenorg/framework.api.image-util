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

#include <Elementary.h>
#include <glib.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <stdbool.h>
#include <stdio.h>
#include <image_util.h>

/*
 * Function displaying image on canva
 */
static void _display_buffer_as_efl_image(Evas_Object *img, unsigned char *buf, int w, int h, image_util_colorspace_e colorspace)
{
	unsigned int sizeBGRA8888;
	int err;

	/*
	 * Calculates the size of image buffer for the specified resolution and colorspace
	 */
	err = image_util_calculate_buffer_size(w, h, IMAGE_UTIL_COLORSPACE_BGRA8888, &sizeBGRA8888);
	if ( IMAGE_UTIL_ERROR_NONE != err)
		return;
	static unsigned char *new_buff;
	free(new_buff);
	new_buff = malloc(sizeBGRA8888);

	/*
	 * Convert the image's colorspace
	 */
	err = image_util_convert_colorspace(new_buff, IMAGE_UTIL_COLORSPACE_BGRA8888, buf, w, h, colorspace);
	if ( IMAGE_UTIL_ERROR_NONE != err) {
		free(new_buff);
		return;
	}

	evas_object_hide(img);
	evas_object_image_size_set(img, w, h);
	evas_object_image_colorspace_set(img, EVAS_COLORSPACE_ARGB8888);
	evas_object_image_data_copy_set(img, (void*)new_buff);
	evas_object_image_reload(img);

	evas_object_image_data_update_add(img,0,0, w, h);
	evas_object_show(img);
}

struct raw_image{
	unsigned char *buffer;
	int size;
	int w;
	int h;
};

Evas_Object* img;

char *colorspace_str_tbl[] = 	{
	"IMAGE_UTIL_COLORSPACE_YV12", 			/**< YV12 */
	"IMAGE_UTIL_COLORSPACE_YUV422", 			/**< YUV422 */
	"IMAGE_UTIL_COLORSPACE_I420", 				/**< I420 */
	"IMAGE_UTIL_COLORSPACE_NV12", 				/**< NV12 */

	"IMAGE_UTIL_COLORSPACE_UYVY", 				/**< UYVY */
	"IMAGE_UTIL_COLORSPACE_YUYV", 				/**< YUYV */

	"IMAGE_UTIL_COLORSPACE_RGB565", 			/**< RGB565, high-byte is Blue */
	"IMAGE_UTIL_COLORSPACE_RGB888", 			/**< RGB888, high-byte is Blue */
	"IMAGE_UTIL_COLORSPACE_ARGB8888", 	/**< ARGB8888, high-byte is Blue */
	
	"IMAGE_UTIL_COLORSPACE_BGRA8888", 	/**< BGRA8888, high-byte is Alpha */
	"IMAGE_UTIL_COLORSPACE_RGBA8888", 	/**< RGBA8888, high-byte is Alpha */
	"IMAGE_UTIL_COLORSPACE_BGRX8888" 		/**< BGRX8888, high-byte is X */
};



bool decode_jpeg_cb( image_util_colorspace_e colorspace , void * user_data){
		int ret;
		unsigned char * buffer = NULL;
		int w, h;
		unsigned int size;

		const char *path = (const char*)user_data;
		
		printf("jpeg(%s) decode to [%s]\n", path, colorspace_str_tbl[colorspace] );
		

		w = h = 0;
		ret = image_util_decode_jpeg( path ,colorspace,  &buffer, &w, &h , &size );
		printf("image (%dx%d) - %dbyte , ret = %d(%x)\n", w,h, size, ret, ret);
		if( ret != 0 )
			return true;

		_display_buffer_as_efl_image( img, buffer, w, h, colorspace);
		sleep(2);
		return true;
	
}
void jpeg_decode_test( const char *path ){

	printf("****************\n");
	printf("JPEG DECODE TEST - start\n");
	printf("****************\n");	
	image_util_foreach_supported_jpeg_colorspace( decode_jpeg_cb , (void*)path);
	printf("****************\n");
	printf("JPEG DECODE TEST - end\n");
	printf("****************\n");	
		
}

bool encode_jpeg_cb( image_util_colorspace_e colorspace , void * user_data){
	struct raw_image *img_data = (struct raw_image *)user_data;
	unsigned char *buffer;
	unsigned int size;
	int ret;
	char filename[255];

	printf("jpeg encode from [%s]\n", colorspace_str_tbl[colorspace] );
	
	image_util_calculate_buffer_size(img_data->w,img_data->h, colorspace, &size);
	buffer = malloc(size);
	ret = image_util_convert_colorspace(buffer, colorspace , img_data->buffer , img_data->w, img_data->h , IMAGE_UTIL_COLORSPACE_BGRA8888);

	snprintf(filename, 255, "test_%s.jpg", colorspace_str_tbl[colorspace]);	
	ret = image_util_encode_jpeg(buffer, img_data->w , img_data->h , colorspace, 100, filename);
	printf("%s encode jpg %d\n",  colorspace_str_tbl[colorspace] , ret);	

	return true;
	
}

void jpeg_encode_test( const char *path ){
	printf("****************\n");
	printf("JPEG ENCODE TEST - start\n");
	printf("****************\n");	
	
	int w,h;
	struct raw_image img_data;
	evas_object_image_file_set(img, path , NULL);
	unsigned char* origin_buffer = evas_object_image_data_get(img, true);	
	evas_object_image_size_get(img, &w, &h);
	evas_object_resize(img, w,h);	
	img_data.buffer = origin_buffer;
	img_data.h = h;
	img_data.w = w;	
	image_util_foreach_supported_jpeg_colorspace( encode_jpeg_cb , (void*)&img_data);
	evas_object_image_data_set(img, origin_buffer);	

	printf("****************\n");
	printf("JPEG ENCODE TEST - end\n");
	printf("****************\n");	
	
}

void colorspace_convert_test( const char *path ){
	printf("****************\n");
	printf("colorspace_convert_test TEST - start\n");
	printf("****************\n");	

	
	int w,h;
	evas_object_image_file_set(img, path, NULL);
	unsigned char* origin_buffer = evas_object_image_data_get(img, true);	
	evas_object_image_size_get(img, &w, &h);
	evas_object_resize(img, w,h);	

	int i;
	int j;
	int ret;

	for( i = IMAGE_UTIL_COLORSPACE_YV12 ; i <= IMAGE_UTIL_COLORSPACE_BGRX8888 ; i++ ){
		unsigned char *buffer;
		unsigned int size;

		if( i == IMAGE_UTIL_COLORSPACE_BGRA8888 )
			continue;
		
		image_util_calculate_buffer_size(w, h, i,&size);
		buffer = malloc(size);
		ret = image_util_convert_colorspace(buffer, i , origin_buffer, w, h, IMAGE_UTIL_COLORSPACE_BGRA8888);
		printf("[%d] convert %s -> %s\n", ret , colorspace_str_tbl[IMAGE_UTIL_COLORSPACE_BGRA8888], colorspace_str_tbl[i]);
		
		for( j = IMAGE_UTIL_COLORSPACE_YV12 ; j <= IMAGE_UTIL_COLORSPACE_BGRX8888 ; j++){
			if( i == j )
				continue;
			unsigned char *buffer2;
			unsigned int size2;
			image_util_calculate_buffer_size(w, h, j,&size2);
			buffer2 = malloc(size2);
			ret = image_util_convert_colorspace(buffer2, j , buffer, w, h, i);
			printf("[%d] convert %s -> %s\n", ret , colorspace_str_tbl[i], colorspace_str_tbl[j]);
			free(buffer2);
		}
		
		free(buffer);		
			
	}



	evas_object_image_data_set(img, origin_buffer);	

	
	printf("****************\n");
	printf("colorspace_convert_test - end\n");
	printf("****************\n");	

}


void rotate_test( const char *path ){
	printf("****************\n");
	printf("rotate_test TEST - start\n");
	printf("****************\n");	
	int w,h,dw,dh;
	int ret;
	unsigned char *origin_buffer;
	unsigned int size;
	int i;
	

	w = h = 0;
	ret = image_util_decode_jpeg( path ,IMAGE_UTIL_COLORSPACE_RGB888,  &origin_buffer, &w, &h , &size );
	printf("image (%dx%d) - %dbyte , ret = %d(%x)\n", w,h, size, ret, ret);

	_display_buffer_as_efl_image(img, origin_buffer, w,h, IMAGE_UTIL_COLORSPACE_RGB888);
	sleep(2);

	unsigned char *dest_buffer = NULL;
	unsigned int nsize;
	dw = h;
	dh = w;
	image_util_calculate_buffer_size( dw,dh, IMAGE_UTIL_COLORSPACE_RGB888 , &nsize);
	dest_buffer = malloc(nsize);

	for(	i = 0 ; i < 10 ; i ++ ){
		ret = image_util_rotate(dest_buffer, &dw,&dh, IMAGE_UTIL_ROTATION_90, origin_buffer, w,h, IMAGE_UTIL_COLORSPACE_RGB888);
		printf("image_util_rotate ret = %d\n", ret);
		printf("%x%x%x\n", dest_buffer[0], dest_buffer[20], dest_buffer[44]);
		_display_buffer_as_efl_image(img, dest_buffer, dw,dh, IMAGE_UTIL_COLORSPACE_RGB888);
		sleep(2);
		int tw, th;
		unsigned char *tmp_buffer;
		tmp_buffer = dest_buffer;
		dest_buffer = origin_buffer;
		origin_buffer = tmp_buffer;
		tw = dw;
		th = dh;
		dw = w;
		dh = h;
		w = tw;
		h = th;
	}
	
	printf("****************\n");
	printf("transform_test TEST - end\n");
	printf("****************\n");	
	
}

void* test_main(void *arg){
	jpeg_decode_test("sample.jpg");
	//jpeg_encode_test("sample.jpg");
	//colorspace_convert_test("sample.jpg");
	rotate_test("sample.jpg");

	
	return NULL;
}



 Eina_Bool _window_update(void *data){
 	return 1;
}


int main(int argc, char ** argv)
{
	int w,h;
	elm_init(argc, argv);
	Evas_Object* win;


	win = elm_win_add(NULL, "VIDEO OVERLAY", ELM_WIN_BASIC);
	elm_win_title_set(win, "video overlay window");
	elm_win_borderless_set(win, EINA_TRUE);
	ecore_x_window_size_get(ecore_x_window_root_first_get(),	&w, &h);
	evas_object_resize(win, w, h);
	elm_win_indicator_state_set(win, EINA_TRUE);

	evas_object_show(win);	

	img = evas_object_image_add(evas_object_evas_get(win));
	printf("%d,%d\n", w,h);


	
	evas_object_resize(img, w,h);	
	evas_object_image_fill_set(img, 0, 0, w, h);	
	evas_object_show(img);	


	ecore_timer_add(0.1, _window_update, NULL);
	

	
	pthread_t gloop_thread;
	pthread_create(&gloop_thread, NULL, test_main,  NULL);

	elm_run();
	elm_shutdown();
	

	return 0;
}


