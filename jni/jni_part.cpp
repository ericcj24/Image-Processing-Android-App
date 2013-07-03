#include <jni.h>
#include <opencv2/core/core.hpp>
#include <android/log.h>
#include <math.h>

using namespace std;
using namespace cv;

extern "C" {

// function declaration for manual color conversion
int convertYUVtoARGB(int, int, int);

JNIEXPORT void JNICALL Java_org_ece420_lab5_Sample4View_YUV2RGB(JNIEnv*, jobject, jlong addrYuv, jlong addrRgba)
{
    Mat* pYUV=(Mat*)addrYuv;
    Mat* pRGB=(Mat*)addrRgba;
    /*************** INSERT CODE TO CONVERT AN ENTIRE IMAGE FROM FROM YUV420sp TO ARGB ******************/
    Size temp = pYUV->size();//(*pYUV).size()
    int width = temp.width;
    int height = temp.height;
    int u, v, y1, y2, y3, y4;

    for(int i=0; i<height/3;i+=1)
    {
    	for (int j=0; j<width;j+=2)
    	{
        	y1 = ((int)((*pYUV).at<uchar>(2*i,j))) &0xff;
        	y2 = ((int)((*pYUV).at<uchar>(2*i+1,j))) &0xff;
        	y3 = ((int)((*pYUV).at<uchar>(2*i,j+1))) &0xff;
        	y4 = ((int)((*pYUV).at<uchar>(2*i+1,j+1))) &0xff;
        	u = (((int)((*pYUV).at<uchar>(i+2*height/3,j))) &0xff)-128;
        	v = (((int)((*pYUV).at<uchar>(i+2*height/3,j+1))) &0xff)-128;


        	(*pRGB).at<int>(2*i,j) = convertYUVtoARGB(y1, u, v);
        	(*pRGB).at<int>(2*i+1,j) = convertYUVtoARGB(y2, u, v);
        	(*pRGB).at<int>(2*i,j+1) = convertYUVtoARGB(y3, u, v);
        	(*pRGB).at<int>(2*i+1,j+1) = convertYUVtoARGB(y4, u, v);
    	}
    }

    /*************** INSERT CODE TO CONVERT AN ENTIRE IMAGE FROM FROM YUV420sp TO ARGB ******************/

}

JNIEXPORT void JNICALL Java_org_ece420_lab5_Sample4View_HistEQ(JNIEnv* env, jobject thiz, jlong addrYuv, jlong addrRgba)
{
    Mat* pYUV=(Mat*)addrYuv;
    Mat* pRGB=(Mat*)addrRgba;
    /*************** Jian Chen ******************/
    Size temp = pYUV->size();//(*pYUV).size()
    int width = temp.width;
    int height = temp.height;
    //int size = width*height*2/3;
    int u, v, y1, y2, y3, y4;
    int cdf[256];
    for (int i=0;i<256;i++){
    	cdf[i]=0;
    }
    int temp1;
    for(int i=0; i<2*height/3;i+=1)
    {
    	for (int j=0; j<width;j+=1)
    	{
    		temp1 = (int)((*pYUV).at<uchar>(i,j));
    		cdf[temp1] = cdf[temp1] + 1;
    	}
    }

    for (int i=1; i<256;i++)
    {
    	cdf[i] = cdf[i-1] + cdf[i];
    }

    // normalize the cdf
    for (int i=0; i<256;i++)
    {
    	cdf[i] =(int)((cdf[i]/(float)cdf[255])*255);
    }

    // half of the screen to histogram
    int index;
    for(int i=0; i<2*height/3-1;i+=1)
    {
    	for (int j=0; j<width/2-1;j+=1)
    	{

    		index = (int)((*pYUV).at<uchar>(i,j));
    		(*pYUV).at<uchar>(i,j) = (uchar)(cdf[index]);
    	}
    }

    //color RGB
    for(int i=0; i<height/3;i+=1)
    {
    	for (int j=0; j<width;j+=2)
    	{
        	y1 = ((int)((*pYUV).at<uchar>(2*i,j))) &0xff;
        	y2 = ((int)((*pYUV).at<uchar>(2*i+1,j))) &0xff;
        	y3 = ((int)((*pYUV).at<uchar>(2*i,j+1))) &0xff;
        	y4 = ((int)((*pYUV).at<uchar>(2*i+1,j+1))) &0xff;
        	u = (((int)((*pYUV).at<uchar>(i+2*height/3,j))) &0xff)-128;
        	v = (((int)((*pYUV).at<uchar>(i+2*height/3,j+1))) &0xff)-128;


        	(*pRGB).at<int>(2*i,j) = convertYUVtoARGB(y1, u, v);
        	(*pRGB).at<int>(2*i+1,j) = convertYUVtoARGB(y2, u, v);
        	(*pRGB).at<int>(2*i,j+1) = convertYUVtoARGB(y3, u, v);
        	(*pRGB).at<int>(2*i+1,j+1) = convertYUVtoARGB(y4, u, v);
    	}
    }
    /*************** Jian Chen ******************/

// EXAMPLE SHOWING HOW TO DISPLAY DEBUGGING INFORMATION TO ADB LOGCAT
//        char buf[128];
//        sprintf(buf,"iteration i=%d",i);
//        __android_log_write(ANDROID_LOG_INFO, "Example Logging", buf);
}

int convertYUVtoARGB(int y, int u, int v) {
	/**************** INSERT CODE TO CONVERT A YUV PIXEL TO A 32-BIT INT REPRESENTING AN ARGB PIXEL ****************/
	int r,g,b;
	r = y + (int)1.402f*v;
	g = y - (int)(0.344f*u +0.714f*v);
	b = y + (int)1.772f*u;
	r = r>255? 255 : r<0 ? 0 : r;// saturation
	g = g>255? 255 : g<0 ? 0 : g;
	b = b>255? 255 : b<0 ? 0 : b;
	return 0xff000000 | (r<<16) | (g<<8) | b;// 8 bit unsigned compose to 32 bit int
	/**************** INSERT CODE TO CONVERT A YUV PIXEL TO A 32-BIT INT REPRESENTING AN ARGB PIXEL ****************/

}

}

