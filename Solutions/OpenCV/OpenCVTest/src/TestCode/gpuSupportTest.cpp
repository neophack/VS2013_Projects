//#include <opencv2\opencv.hpp>
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//using namespace cv::cuda;
//
//int main()
//{	
//	try
//	{
//		cv::cuda::DeviceInfo _deviceInfo;
//
//		bool _isDeviceOK = _deviceInfo.isCompatible();
//		std::cout << "IsGPUDeviceOK : " << _isDeviceOK << std::endl;
//
//		/*-------------------------����������֤��ʽ����ѡȡһ�ּ���-------------------------*/
//		//��ȡ�Կ�����Ϣ
//		cuda::printShortCudaDeviceInfo(cuda::getDevice());  //���Կ���Ϣ��ʾGPUģ�����óɹ�
//
//		//��ȡ�Կ���ϸ��Ϣ
//		cuda::printCudaDeviceInfo(cuda::getDevice());  //���Կ���Ϣ��ʾGPUģ�����óɹ�
//
//		//��ȡ�Կ��豸����
//		int Device_Num = cuda::getCudaEnabledDeviceCount();
//		cout << Device_Num << endl;  //����ֵ����0��ʾGPUģ�����óɹ�
//
//		//��ȡ�Կ��豸״̬
//		cuda::DeviceInfo Device_State;
//		bool Device_OK = Device_State.isCompatible();
//		cout << "Device_State: " << Device_OK << endl;  //����ֵ����0��ʾGPUģ�����óɹ�
//	}
//	catch (exception e){
//		std::cout << "�����쳣��" << e.what() << std::endl;
//	}
//
//	system("pause");
//	
//	return 0;
//}