import os
import os.path
import getopt 
import sys

# This script performs batch 'read text' on a directory of images.

def batch(path):
	dirlist = os.listdir(path)
	executeBin = "read_text -v -f "
	print "Current directory: " + path
	for name in dirlist:
		absolutePath = os.path.join(path,name)
		if not  os.path.isdir(absolutePath):
			print name
			if name.endswith(".jpg") or name.endswith(".png") or name.endswith(".JPG"):
				execute = executeBin + absolutePath
				os.system(execute)
		else:
			batch(absolutePath)

if __name__ == "__main__":
	batch(sys.argv[1]);
