read_text
=========

A a command line tool for text localisation and identification.

This is a fork of [Menglong Zhu](http://www.seas.upenn.edu/~menglong/)'s ROS package [read_text](http://www.ros.org/wiki/read_text). I have made modifications for my own needs.

Help
----

	Usage: read_text [image] <options>

	Generic options:
	  -h [ --help ]         Print help message		
	
	Configuration options:
	  -r [ --rgb ]          Process RGB images
	  -g [ --gray ]         Process grayscale images
	  -b [ --binary ]       Process binary images
	  -v [ --verbose ]      Save individual patches and OCR output
	  -f [ --filter ]       Filter results of OCR
	  -s [ --speak ]        Speak the results of OCR