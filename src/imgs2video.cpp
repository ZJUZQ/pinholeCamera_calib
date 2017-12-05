#include <iostream>
#include <vector>
#include <cstdlib> // rand()
#include <ctime>
#include <opencv2/opencv.hpp>

void help(){
	std::cout << "usage: imgs2video imgs2video.xml"
}

int main(int argc, char** argv)
{
	std::string param = argv[1];
    cv::FileStorage fs(param, cv::FileStorage::READ);
    int num_cams = fs["num_cams"];
   
    std::vector<std::string> camLists;
    cv::FileNode n = fs["cam_imgList"];
    for(cv::FileNodeIterator it = n.begin(); it != n.end(); ++it)
        camLists.push_back((std::string)*it);

    std::vector<std::string> camIP;
    for(int i = 0; i < camLists.size(); ++i){
        std::size_t found1 = camLists[i].find_last_of('/');
        std::string tmp = camLists[i].substr(found1 + 1);
        camIP.push_back( tmp.substr(0, tmp.find('.')) );
    }

    std::map<std::string, std::vector<std::string> > imgNameList;
    for(int i = 0; i < camIP.size(); ++i){
        std::vector<std::string> imgList_tmp;
        cv::FileStorage fs1(camLists[i], cv::FileStorage::READ);
        cv::FileNode n = fs1["images"];
        for(cv::FileNodeIterator it = n.begin(); it != n.end(); ++it)
            imgList_tmp.push_back(*it);
        imgNameList[camIP[i]] = imgList_tmp;
        fs1.close();
    }

    std::map<std::string, cv::VideoWriter> writerList;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    double fps = fs["fps"];                         // framerate of the created video stream
    int height = fs["height"];
    int width = fs["width"];
    for(int i = 0; i < camIP.size(); ++i){
        std::stringstream videoName;
        videoName << camIP[i] << ".avi";
        writerList[camIP[i]].open(videoName, codec, fps, cv::Size(width, height), true);
    }

    int numImgsGenerate = fs["numImgsGenerate"];
    for(int i = 0; i < numImgsGenerate; ++i){
        int k = std::rand() % imgNameList[camIP[0]].size();
        for(int j = 0; j < camIP.size(); ++j){
            cv::Mat image = cv::imread(imgNameList[camIP[j]][k]);
            writerList[camIP[j]] << image;
        }

    }

}