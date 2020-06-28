#pragma once

#if defined(_DLL_EXPORTS) // inside DLL
#   define DLL_API   __declspec(dllexport)
#else // outside DLL
#   define DLL_API   __declspec(dllimport)
#endif  // XYZLIBRARY_EXPORT

#include <opencv2\opencv.hpp>
//#include <experimental/filesystem>
#include <fstream>

class SurroundView
{
public:
	/************************����궨�Լ�����****************************/
	virtual int Init(int nSrcHeight, int nSrcWidth) = 0;
	virtual cv::cuda::GpuMat Undistort(cv::cuda::GpuMat &mSrcImg) = 0;
	virtual cv::Mat Undistort(cv::Mat &mSrcImg) = 0;

	/************************��ͶӰ�任*******************************/
	virtual cv::Mat PerspectiveTransform(cv::InputArray aInput, cv::Point2f *pSrcPoints, cv::Point2f *pDstPoints, cv::Size sOutputSize, int nOrientation) = 0;

	/*************************ͼ��ƴ��**********************************/
	virtual cv::Mat ImageStitching(int nWidth, int nHeight, cv::Mat aInputLeft, cv::Mat aInputRight, cv::Mat aInputFront, cv::Mat aInputBack,
		std::vector<cv::Point> vPtsInputLeft, std::vector<cv::Point> vPtsInputRight, std::vector<cv::Point> vPtsInputFront, std::vector<cv::Point> vPtsInputBack) = 0;

};

extern "C" DLL_API SurroundView *GetStitching();