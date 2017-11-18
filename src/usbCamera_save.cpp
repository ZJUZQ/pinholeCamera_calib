#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>

#include <iostream>
#include <string>
#include <sstream>

const char* keys = 
{   
    "{d | | device number}"
    "{p | | storage path}"
    "{help h ? | |  print help message}"
};

void help(){
    std::cout << "usage: usbCamera_save -d=0 -p=./images\n";
} 

int main(int argc, const char** argv)
{
    cv::CommandLineParser parser(argc, argv, keys);
    std::cout << "hotkeys: \n"
                 "  q: quit\n"
                 "  s: save current image\n";
    if(parser.has("help") || parser.has("h") || parser.has("?")){
        help();
        return 0;
    }  
    if(!parser.has("p") || !parser.has("d")){
        help();
        return 0;
    }
    int device = parser.get<int>("d");
    std::string storagePath = parser.get<std::string>("p");
    if(storagePath[storagePath.length() - 1] != '/')
        storagePath = storagePath + "/";
    cv::VideoCapture cap;
    cap.open(device);
    if(!cap.isOpened()){
        std::cerr << "ERROR! Cannot open camera device: " << device << std::endl;
        help();
        return -1;
    }
    cv::namedWindow("show");
    int count = 0;
    cv::Mat image;
    for(;;){
        cap >> image;
        cv::imshow("show", image);
        
        char c = (char)cv::waitKey(2);
        if(c == 'q')
            break;
        if(c == 's'){
            std::stringstream ss;
            ss << storagePath << "image_" << count++ << ".png";
            cv::imwrite(ss.str(), image);
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}