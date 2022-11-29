#include<iostream>
#include "manager.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>
#include <map>
#include <cmath>
#include <time.h>
using namespace cv;

// void save::detectResult(cv::Mat& img, std::vector<DetectBox>& boxes) {
//     cv::Mat temp = img.clone();
//     for (auto box : boxes) {
//         cv::Point lt(box.x1, box.y1);
//         cv::Point br(box.x2, box.y2);
//         cv::rectangle(temp, lt, br, cv::Scalar(255, 0, 0), 1);
//         //std::string lbl = cv::format("ID:%d_C:%d_CONF:%.2f", (int)box.trackID, (int)box.classID, box.confidence);
// 		//std::string lbl = cv::format("ID:%d_C:%d", (int)box.trackID, (int)box.classID);
// 		std::string lbl = cv::format("ID:%d_x:%f_y:%f",(int)box.trackID,(box.x1+box.x2)/2,(box.y1+box.y2)/2);
//         cv::putText(temp, lbl, lt, cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0,255,0));
//     }
// 	// cv::imwrite("../result.mp4", temp)
//     // cv::imshow("img", temp);
//     // cv::waitKey(1);
// }



int main(){
	// calculate every person's (id,(up_num,down_num,average_x,average_y))
	map<int,vector<int>> personstate;
	map<int,int> classidmap;
	bool is_first = true;
	char* yolo_engine = "../weights/yolov5.engine";
	char* sort_engine = "../weights/deepsort.engine";
	float conf_thre = 0.4;
	Trtyolosort yosort(yolo_engine,sort_engine);

	VideoCapture capture;
	cv::Mat frame;
	std::string video_path = "../../test.mp4";
	frame = capture.open(video_path);
	if (!capture.isOpened()){
		std::cout<<"can not open"<<std::endl;
		return -1 ;
	}
	capture.read(frame);
	std::vector<DetectBox> det;
	auto start_draw_time = std::chrono::system_clock::now();

	
	// 设置视频及保存视频路径
	string save_path = "../../test_result.mp4";

	// 添加，保存视频
	VideoWriter writer;
	Size size = Size(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	int codec = VideoWriter::fourcc('m', 'p', '4', 'v');
	int rate = capture.get(cv::CAP_PROP_FPS);
	std::cout << "rate" << rate;
	writer.open(save_path, codec, rate, size, true);
	
	clock_t start_draw,end_draw;
	start_draw = clock();
	int i = 0;
	while(capture.read(frame)){
		if (i%3==0){
		//std::cout<<"origin img size:"<<frame.cols<<" "<<frame.rows<<std::endl;
		auto start = std::chrono::system_clock::now();
		yosort.TrtDetect(frame,conf_thre,det);

		std::cout << "det: " << det.size() << std::endl;

		// std::cout << "->classID" << det.begin()->classID << "->confidence" << det.begin()->confidence << ">x1e" << det.begin()->x1 << std::endl;
		for (int i = 0; i < det.size(); ++i) 
		{
    		std::cout << "classID: " << det[i].classID << "confidence" << det[i].confidence <<std::endl;
			// yosort.showDetection(frame, det[i]);
			}
		// std::cout << "det: " << det.begin() << std::endl;

		// save video
		// cv::Mat temp = frame.clone();
		for (auto box : det) {
			cv::Point lt(box.x1, box.y1);
			cv::Point br(box.x2, box.y2);
			cv::rectangle(frame, lt, br, cv::Scalar(255, 0, 0), 1);
			//std::string lbl = cv::format("ID:%d_C:%d_CONF:%.2f", (int)box.trackID, (int)box.classID, box.confidence);
			//std::string lbl = cv::format("ID:%d_C:%d", (int)box.trackID, (int)box.classID);
			std::string lbl = cv::format("ID:%d_x:%f_y:%f",(int)box.trackID,(box.x1+box.x2)/2,(box.y1+box.y2)/2);
			cv::putText(frame, lbl, lt, cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0,255,0));
		}
		writer << frame;
		
		// yosort.showDetection(frame, det);
		auto end = std::chrono::system_clock::now();
		int delay_infer = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		// std::cout  << "delay_infer:" << delay_infer << "ms" << std::endl;
		}
		i++;
	}
	capture.release();
	return 0;
	
}
