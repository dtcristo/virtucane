#include "read_text.h"

using namespace std;

// Program defaults
int imageMode_ = IMAGE_MODE_BINARY;
bool verbose_ = false;
bool scoring_ = false;
bool speak_ = false;

int main(int ac, char* av[]) {

	try {
		po::options_description generic("Generic options");
		generic.add_options()
				("help,h", "Print help message");

		po::options_description config("Configuration options");
		config.add_options()
				("rgb,r","Process RGB images")
				("gray,g","Process grayscale images")
				("binary,b","Process binary images")
				("verbose,v","Save individual patches and OCR output")
				("filter,f","Filter results of OCR")
				("speak,s","Speak the results of OCR");

		po::options_description hidden("Hidden");
		hidden.add_options()
				("image",po::value<string>(), "Image to be processed");

		po::options_description cmdline_options;
		cmdline_options.add(generic).add(config).add(hidden);

		po::positional_options_description p;
		p.add("image", -1);

		po::variables_map vm;
		po::store(po::command_line_parser(ac, av).options(cmdline_options).positional(p).run(), vm);
		po::notify(vm);

		if (vm.count("help")) {
			cout << "Usage: read_text [image] <options>\n\n" << generic << endl << config << endl;
			return 1;
		}

		if (vm.count("rgb")) {
			imageMode_ = IMAGE_MODE_RGB;
		}

		if (vm.count("gray")) {
			imageMode_ = IMAGE_MODE_GRAY;
		}

		if (vm.count("binary")) {
			imageMode_ = IMAGE_MODE_BINARY;
		}

		if (vm.count("verbose")) {
			verbose_ = true;
		} else {
			verbose_ = false;
		}

		if (vm.count("filter")) {
			scoring_ = true;
		} else {
			scoring_ = false;
		}

		if (vm.count("speak")) {
			speak_ = true;
		} else {
			speak_ = false;
		}

		if (vm.count("image"))
		{
			DetectText detector = DetectText();
			//detector.readLetterCorrelation(argv[2]);
			//detector.readWordList(argv[3]);
			detector.detect(vm["image"].as<string>());
		}
		else {
			cout << "Usage: read_text [image] <options>" << endl;
			return 1;
		}

	} catch (exception& e) {
		cerr << "error: " << e.what() << endl;
		return 1;
	} catch (...) {
		cerr << "Exception of unknown type!" << endl;
		return 1;
	}

	return 0;
}

