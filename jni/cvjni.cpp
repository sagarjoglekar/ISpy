/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include "join.h"
#include "cv.h"
#include "cxcore.h"
#include "bmpfmt.h"
#define ANDROID_LOG_VERBOSE ANDROID_LOG_DEBUG
#define LOG_TAG "CVJNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif
IplImage* pImage = NULL;
IplImage* loadPixels(int* pixels, int width, int height);
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data,jint width, jint height);
//void detect_and_draw( IplImage* image);
IplImage* detect_and_draw( IplImage* image);

int element_array[3];
static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;
static CvHaarClassifierCascade* nested_cascade = 0;
int use_nested_cascade = 0;
int scale = 2;
const char* cascade_name = "/mnt/sdcard/haarcascade_frontalface_alt_tree.xml";
int num;


int f=0;
int a,b,W,H;
int repeat=0;
int element[4];
int face_no=0;
int frames=400;
int people=1;
int Cx,Cy,R;
int flag=0;
JNIEXPORT void JNICALL Java_ispy_main_OpenCV_extractSURFFeature(
		JNIEnv* env, jobject thiz) {
	IplImage *pWorkImage=cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);
	cvCvtColor(pImage,pWorkImage,CV_BGR2GRAY);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
	CvSURFParams params = cvSURFParams(2000, 0);
	cvExtractSURF( pWorkImage, 0, &imageKeypoints, &imageDescriptors, storage, params );
	// show features
	for( int i = 0; i < imageKeypoints->total; i++ )
	{
		CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem( imageKeypoints, i );
		CvPoint center;
		int radius;
		center.x = cvRound(r->pt.x);
		center.y = cvRound(r->pt.y);
		radius = cvRound(r->size*1.2/9.*2);
		cvCircle( pImage, center, radius, CV_RGB(255,0,0), 1, CV_AA, 0 );
	}
	cvReleaseImage(&pWorkImage);
	cvReleaseMemStorage(&storage);
}

JNIEXPORT jboolean JNICALL Java_ispy_main_OpenCV_setSourceImage(
		JNIEnv* env, jobject thiz, jintArray photo_data, jint width,
		jint height) {
	if (pImage != NULL) {
		cvReleaseImage(&pImage);
		pImage = NULL;
	}
	pImage = getIplImageFromIntArray(env, photo_data, width, height);
	if (pImage == NULL) {
		return 0;
	}
	LOGI("Load Image Done.");
	return 1;
}
JNIEXPORT jbooleanArray JNICALL Java_ispy_main_OpenCV_getSourceImage(
		JNIEnv* env, jobject thiz) {
	if (pImage == NULL) {
		LOGE("No source image.");
		return 0;
	}
	cvFlip(pImage);
	int width = pImage->width;
	int height = pImage->height;
	int rowStep = pImage->widthStep;
	int headerSize = 54;
	int imageSize = rowStep * height;
	int fileSize = headerSize + imageSize;
	unsigned char* image = new unsigned char[fileSize];
	struct bmpfile_header* fileHeader = (struct bmpfile_header*) (image);
	fileHeader->magic[0] = 'B';
	fileHeader->magic[1] = 'M';
	fileHeader->filesz = fileSize;
	fileHeader->creator1 = 0;
	fileHeader->creator2 = 0;
	fileHeader->bmp_offset = 54;
	struct bmp_dib_v3_header_t* imageHeader =
			(struct bmp_dib_v3_header_t*) (image + 14);
	imageHeader->header_sz = 40;
	imageHeader->width = width;
	imageHeader->height = height;
	imageHeader->nplanes = 1;
	imageHeader->bitspp = 24;
	imageHeader->compress_type = 0;
	imageHeader->bmp_bytesz = imageSize;
	imageHeader->hres = 0;
	imageHeader->vres = 0;
	imageHeader->ncolors = 0;
	imageHeader->nimpcolors = 0;
	memcpy(image + 54, pImage->imageData, imageSize);
	jbooleanArray bytes = env->NewBooleanArray(fileSize);
	if (bytes == 0) {
		LOGE("Error in creating the image.");
		delete[] image;
		return 0;
	}
	env->SetBooleanArrayRegion(bytes, 0, fileSize, (jboolean*) image);
	delete[] image;
	return bytes;
}

JNIEXPORT jint JNICALL Java_ispy_main_OpenCV_facedetect(JNIEnv* env, jobject thiz)
{
	//int result[9];
	//result = (*env)->NewIntArray(env, 9);

/*
	if (result == NULL) {
	     return NULL;
	 }
*/
	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
	if(!cascade)
	{
		LOGE("Error in Opening cascade classifier") ;
	}
	storage = cvCreateMemStorage(0);
	IplImage *pWorkImage=cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);
	cvCvtColor(pImage,pWorkImage,CV_BGR2GRAY);




	if(num==1){

		LOGE("****************************Tracking*************************************");
			track(pImage,flag,Cx,Cy,R);
			flag=1;


		}

	if(num==0){
		LOGE("****************************Detecting Faces**************************");
	pWorkImage = detect_and_draw( pImage );
	}

	 //(*env)->SetIntArrayRegion(env, result, 0, 9, coord);
	 LOGE("Value returning");
     return num;

}

JNIEXPORT jint JNICALL Java_ispy_main_OpenCV_getX(JNIEnv* env, jobject thiz)
{
	if(flag==1){
		Cx=returnX();
		return Cx;
	}
	return Cx;
}

JNIEXPORT jint JNICALL Java_ispy_main_OpenCV_getY(JNIEnv* env, jobject thiz)

{

	if(flag==1){
		Cy=returnY();
	}
    return Cy;
}

JNIEXPORT jint JNICALL Java_ispy_main_OpenCV_getR(JNIEnv* env, jobject thiz)

{

	if(flag==1){
		R=returnR();
	}
    return R;
}

IplImage* loadPixels(int* pixels, int width, int height) {
	int x, y;
	IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	unsigned char* base = (unsigned char*) (img->imageData);
	unsigned char* ptr;
	for (y = 0; y < height; y++) {
		ptr = base + y * img->widthStep;
		for (x = 0; x < width; x++) {
			// blue
			ptr[3 * x] = pixels[x + y * width] & 0xFF;
			// green
			ptr[3 * x + 1] = pixels[x + y * width] >> 8 & 0xFF;
			// blue
			ptr[3 * x + 2] = pixels[x + y * width] >> 16 & 0xFF;
		}
	}
	return img;
}
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data,
		jint width, jint height) {
	int *pixels = env->GetIntArrayElements(array_data, 0);
	if (pixels == 0) {
		LOGE("Error getting int array of pixels.");
		return 0;
	}
	IplImage *image = loadPixels(pixels, width, height);
	env->ReleaseIntArrayElements(array_data, pixels, 0);
	if (image == 0) {
		LOGE("Error loading pixel array.");
		return 0;
	}
	return image;
}

IplImage * detect_and_draw( IplImage* img )
{
	static CvScalar colors[] =
		    {
		        {{0,0,255}},
		        {{0,128,255}},
		        {{0,255,255}},
		        {{0,255,0}},
		        {{255,128,0}},
		        {{255,255,0}},
		        {{255,0,0}},
		        {{255,0,255}}
		    };

		    IplImage *gray, *small_img;
		    int i, j;

		    gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
		    small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
		                         cvRound (img->height/scale)), 8, 1 );

		    cvCvtColor( img, gray, CV_BGR2GRAY );
		    cvResize( gray, small_img, CV_INTER_LINEAR );
		    cvEqualizeHist( small_img, small_img );
		    cvClearMemStorage( storage );

		    if( cascade )
		    {
		        double t = (double)cvGetTickCount();
		        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
		                                            1.1, 2, 0
		                                            //|CV_HAAR_FIND_BIGGEST_OBJECT
		                                            //|CV_HAAR_DO_ROUGH_SEARCH
		                                            |CV_HAAR_DO_CANNY_PRUNING
		                                            //|CV_HAAR_SCALE_IMAGE
		                                            ,
		                                            cvSize(30, 30) );
		        t = (double)cvGetTickCount() - t;
		        num=faces->total;
		        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		        for( i = 0; i < (faces ? faces->total : 0); i++ )
		        {
		            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
		            CvMat small_img_roi;
		            CvSeq* nested_objects;
		            CvPoint center;
		            CvScalar color = colors[i%8];
		            int radius;
		            center.x = cvRound((r->x + r->width*0.5)*scale);
		            center.y = cvRound((r->y + r->height*0.5)*scale);
		            radius = cvRound((r->width + r->height)*0.25*scale);
		            Cx=center.x;
		            Cy=center.y;
		            R=radius;
		            cvCircle( img, center, radius, color, 3, 8, 0 );
		            if( !nested_cascade )
		                continue;
		            cvGetSubRect( small_img, &small_img_roi, *r );
		            nested_objects = cvHaarDetectObjects( &small_img_roi, nested_cascade, storage,
		                                        1.1, 2, 0
		                                        //|CV_HAAR_FIND_BIGGEST_OBJECT
		                                        //|CV_HAAR_DO_ROUGH_SEARCH
		                                        //|CV_HAAR_DO_CANNY_PRUNING
		                                        //|CV_HAAR_SCALE_IMAGE
		                                        ,
		                                        cvSize(0, 0) );
		            for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
		            {
		                CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
		                center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
		                center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
		                radius = cvRound((nr->width + nr->height)*0.25*scale);
		                cvCircle( img, center, radius, color, 3, 8, 0 );
		            }
		        }
		    }
	   cvReleaseImage( &gray );
	   cvReleaseImage( &small_img );
	   //fp = fopen("/sdcard/test.jpg","w+");
	   return img;
}


#ifdef __cplusplus
}
#endif

