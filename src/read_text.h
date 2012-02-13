#ifndef _READ_TEXT_
#define _READ_TEXT_

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "DetectText.h"

using namespace std;

// Program options
#define IMAGE_MODE_RGB    0
#define IMAGE_MODE_GRAY   1
#define IMAGE_MODE_BINARY 2

extern int imageMode_;
extern bool verbose_;
extern bool scoring_;
extern bool speak_;

#endif
