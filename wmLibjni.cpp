#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include "WatermarkAPI.h"
#include "watermark_wmLib.h"

#define WATERMARK_WIDTH     256
#define WATERMARK_HEIGHT    256

extern "C" {
	
	//read the region of the img and change ycrcb, then return y channel
	cv::Mat GetImageData(cv::Mat& img, cv::Rect region)
	{
		cv::Mat data = img(region);
		cv::Mat imgycc;

		cv::cvtColor(data, imgycc, cv::COLOR_BGR2YCrCb);
		std::vector<cv::Mat> imgchannels;
		
		cv::split(imgycc, imgchannels);
		cv::Mat result = imgchannels[0].clone();

		return result;
	}
	
	//put ychannel image back to image
	void PutImageData(cv::Mat& img, cv::Rect region, cv::Mat& ychannel)
	{
		cv::Mat data = 	img(region);
		cv::Mat imgycc;
		cv::cvtColor(data, imgycc, cv::COLOR_BGR2YCrCb);
		std::vector<cv::Mat> imgchannels;

		cv::split(imgycc, imgchannels);
		ychannel.copyTo(imgchannels[0]);
		cv::merge(imgchannels, imgycc);

		cv::cvtColor(imgycc, data, cv::COLOR_YCrCb2BGR);
		data.copyTo(img(region));
	}

	//imgfile : image filename which watermark will embed to
	//wminfo  : watermark info, eight numbers, each number '1'-'6'
	//x,y	  : position of the watermark
	//strength: strength of watermark
	JNIEXPORT jint JNICALL Java_watermark_wmLib_addwm(JNIEnv * env, 
											jobject obj, 
											jbyteArray imgfile, 
											jbyteArray wminfo,
											jint x, 
											jint y,
											jint strength)
	{
		jbyte * filebuf;
		jbyte * wmbuf;

		filebuf = env->GetByteArrayElements(imgfile,JNI_FALSE);
		wmbuf   = env->GetByteArrayElements(wminfo,JNI_FALSE);

		if (filebuf == NULL || wmbuf == NULL)
		{
			fprintf(stderr,"image file name or watermark must not empty\n");
			return -1;
		}
	
		//read image file
		cv::Mat img = cv::imread((char*)filebuf);
		
		if (img.empty())
		{
			fprintf(stderr,"image file %s can't be opened, please check!\n",filebuf);
			return -2;
		}
			
		if (img.rows < WATERMARK_HEIGHT || img.cols < WATERMARK_WIDTH ||
			img.channels() != 3	)
		{
			fprintf(stderr,"image file size or channels not correct, must >= 256X256, RGB\n");
			return -3;
		}
		
		//maybe should check the region
		cv::Rect rg(x,y,WATERMARK_WIDTH,WATERMARK_HEIGHT);
		
		cv::Mat wmImg = GetImageData(img, rg);
		AddWatermark256((unsigned char*)wmImg.data, 
						(char*)wmbuf, 
						(int)strength);
	

		PutImageData(img, rg, wmImg);
		
		//save image file
		std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 100};
		cv::imwrite((char*)filebuf,img, params);
	  char newfile[256];
	  strcpy(newfile,(char*)filebuf);
	  strcat(newfile,".wm.jpg");
    cv::imwrite(newfile, wmImg, params);
    fprintf(stderr,"writing temp file %s\n",newfile);

		env->ReleaseByteArrayElements(imgfile, filebuf, 0);
		env->ReleaseByteArrayElements(wminfo, wmbuf, 0);			
		
		return 0;
	}


	JNIEXPORT jint JNICALL Java_watermark_wmLib_detwm(JNIEnv * env, 
											jobject obj, 
											jbyteArray imgfile, 
											jbyteArray wminfo,
											jint x,
											jint y)
	{

		jbyte * filebuf;
		jbyte * wmbuf;

		filebuf = env->GetByteArrayElements(imgfile,JNI_FALSE);
		wmbuf   = env->GetByteArrayElements(wminfo,JNI_FALSE);

		if (filebuf == NULL || wmbuf == NULL)
		{
			fprintf(stderr,"image file name or watermark must not empty\n");
			return -1;
		}
	
		//read image file
		cv::Mat img = cv::imread((char*)filebuf);
		
		if (img.empty())
		{
			fprintf(stderr,"image file %s can't be opened, please check!\n",filebuf);
			return -2;
		}
			
		if (img.rows < WATERMARK_HEIGHT || img.cols < WATERMARK_WIDTH ||
			img.channels() != 3	)
		{
			fprintf(stderr,"image file size or channels not correct, must >= 256X256, RGB\n");
			return -3;
		}
		
		//maybe should check the region
		cv::Rect rg(x,y,WATERMARK_WIDTH,WATERMARK_HEIGHT);
		
		cv::Mat wmImg = GetImageData(img, rg);

		char wm[9] = {0};
		
		DetectWatermark256((unsigned char*)wmImg.data, 
						   wm);
		
		fprintf(stderr, "DetectWatermark256 return : %s\n", wm);

		memcpy(wmbuf, wm, 9);
		
		env->ReleaseByteArrayElements(imgfile, filebuf, 0);
		env->ReleaseByteArrayElements(wminfo, wmbuf, 0);

		return 0;

	}




}
