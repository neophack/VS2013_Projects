#pragma once

#include <opencv2\opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

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

	// ���۽��������������ļ�
	bool CalibrateAndSave2File(const int& totalImgNumber, const string& fisheyeFileName, const string& caliResFile);

	

	void RunCalibration();

	// defisheye
	void Defisheye();

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
};

