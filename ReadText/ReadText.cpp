#include "read_text.h"

using namespace std;

// Program defaults
int imageMode_ = IMAGE_MODE_BINARY;
bool verbose_ = false;
bool scoring_ = false;
bool speak_ = false;
int speakMax_ = 0;

int main(int argc, char* argv[]) {

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
