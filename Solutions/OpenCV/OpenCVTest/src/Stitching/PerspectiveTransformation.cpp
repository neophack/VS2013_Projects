//#include "opencv2/opencv.hpp"
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//
///*
//OpenCV��͸�ӱ任���ַ�Ϊ����:
//1. ϡ��͸�ӱ任 2. �ܼ�͸�ӱ任
//���Ǿ����ᵽ�Ķ�ͼ���͸�ӱ任����ָ�ܼ�͸�ӱ任����ϡ��͸�ӱ任��OpenCV��������ƥ��֮����������������ʶ�о����õ���
//һ��������ܼ�͸�ӱ任warpPerspective�������뺯��getPerspectiveTransformһ��ʹ��ʵ�ֶ�ͼ���͸��У��
//��ϡ��͸�ӱ任perspectiveTransform������findhomographyһ��ʹ�á�
//
//*/
//
//vector<Point2f> srcTri(4);
//vector<Point2f> dstTri(4);
//int clickTimes = 0;  //��ͼ���ϵ�������
//Mat src, dst;
//Mat imageWarp;
//
//void onMouse(int event, int x, int y, int flags, void *utsc);
//
//int main()
//{
//	src = imread("./img/cali_1.jpg");
//	namedWindow("src", CV_WINDOW_AUTOSIZE);
//	imshow("src", src);
//
//	setMouseCallback("src", onMouse);
//
//	waitKey();
//
//	return 0;
//
//	
//	//Mat src = imread("./img/cali_1.jpg");
//	//if (src.empty()){
//	//	cout << "can not load picture" << endl;	
//	//	return -1;
//	//}
//	//
//	////namedWindow("src", CV_WINDOW_AUTOSIZE);
//	////imshow("src", src);
//
//	//vector<Point2f>src_coners(4);
//	//src_coners[0] = Point2f(0, 0);
//	//src_coners[1] = Point2f(0, src.rows);
//	//src_coners[2] = Point2f(src.cols, 0);
//	//src_coners[3] = Point2f(src.cols, src.rows);
//	//circle(src, src_coners[0], 3, Scalar(0, 0, 255), 3, 8);
//	//circle(src, src_coners[1], 3, Scalar(0, 0, 255), 3, 8);
//	//circle(src, src_coners[2], 3, Scalar(0, 0, 255), 3, 8);
//	//circle(src, src_coners[3], 3, Scalar(0, 0, 255), 3, 8);
//
//	//imshow("src", src);
//
//	//vector<Point2f>dst_coners(4);
//	//dst_coners[0] = Point2f(0, 0);
//	//dst_coners[1] = Point2f(0, src.rows);
//	//dst_coners[2] = Point2f(src.cols, 0);
//	//dst_coners[3] = Point2f(src.cols, src.rows);
//
//	//Mat warpMatrix = getPerspectiveTransform(src_coners, dst_coners);
//	//Mat dst;
//	//warpPerspective(src, dst, warpMatrix, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
//
//	//imshow("dst", dst);
//
//	//waitKey();
//	//return 0;
//}
//
//
//void onMouse(int event, int x, int y, int flags, void *utsc)
//{
//	if (event == CV_EVENT_LBUTTONUP)   //��Ӧ�������¼�
//	{
//		circle(src, Point(x, y), 2, Scalar(0, 0, 255), 2);  //���ѡ�е�
//		imshow("wait ", src);
//		srcTri[clickTimes].x = x;
//		srcTri[clickTimes].y = y;
//		clickTimes++;
//
//		cout << "x: " << x << " y: " << y << endl;
//	}
//	if (clickTimes == 4)
//	{
//		dstTri[0].x = 0;
//		dstTri[0].y = 0;
//
//		dstTri[1].x = 282;
//		dstTri[1].y = 0;
//
//		dstTri[2].x = 282;
//		dstTri[2].y = 438;
//
//		dstTri[3].x = 0;
//		dstTri[3].y = 438;
//
//		//Mat H = findHomography(srcTri, dstTri, RANSAC);//����͸�Ӿ���
//		//warpPerspective(src, dst, H, Size(282, 438));//ͼ��͸�ӱ任
//
//		Mat warpMatrix = getPerspectiveTransform(srcTri, dstTri);
//		//warpPerspective(src, dst, warpMatrix, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
//		warpPerspective(src, dst, warpMatrix, Size(282, 438));//ͼ��͸�ӱ任
//
//		imshow("output", dst);
//	}
//}
