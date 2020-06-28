#pragma once

#include <opencv2\opencv.hpp>
#include <fstream>

#include "SearchContours.h"

using namespace std;
using namespace cv;

typedef struct Line_ {		// Line description y = alpha * x + beta
	double alpha;
	double beta;
}Line;

class FisheyeCalibration
{
public:
	FisheyeCalibration();
	~FisheyeCalibration();

	/* ��ȡ�ǵ� */
	bool FindCorners(const Mat& src, vector<Point2f>& corners);

	/* ������⵽�Ľǵ� */
	void DrawCorners(const Mat& src, const vector<Point2f>& corners);

	/* ��ʼ��������Ͻǵ����ά���� ����z=0 */
	void SetObjectPoints(vector<vector<Point3f>>& points);

	/* ���۽��� */
	void FisheyeCameraCalibration(Matx33d& intrinsicMatrix, Vec4d& distortionCoeffs, vector<Vec3d>& rotationVectors, vector<Vec3d>& translationVectors);

	/* ���۽������ */
	void EvaluateCalibrateRes();

	/* �������������ļ� */
	void SaveCalibrateRes(const string& filename);

	/* ��ȡ������� */
	bool ReadCalibrateRes(const string& filename);

	/* ��ӳ�� */
	Mat ImageRemap(const Mat& image, const double& scale);

	// ���н�������
	void RunCalibration();

	// ���۽��������������ļ�
	bool CalibrateAndSave2File(const int& totalImgNumber, const string& filename);

	// defisheye
	void Defisheye();

	Mat GetPerspectiveImgLeft(Mat& image);

	int GetImagePoints(cv::Mat &undist_img, uint num, std::vector<cv::Point2f> &img_points);

	void RunStitching();

	Mat GetPerspectiveImg(Mat& image);

	// ��������

private:
	const Size boardSize = Size(7, 7);		//�������ÿ�С��еĽǵ���
	//const Size boardSize = Size(9, 6);		//�������ÿ�С��еĽǵ���
	const int  imageNum = 13;				//��������ͼƬ������
	vector<vector<Point2f>>  cornersSeq;	//�����⵽�����нǵ�
	vector<Mat>  imageSeq;					//������Լ�⵽�ǵ��ͼƬ

	Size squareSize = Size(20, 20);
	vector<vector<Point3f>>  objectPoints;	// ���涨����Ͻǵ����ά����

	Matx33d intrinsicMatrix;				// ������ڲ�������
	Matx33d newIntrinsicMatrix;				// �µ�������ڲ�������
	Vec4d distortionCoeffs;					// �������4������ϵ����k1,k2,k3,k4
	vector<Vec3d> rotationVectors;          // ÿ��ͼ�����ת����
	vector<Vec3d> translationVectors;		// ÿ��ͼ���ƽ������

	Mat rotationVec;						// ��ת����
	Mat translationVec;						// ƽ������

	SearchContours contours;

	Line line_left[4];
	Line line_right[4];
	int cnt = 0;

private:
	inline Line GetAlphaBeta(cv::Point2f p1, cv::Point2f p2)
	{
		Line result;
		if (p1.x == p2.x) // If line is x = X, then alpha = INFINITY and beta = X.
		{
			result.alpha = INFINITY;
			result.beta = p1.x;
		}
		else // Otherwise
		{
			result.alpha = (p2.y - p1.y) / (p2.x - p1.x);
			result.beta = (p1.y * p2.x - p2.y * p1.x) / (p2.x - p1.x);
		}
		return result;
	}
	// �ж�����ĵ��Ƿ�Ϊ��Ч�����ݵ㣬����Ҫ�����ĵ�
	inline bool IsValidPoint(const Point2f& p, const Line& line_left, const Line& line_right)
	{
		// ������ͼ���ڲ�������y <= y_left && y <= y_right����Ҫ����
		double y_left = line_left.alpha*p.x + line_left.beta;
		double y_right = line_right.alpha*p.x + line_right.beta;

		if ((p.y <= y_left) && (p.y <= y_right))
		{
			return true;
		}
		
		return false;
	}

	inline void CutImage(Mat& image, const Line& line_left, const Line& line_right)
	{
		// �������������	
		for (int row = 0; row < image.rows; row++)
		{
			for (int col = 0; col < image.cols; col++)
			{
				if (IsValidPoint(Point2f(col, row), line_left, line_right))
				{
					image.at<Vec3b>(row, col)[0] = image.at<Vec3b>(row, col)[0];
					image.at<Vec3b>(row, col)[1] = image.at<Vec3b>(row, col)[1];
					image.at<Vec3b>(row, col)[2] = image.at<Vec3b>(row, col)[2];
				}
				else
				{
					image.at<Vec3b>(row, col)[0] = 0;
					image.at<Vec3b>(row, col)[1] = 0;
					image.at<Vec3b>(row, col)[2] = 0;
				}
			}
		}
	}
};

