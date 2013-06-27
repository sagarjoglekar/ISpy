

#include "cv.h"
#include <android/log.h>
#include <stdio.h>

#define LOG_TAG "CVJNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

int backproject_mode = 0;
int select_object = 0;
int track_object = 0;
int show_hist = 1;
CvPoint origin;
CvRect selection;
CvRect track_window;
CvBox2D track_box;
CvConnectedComp track_comp;
int hdims = 16;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
int vmin = 10, vmax = 256, smin = 30;


CvScalar hsv2rgb( float hue )
{
    int rgb[3], p, sector;
    static const int sector_data[][3]=
        {{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= sector & 1 ? 255 : 0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

int track( IplImage* frame, int flag,int Cx,int Cy,int R )
{

    {

        int i, bin_w, c;

        LOGE("#######################Check1############################");

        if( !image )
        {
            /* allocate all the buffers */
            image = cvCreateImage( cvGetSize(frame), 8, 3 );
            image->origin = frame->origin;
            hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
            hue = cvCreateImage( cvGetSize(frame), 8, 1 );
            mask = cvCreateImage( cvGetSize(frame), 8, 1 );
            backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
            histimg = cvCreateImage( cvSize(320,200), 8, 3 );
            cvZero( histimg );
            LOGE("######################Check2###########################");
        }

        cvCopy( frame, image, 0 );
        cvCvtColor( image, hsv, CV_BGR2HSV );


        {
            int _vmin = vmin, _vmax = vmax;

            cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
                        cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
            cvSplit( hsv, hue, 0, 0, 0 );
            LOGE("###########################Check3######################");
            if(flag==0)
            {
            	LOGE("###############Initialized#############################");
				selection.x=Cx-R;
				selection.y=Cy-R;
				selection.height=2*R;
				selection.width=2*R;
                float max_val = 0.f;
                cvSetImageROI( hue, selection );
                cvSetImageROI( mask, selection );
                cvCalcHist( &hue, hist, 0, mask );
                cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );
                cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 );
                cvResetImageROI( hue );
                cvResetImageROI( mask );
                track_window = selection;
                track_object = 1;

                cvZero( histimg );
                bin_w = histimg->width / hdims;
                for( i = 0; i < hdims; i++ )
                {
                    int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
                    CvScalar color = hsv2rgb(i*180.f/hdims);
                    cvRectangle( histimg, cvPoint(i*bin_w,histimg->height),
                                 cvPoint((i+1)*bin_w,histimg->height - val),
                                 color, -1, 8, 0 );
                }
                LOGE("##############Check4#########################");
            }
            LOGE("##############Check5#########################");
            cvCalcBackProject( &hue, backproject, hist );
            cvAnd( backproject, mask, backproject, 0 );
            cvCamShift( backproject, track_window,
                        cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
                        &track_comp, &track_box );
            track_window = track_comp.rect;
            char buffer[50];
            sprintf(buffer,"vals= %d %d and %d",track_window.x,track_window.y,track_window.width);
            LOGE(buffer);
            if( backproject_mode )
                cvCvtColor( backproject, image, CV_GRAY2BGR );
            if( image->origin )
                track_box.angle = -track_box.angle;
            cvEllipseBox( image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
        }

        if( select_object && selection.width > 0 && selection.height > 0 )
        {
            cvSetImageROI( image, selection );
            cvXorS( image, cvScalarAll(255), image, 0 );
            cvResetImageROI( image );
        }

        LOGE("!!!!!!!!!!!!!!!!!!Done Tracking!!!!!!!!!!!!!!!!!!!!!!!!!!!!");


    }



    return 0;
}




int returnX(){

	return track_window.x+(track_window.width/2);
}

int returnY(){

	return track_window.y+(track_window.width/2);
}

int returnR(){

	return (track_window.width/2);
}





