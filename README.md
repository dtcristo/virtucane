read_text
=========
A command line tool for speaking text in an image. Tested in OS X but should work anywhere.

This is a fork of [Menglong Zhu](http://www.seas.upenn.edu/~menglong/)'s ROS package [read_text](http://www.ros.org/wiki/read_text). I have made modifications for my own needs.

Installation (OS X)
-------------------
The easiest way to install on OS X is to use [Homebrew](http://mxcl.github.com/homebrew/).

1. Install Homebrew. See the Homebrew [Installation](https://github.com/mxcl/homebrew/wiki/installation) for more info. Make sure you have [Xcode](https://developer.apple.com/xcode/) installed first, alternatively [Commannd Line Tools for Xcode](https://developer.apple.com/downloads/index.action?=command%20line%20tools).

		/usr/bin/ruby -e "$(/usr/bin/curl -fsSL https://raw.github.com/mxcl/homebrew/master/Library/Contributions/install_homebrew.rb)"

2. Run the installation command:

		brew install --HEAD https://raw.github.com/dtcristo/read_text/master/Formula/read_text.rb

Dependencies
------------
If you use Homebrew, it will automatically take care of dependencies.

[**Tesseract**](http://code.google.com/p/tesseract-ocr/)

[**OpenCV**](http://opencv.willowgarage.com/)

[**Boost C++ Libraries**](http://www.boost.org/)

Example Usage
-------------
To detect text in an image, filter the OCR results, then speak; run the following command:

	read_text image.jpg -f -s

To batch process a folder of images, use the Python script. The script uses the `-f` option.

	python /usr/local/bin/read_text_batch.py /directory/with/images/

Help
----
After building, run `read_text --help` for the following help:

	Usage: read_text <image> [options]

	Generic options:
  		-h [ --help ]         Print this help message.

	Processing mode (choose one):
  		-r [ --rgb ]          Process RGB images.
  		-g [ --gray ]         Process grayscale images.
  		-b [ --binary ]       (Default) Process binary images. Otsu's method.

	Configuration options:
  		-v [ --verbose ]          	Save individual patches and OCR output. Also print 
                            		progress to terminal.
  		-f [ --filter ]           	Filter results of OCR.
  		-s [ --speak ] [=arg(=0)] 	Speak filtered results of OCR. (Optional) "arg" is 
                            		the max phrases to speak.