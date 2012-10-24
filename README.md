# Assistive technology for the visually impaired

Virtucane is an assistive app for the visually impaired. Once complete, it will assist blind people with everyday tasks. One of which is environmental text recognition. Another is detecting everyday objects such as power points. In the future, this may extend to localising an individual in a large environment (such as a shopping centre or airport).

There are two main aspects of this project:

* A set of tools for text recognition and object detection. These are programmed in C++ and use the [OpenCV](http://www.opencv.org/) computer vision library.
* A implementation of these computer vision tasks for an ARM based portable device. This will have a user interface designed for use by someone blind or visually impaired.

See the [wiki](https://github.com/dtcristo/virtucane/wiki) for extensive documentation on the project.

## Identification tools and implementations

The programs here are used for testing various text localisation/identification and object detection algorithms before final implementation in Android/Gumsix/ARM.

* *ImageLabeler* is a tool for creating the ground truth on power point images.
* *PpDetect* is an implementation of the template matching power point detection.
* *PpDetectGpio* is the same implementation as above except it will output to the Gumstix through a *screen* terminal interface. It uses AppleScript to communicate with this terminal window. A serial connection to the Gumstix is created using [these](http://wiki.gumstix.org/index.php?title=Gaining_Console_Connection_via_Terminal) instructions.
* *ReadText* is an implementation of text recognition with webcam input. For past developments in text recognition (and a command line tool), see the [dtcristo/read_text](https://github.com/dtcristo/read_text) repository.

## Gumstix tools and implementations

The subfolder *gumstix* contains programs to run on a Gumstix Overo. They were tested running a Linaro build of Ubuntu, specifically Linaro 12.03 [overo-alip.img.gz](http://releases.linaro.org/images/12.03/oneiric/alip). The MLO file in the boot partition needs to be replaced with [mlo-updated](http://cumulus.gumstix.org/images/angstrom/factory/2011-08-30-1058/) (rename this to *MLO* and overwrite). This is required for OveroSTORM chips. The kernel also needs to be re-built to add support for V4L2 as per [these](http://wiki.gumstix.org/index.php?title=Linaro_Webcam_Drivers) instructions.

* *find_blob* is a tiny program that uses OpenCV for simple blue "blob" detection.
* *gpio* initialises the GPIO pins on the Gumstix. It also includes functions to modify a specific GPIO pin.
* *keyboard* takes keyboard input (number keys) and triggers a corresponding GPIO pin. This is used for debugging.
* *ppdetect* is an implementation of the template matching power point detection.
* *video* is lower level code to access and image using the V4L2 driver. The frame is then converted to an OpenCV Mat object ready for processing.



## Android implementation

The subfolder *android* is an implementation of the project on Android. Current development status is **incomplete**. Currently it only obtains a frame from the camera.
