#include "FisheyeCalibration.h"

FisheyeCalibration::FisheyeCalibration()
{
	cout << "FisheyeCalibration" << endl;
}


FisheyeCalibration::~FisheyeCalibration()
{
	cout << "~FisheyeCalibration" << endl;
}

void FisheyeCalibration::Defisheye()
{
	// ��ȡ�ڲξ���ͻ���ϵ��
	if (!ReadCalibrateRes("calib_results.txt"))
	{
		cout << "No calibration data!" << endl;
	}

	// ��ȡ��Ҫƴ�ӵ�ǰ������ͼƬ
	Mat left = ImageRemap(imread("./ov10635/left.jpg"), 2);

	//Mat left_cut = GetPerspectiveImgLeft(left);

	for (int i = 0; i < 4; i++) 
	{
		string src_name = "./image/src_" + to_string(i + 1) + ".jpg";
		string dst_name = "./image/cali_" + to_string(i + 1) + ".jpg";
		Mat dst_image = ImageRemap(imread(src_name), 2);
		imshow(dst_name, dst_image);
		imwrite(dst_name, dst_image);
	}

	waitKey(0);
}

bool FisheyeCalibration::CalibrateAndSave2File(const int& totalImgNumber, const string& fisheyeFileName, const string& caliResFile)
{
	imageSeq.clear();
	cornersSeq.clear();
	
	// ��ȡͼƬ����ȡ�ǵ�
	for (int i = 0; i < totalImgNumber; i++)
	{
		String imageName = fisheyeFileName + "_" + to_string(i) + ".jpg";

		Mat image = imread(imageName);

		if (image.empty()) 
		{
			cout << "Image: " << imageName << "is empty!" << endl;
			continue;
		}

		vector<Point2f> corners;

		if (FindCorners(image, corners)) 
		{
			cout << "Image: " << imageName << " find Chessboard Corners success!" << endl;
			cornersSeq.push_back(corners);
			imageSeq.push_back(image);

			//DrawCorners(image, corners);
		}
	}

	// ���ö���������������ϵ�е�����
	SetObjectPoints(objectPoints);

	FisheyeCameraCalibration(intrinsicMatrix, distortionCoeffs, rotationVectors, translationVectors);

	EvaluateCalibrateRes();

	SaveCalibrateRes(caliResFile);

	return true;	
}

void FisheyeCalibration::RunCalibration()
{	
	if (!ReadCalibrateRes("calib_results.txt"))
	{
		// ��ȡͼƬ����ȡ�ǵ�
		for (int i = 0; i < imageNum; i++)
		{
			String imageName = "./fisheye_img/frame1_" + to_string(i) + ".jpg";

			Mat image = imread(imageName);

			if (image.empty())
			{
				cout << "Image: " << imageName << "is empty!" << endl;
				continue;
			}

			vector<Point2f> corners;

			if (FindCorners(image, corners))
			{
				cout << "Image: " << imageName << " find Chessboard Corners success!" << endl;
				cornersSeq.push_back(corners);
				imageSeq.push_back(image);

				//DrawCorners(image, corners);
			}
		}

		// ���ö���������������ϵ�е�����
		SetObjectPoints(objectPoints);

		FisheyeCameraCalibration(intrinsicMatrix, distortionCoeffs, rotationVectors, translationVectors);

		EvaluateCalibrateRes();

		SaveCalibrateRes("calib_results.txt");
	}	
}

Mat FisheyeCalibration::ImageRemap(const Mat& image, const double& scale)
{
	Mat imageRemap;
	Size imageSize = image.size();
	Mat mapx = Mat(imageSize, CV_32FC1);
	Mat mapy = Mat(imageSize, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);

	Matx33d newK = intrinsicMatrix;
	newK(0, 0) = intrinsicMatrix(0, 0) / scale; // fx
	newK(1, 1) = intrinsicMatrix(1, 1) / scale; // fy

	fisheye::initUndistortRectifyMap(intrinsicMatrix, distortionCoeffs, R, newK, imageSize, CV_32FC1, mapx, mapy);
	//fisheye::initUndistortRectifyMap(intrinsicMatrix, distortionCoeffs, R, intrinsicMatrix, imageSize, CV_32FC1, mapx, mapy);	
	
	remap(image, imageRemap, mapx, mapy, INTER_LINEAR);

	return imageRemap;
}

bool FisheyeCalibration::ReadCalibrateRes(const string& filename)
{
	//string file = "./calibration_result/" + filename;

	string file = filename;
	ifstream fin(file);

	if (!fin.good())
	{
		std::cout << "File " << filename << " not found" << std::endl;
		return false;
	}

	string line;
	int lineNum = 0;
	double val;

	while (getline(fin, line))
	{
		if ((line.length() > 1) && (line.at(0) != '#'))
		{
			stringstream str_stream(line);
			int i = 0;

			switch (lineNum++)
			{
			case 0:	// camera intrinsic matrix
				while (str_stream >> val)
				{
					intrinsicMatrix(i / 3, i % 3) = val;
					i++;
				}
				break;
			case 1: // distortion coeffs
				while (str_stream >> val)
				{
					distortionCoeffs(i++) = val;
				}
				break;
			case 2: // camera new intrinsic matrix
				while (str_stream >> val)
				{
					newIntrinsicMatrix(i / 3, i % 3) = val;
					i++;
				}
				break;

			default:
				return false;
				break;
			}
		}
	}

	return (lineNum == 2 ? true : false);
}

void FisheyeCalibration::SaveCalibrateRes(const string& filename)
{
	std::cout << "Save calibration result file: " << filename << std::endl;
	// calib_results_1.txt
	string file = filename;
	ofstream fout(file);

	fout << "#camera intrinsic matrix" << endl;
	fout << intrinsicMatrix(0, 0) << ' ' << intrinsicMatrix(0, 1) << ' ' << intrinsicMatrix(0, 2) << ' '
		<< intrinsicMatrix(1, 0) << ' ' << intrinsicMatrix(1, 1) << ' ' << intrinsicMatrix(1, 2) << ' '
		<< intrinsicMatrix(2, 0) << ' ' << intrinsicMatrix(2, 1) << ' ' << intrinsicMatrix(2, 2) << endl << endl;

	fout << "#distortion coeffs" << endl;
	fout << distortionCoeffs(0) << ' ' << distortionCoeffs(1) << ' ' << distortionCoeffs(2) << ' ' << distortionCoeffs(3) << endl << endl;

	//fout << "#camera new intrinsic matrix" << endl;

	//fout << newIntrinsicMatrix(0, 0) << ' ' << newIntrinsicMatrix(0, 1) << ' ' << newIntrinsicMatrix(0, 2) << ' '
	//	<< newIntrinsicMatrix(1, 0) << ' ' << newIntrinsicMatrix(1, 1) << ' ' << newIntrinsicMatrix(1, 2) << ' '
	//	<< newIntrinsicMatrix(2, 0) << ' ' << newIntrinsicMatrix(2, 1) << ' ' << newIntrinsicMatrix(2, 2) << endl;
}

void FisheyeCalibration::EvaluateCalibrateRes()
{
	cout << "��ʼ���۶�����������������" << endl;
	double total_err = 0.0;                  /* ����ͼ���ƽ�������ܺ� */
	double err = 0.0;                        /* ÿ��ͼ���ƽ����� */
	vector<Point2f>  image_points2;          /****   �������¼���õ���ͶӰ��    ****/

	vector<int>  point_counts;

	for (int i = 0; i< imageSeq.size(); i++)
	{
		point_counts.push_back(boardSize.width*boardSize.height);
	}

	cout << "ÿ��ͼ��Ķ�����" << endl;

	for (int i = 0; i<imageSeq.size(); i++)
	{
		vector<Point3f> tempPointSet = objectPoints[i];
		/****    ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ��     ****/
		fisheye::projectPoints(tempPointSet, image_points2, rotationVectors[i], translationVectors[i], intrinsicMatrix, distortionCoeffs);
		
		/* �����µ�ͶӰ��;ɵ�ͶӰ��֮������*/
		vector<Point2f> tempImagePoint = cornersSeq[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (size_t i = 0; i != tempImagePoint.size(); i++)
		{
			image_points2Mat.at<Vec2f>(0, i) = Vec2f(image_points2[i].x, image_points2[i].y);
			tempImagePointMat.at<Vec2f>(0, i) = Vec2f(tempImagePoint[i].x, tempImagePoint[i].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err += err /= point_counts[i];
		cout << "��" << i + 1 << "��ͼ���ƽ����" << err << "����" << endl;		
	}
	cout << "����ƽ����" << total_err / imageSeq.size() << "����" << endl;
	cout << "������ɣ�" << endl;
}

void FisheyeCalibration::FisheyeCameraCalibration(Matx33d& intrinsicMatrix, Vec4d& distortionCoeffs, vector<Vec3d>& rotationVectors, vector<Vec3d>& translationVectors)
{
	Size imageSize = imageSeq[0].size();

	int flags = 0;
	flags |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
	flags |= cv::fisheye::CALIB_CHECK_COND;
	flags |= cv::fisheye::CALIB_FIX_SKEW;

	/* �ǵ��ڱ궨���ڵ����꣬ �ǵ���ͼ���е����꣬ ͼƬ��С�� �ڲξ��� �ĸ���������� �������ת������ �����ƽ�������� �������� ���*/
	double totalError = fisheye::calibrate(objectPoints, cornersSeq, imageSize, intrinsicMatrix, distortionCoeffs, rotationVectors, translationVectors, flags, TermCriteria(3, 20, 1e-6));
	cout << "Total error is: " << totalError << endl;
	//fisheye::estimateNewCameraMatrixForUndistortRectify(intrinsicMatrix, distortionCoeffs, imageSize, Matx33d::eye(), newIntrinsicMatrix, 0, imageSize, 1);
}

void FisheyeCalibration::SetObjectPoints(vector<vector<Point3f>>& points)
{
	if (cornersSeq.empty())
	{
		return;
	}

	points.clear();

	for (int i = 0; i < cornersSeq.size(); i++)
	{
		vector<Point3f> tempPointSet;
		for (int i = 0; i < boardSize.height; i++)
		{
			for (int j = 0; j < boardSize.width; j++)
			{
				/* ���趨��������������ϵ��z=0��ƽ���� */
				Point3f tempPoint;
				tempPoint.x = i*squareSize.width;
				tempPoint.y = j*squareSize.height;
				tempPoint.z = 0;
				tempPointSet.push_back(tempPoint);
			}
		}

		points.push_back(tempPointSet);
	}
}

void FisheyeCalibration::DrawCorners(const Mat& src, const vector<Point2f>& corners)
{
	/* ���Ƽ�⵽�Ľǵ㲢���� */
	Mat imageTemp = src.clone();
	for (int j = 0; j < corners.size(); j++)
	{
		circle(imageTemp, corners[j], 10, Scalar(0, 0, 255), 2, 8, 0);
	}

	imshow("corners", imageTemp);	

	int key = 0;
	while (key != 'q')
	{
		key = waitKey(10);
	}
}

bool FisheyeCalibration::FindCorners(const Mat& src, vector<Point2f>& corners)
{
	if (src.empty())
	{
		cout << "image is empty!" << endl;

		return false;
	}

	// TODO: ��ͼ��ı�Ե������չ �ʵ�������Բ���ʧԭͼ����Ϣ

	// ת���ɻҶ�ͼ��
	Mat imageGray;
	cvtColor(src, imageGray, CV_RGB2GRAY);

	// ��ȡ�ǵ�
	bool patternfound = findChessboardCorners(src, boardSize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
	if (patternfound)
	{
		/* �����ؾ�ȷ��,�Լ�⵽����������ǵ㾫ȷ������ȷ����ĵ����corners�У� ��С���˵���100�Σ������0.001*/
		cornerSubPix(imageGray, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 0.001));

		return true;
	}
	else
	{
		cout << "find Chessboard Corners failed!" << endl;

		return false;
	}
}

