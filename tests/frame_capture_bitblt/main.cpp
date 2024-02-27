#include <frame/capture/capture.bitblt.h>
#include <frame/capture/capture.include.h>
#include <frame/frame.include.h>
#include <utils/convect.string.h>

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


#include <Windows.h>

int main()
{
    auto capture = std::make_shared<tianli::frame::capture::capture_bitblt>();
    capture->set_capture_handle(GetDesktopWindow());
    cv::Mat frame;
    capture->get_frame(frame);
    if (frame.empty())
        std::cout << "frame is empty" << std::endl;
    else
        std::cout << "frame is not empty" << std::endl;
    cv::imwrite("test.png", frame);
    std::cout << frame.size() << std::endl;

    capture->set_capture_handle(GetForegroundWindow());
    capture->get_frame(frame);
    if (frame.empty())
        std::cout << "frame is empty" << std::endl;
    else
        std::cout << "frame is not empty" << std::endl;
    cv::imwrite("test.png", frame);
    std::cout << frame.size() << std::endl;

    capture->set_source_handle_callback([]() { return FindWindowW(NULL, utils::to_wstring("原神").c_str()); });
    capture->get_frame(frame);
    if (frame.empty())
        std::cout << "frame is empty" << std::endl;
    else
        std::cout << "frame is not empty" << std::endl;
    cv::imwrite("test.png", frame);
    cv::imshow("gi", frame);
    cv::waitKey(0);
    std::cout << frame.size() << std::endl;
    return 0;
}