#include "ReadText.h"

#define WEBCAM        true

using namespace std;

// Program defaults
int imageMode_ = IMAGE_MODE_BINARY;
bool verbose_ = false;
bool scoring_ = true;
bool speak_ = true;
int speakMax_ = 0;

int main(int argc, char* argv[]) {
    
    int frameNo = 0;
    bool showingResult = false;

    if (WEBCAM)
    {
        Mat frame;
        
        namedWindow("Frame", 1);
        //namedWindow("Result", 1);
        
        VideoCapture capture(0);
        if (!capture.isOpened())
        {
            cout << "Could not open VideoCapture." << endl;
            return -1;
        }
        
        for(;;)
        {
            capture >> frame;
            
            if(!showingResult) imshow("Frame", frame);

            int c = waitKey(1);
            if ((char) c == 27 /*ESC key*/)
            {
                return 0;
            }
            else if ((char) c == ' ')
            {
                if (!showingResult)
                {
                    stringstream ss;
                    ss.str("");
                    ss << "frame_" << frameNo << ".jpg";
                    
                    imwrite(ss.str(), frame);
                    DetectText detector = DetectText();
                    detector.detect(ss.str());
                    
                    //Mat result = imread("frame_detection.jpg");
                    //imshow("Result", result);
                    
                    frameNo++;
                    showingResult = true;
                }
                else
                {
                    showingResult = false;
                }
            }
        }
    }
    else
    {
        try {
            po::options_description generic("Generic options");
            generic.add_options()
                    ("help,h", "Print this help message.");

            po::options_description mode("Processing mode (choose one)");
            mode.add_options()
                    ("rgb,r","Process RGB images.")
                    ("gray,g","Process grayscale images.")
                    ("binary,b", "(Default) Process binary images. Otsu's method.");

            po::options_description config("Configuration options");
            config.add_options()
                    ("verbose,v","Save individual patches and OCR output. Also print progress to terminal.")
                    ("filter,f","Filter results of OCR.")
                    ("speak,s",po::value<int>(&speakMax_)->implicit_value(0) ,"Speak filtered results of OCR. (Optional) \"arg\" is the max phrases to speak.");

            po::options_description hidden("Hidden");
            hidden.add_options()
                    ("image",po::value<string>(), "Image to be processed");

            po::options_description cmdline_options;
            cmdline_options.add(generic).add(mode).add(config).add(hidden);

            po::positional_options_description p;
            p.add("image", -1);

            po::variables_map vm;
            po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);

            if (vm.count("help")) {
                cout << "Usage: read_text <image> [options]\n\n" << generic << endl << mode << endl << config << endl;
                return 1;
            }

            po::notify(vm);

            if (vm.count("rgb")) imageMode_ = IMAGE_MODE_RGB;

            if (vm.count("gray")) imageMode_ = IMAGE_MODE_GRAY;

            if (vm.count("binary")) imageMode_ = IMAGE_MODE_BINARY;

            if (vm.count("verbose")) verbose_ = true;

            if (vm.count("filter")) scoring_ = true;

            if (vm.count("speak")) speak_ = true;

            if (vm.count("image"))
            {
                // We're all good, ready to run detection.
                DetectText detector = DetectText();
                detector.detect(vm["image"].as<string>());
            }
            else {
                cout << "Usage: read_text <image> [options]\n\n" << generic << endl << mode << endl << config << endl;
                return 1;
            }

        } catch (exception& e) {
            cerr << "Error: " << e.what() << endl;
            return 1;
        } catch (...) {
            cerr << "Exception of unknown type!" << endl;
            return 1;
        }

        return 0;
    }
}
