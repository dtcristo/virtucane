#include "video.h"

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>

#include <ctime>
#include <sys/time.h>

#include <iostream>

using namespace std;
//using namespace cv;

int main()
{
    while(video_init((char*) "/dev/video0", 640, 480, 30))
    {
        printf("Camera initialisation failed. Retry in 2 seconds...\n");
        sleep(2);
    }
    
    unsigned char* frame = new unsigned char[640*480*2];
    
    sleep(5);
    
    int framecount = 0;
    timeval start;
    gettimeofday(&start, 0);
    
    while(framecount < 100)
    {
        get_frame_grey(frame);
        //Mat cvFrame = Mat(480, 640, CV_8UC1, (void*) frame);
        
        framecount++;
        cout << "Frame: " << framecount << endl;
        
        while(!video_frame_ready())
            usleep(50);
    }
    
    timeval end;
    gettimeofday(&end, 0);
    int seconds = (end.tv_sec - start.tv_sec);
    int fps = framecount / seconds + 0.5;
    cout << "fps = " << fps << endl;
    
    //FILE *file_fd;
    //file_fd = fopen("video_output.yuv", "wb");
    //get_frame_grey(frame);
    //fwrite((const void *) frame, 640*480, 1, file_fd);
    //fclose(file_fd);
    
    video_close();
    
    return 0;
}