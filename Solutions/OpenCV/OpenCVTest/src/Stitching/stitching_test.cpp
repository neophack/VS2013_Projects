//#include "opencv2/xfeatures2d/nonfree.hpp"  
//#include <opencv2\opencv.hpp>
//#include <iostream>  
//
//using namespace cv;
//using namespace std;
//using namespace xfeatures2d;
//
//typedef struct
//{
//	Point2f left_top;
//	Point2f left_bottom;
//	Point2f right_top;
//	Point2f right_bottom;
//}four_corners_t;
//
//four_corners_t corners;
//
//void CalcCorners(const Mat& H, const Mat& src)
//{
//	double v2[] = { 0, 0, 1 };//���Ͻ�
//	double v1[3];//�任�������ֵ
//	Mat V2 = Mat(3, 1, CV_64FC1, v2);  //������
//	Mat V1 = Mat(3, 1, CV_64FC1, v1);  //������
//
//	V1 = H * V2;
//	//���Ͻ�(0,0,1)
//	cout << "V2: " << V2 << endl;
//	cout << "V1: " << V1 << endl;
//	corners.left_top.x = v1[0] / v1[2];
//	corners.left_top.y = v1[1] / v1[2];
//
//	//���½�(0,src.rows,1)
//	v2[0] = 0;
//	v2[1] = src.rows;
//	v2[2] = 1;
//	V2 = Mat(3, 1, CV_64FC1, v2);  //������
//	V1 = Mat(3, 1, CV_64FC1, v1);  //������
//	V1 = H * V2;
//	corners.left_bottom.x = v1[0] / v1[2];
//	corners.left_bottom.y = v1[1] / v1[2];
//
//	//���Ͻ�(src.cols,0,1)
//	v2[0] = src.cols;
//	v2[1] = 0;
//	v2[2] = 1;
//	V2 = Mat(3, 1, CV_64FC1, v2);  //������
//	V1 = Mat(3, 1, CV_64FC1, v1);  //������
//	V1 = H * V2;
//	corners.right_top.x = v1[0] / v1[2];
//	corners.right_top.y = v1[1] / v1[2];
//
//	//���½�(src.cols,src.rows,1)
//	v2[0] = src.cols;
//	v2[1] = src.rows;
//	v2[2] = 1;
//	V2 = Mat(3, 1, CV_64FC1, v2);  //������
//	V1 = Mat(3, 1, CV_64FC1, v1);  //������
//	V1 = H * V2;
//	corners.right_bottom.x = v1[0] / v1[2];
//	corners.right_bottom.y = v1[1] / v1[2];
//
//}
//
////�Ż���ͼ�����Ӵ���ʹ��ƴ����Ȼ
//void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst)
//{
//	imshow("img1", img1);
//	imshow("trans", trans);
//	imshow("dst", dst);
//
//	int start = MIN(corners.left_top.x, corners.left_bottom.x);//��ʼλ�ã����ص��������߽�  
//
//	double processWidth = img1.cols - start;//�ص�����Ŀ��  
//	int rows = dst.rows;
//	int cols = img1.cols; //ע�⣬������*ͨ����
//	double alpha = 1;//img1�����ص�Ȩ��  
//	for (int i = 0; i < rows; i++)
//	{
//		uchar* p = img1.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
//		uchar* t = trans.ptr<uchar>(i);
//		uchar* d = dst.ptr<uchar>(i);
//		for (int j = start; j < cols; j++)
//		{
//			//�������ͼ��trans�������صĺڵ㣬����ȫ����img1�е�����
//			if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
//			{
//				alpha = 1;
//			}
//			else
//			{
//				//img1�����ص�Ȩ�أ��뵱ǰ�������ص�������߽�ľ�������ȣ�ʵ��֤�������ַ���ȷʵ��  
//				alpha = (processWidth - (j - start)) / processWidth;
//			}
//
//			d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
//			d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
//			d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);
//
//		}
//	}
//
//}
//
//
//int main()
//{
//	Mat image01 = imread("./img/22.jpg", 1);    //��ͼ
//	Mat image02 = imread("./img/11.jpg", 1);    //��ͼ
//	//namedWindow("p2", 0);
//	//namedWindow("p1", 0);
//	//imshow("p2", image01);
//	//imshow("p1", image02);
//
//	//�Ҷ�ͼת��  
//	Mat image1, image2;
//	cvtColor(image01, image1, CV_RGB2GRAY);
//	cvtColor(image02, image2, CV_RGB2GRAY);
//	
//	//��ȡ������ 
//	// ���������OpenCV3.0��ķ�����2.0��ͬ
//	int minHessian = 1000;	// ����������ֵ��������������ȣ�ֵԽ���Խ�٣�Խ��׼ 
//	Ptr<SURF> surfDetector = SURF::create(minHessian); 
//	vector<KeyPoint> keyPoint1, keyPoint2;
//	Mat imageDesc1, imageDesc2;
//	surfDetector->detectAndCompute(image1, Mat(), keyPoint1, imageDesc1);
//	surfDetector->detectAndCompute(image2, Mat(), keyPoint2, imageDesc2);
//
//	////���ƥ�������㣬����ȡ�������     
//	//FlannBasedMatcher matcher;
//	//vector<DMatch> matchePoints;
//
//	//matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
//	//cout << "total match points: " << matchePoints.size() << endl;
//
//	//Mat img_match;
//	//drawMatches(image01, keyPoint1, image02, keyPoint2, matchePoints, img_match);
//	//namedWindow("match", 0);
//	//imshow("match", img_match);
//	////imwrite("match.jpg", img_match);
//	
//	FlannBasedMatcher matcher;
//	vector<vector<DMatch> > matchePoints;
//	vector<DMatch> GoodMatchePoints;
//
//	vector<Mat> train_desc(1, imageDesc1);
//	matcher.add(train_desc);
//	matcher.train();
//
//	matcher.knnMatch(imageDesc2, matchePoints, 2);
//	cout << "total match points: " << matchePoints.size() << endl;
//
//	// Lowe's algorithm,��ȡ����ƥ��� ɸѡƥ���
//	for (int i = 0; i < matchePoints.size(); i++)
//	{
//		if (matchePoints[i][0].distance < 0.4 * matchePoints[i][1].distance)
//		{
//			GoodMatchePoints.push_back(matchePoints[i][0]);
//		}
//	}
//
//	Mat first_match;
//	drawMatches(image02, keyPoint2, image01, keyPoint1, GoodMatchePoints, first_match);
//	imshow("first_match ", first_match);
//
//	/*--------------ͼ����׼-----------------*/
//	// ͼ�����׼,��������ͼ��ת��Ϊͬһ�����¡�ʹ��findHomography��������ñ任����
//	// findHomography������Ҫ�õ��ĵ㼯��Point2f���͵ģ���Ҫ�����Ǹյõ��ĵ㼯GoodMatchePoints����һ�δ���ʹ��ת��ΪPoint2f���͵ĵ㼯
//
//	vector<Point2f> imagePoints1, imagePoints2;
//
//	for (int i = 0; i<GoodMatchePoints.size(); i++)
//	{
//		imagePoints2.push_back(keyPoint2[GoodMatchePoints[i].queryIdx].pt);
//		imagePoints1.push_back(keyPoint1[GoodMatchePoints[i].trainIdx].pt);
//	}
//
//	// ��ȡͼ��1��ͼ��2��ͶӰӳ����� �ߴ�Ϊ3*3 
//	// findHomography�����Ĳ���������ѡ����CV_RANSAC�����������ѡ��RANSAC�㷨����ɸѡ�ɿ���ƥ��㣬��ʹ��ƥ�����Ϊ��ȷ��
//	Mat homo = findHomography(imagePoints1, imagePoints2, CV_RANSAC);
//	// Ҳ����ʹ��getPerspectiveTransform�������͸�ӱ任���󣬲���Ҫ��ֻ����4���㣬Ч���Բ�  
//	// Mat   homo=getPerspectiveTransform(imagePoints1,imagePoints2);  
//
//	cout << "�任����Ϊ��\n" << homo << endl << endl; //���ӳ�����     
//
//	//������׼ͼ���ĸ���������
//	CalcCorners(homo, image01);
//	cout << "left_top:" << corners.left_top << endl;
//	cout << "left_bottom:" << corners.left_bottom << endl;
//	cout << "right_top:" << corners.right_top << endl;
//	cout << "right_bottom:" << corners.right_bottom << endl;
//
//	//ͼ����׼  
//	Mat imageTransform1, imageTransform2;
//	warpPerspective(image01, imageTransform1, homo, Size(MAX(corners.right_top.x, corners.right_bottom.x), image02.rows));
//	//warpPerspective(image01, imageTransform2, adjustMat*homo, Size(image02.cols*1.3, image02.rows*1.8));
//	imshow("ֱ�Ӿ���͸�Ӿ���任", imageTransform1);
//	//imwrite("trans1.jpg", imageTransform1);
//
//	//����ƴ�Ӻ��ͼ,����ǰ����ͼ�Ĵ�С
//	int dst_width = imageTransform1.cols;  //ȡ���ҵ�ĳ���Ϊƴ��ͼ�ĳ���
//	int dst_height = image02.rows;
//
//	Mat dst(dst_height, dst_width, CV_8UC3);
//	//dst.setTo(0);
//
//	imageTransform1.copyTo(dst(Rect(0, 0, imageTransform1.cols, imageTransform1.rows)));
//	image02.copyTo(dst(Rect(0, 0, image02.cols, image02.rows)));
//
//	//imshow("b_dst", dst);
//
//
//	OptimizeSeam(image02, imageTransform1, dst);
//	//imshow("dst", dst);
//
//
//	waitKey();
//	return 0;
//}