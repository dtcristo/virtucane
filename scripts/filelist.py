import os
import os.path
import getopt 
import sys

def filelist(thepath):
	'''		this python script read images from a directory, 
				add perform text detection	'''
	dirlist = os.listdir(thepath)
	executeBin = "~/Dropbox/Workspace/read_text/Debug/read_text -f "
	print "current directory:" + thepath
	for name in dirlist:
		absolutePath = os.path.join(thepath,name)
		if not  os.path.isdir(absolutePath):
			print name
			if name.endswith(".jpg") or name.endswith(".png") or name.endswith(".JPG"):
				execute = executeBin + absolutePath
				os.system(execute)
		else:
			filelist(absolutePath)

if __name__ == "__main__":
	filelist(sys.argv[1]);
