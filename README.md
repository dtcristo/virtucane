read_text
=========

A command line tool for speaking text in an image. Tested on Mac OS X but should work anywhere.

This is a fork of [Menglong Zhu](http://www.seas.upenn.edu/~menglong/)'s ROS package [read_text](http://www.ros.org/wiki/read_text). I have made modifications for my own needs.

Dependencies
------------
Use [Homebrew](http://mxcl.github.com/homebrew/) for installation on OS X.

**Tesseract 3.01**

	brew install tesseract

**OpenCV 2.3.1**

	brew install opencv

Help
----
After building, run `read_text` for the following help:

	Usage: read_text [image] <options>

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