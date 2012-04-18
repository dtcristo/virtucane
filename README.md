Assistive technology for the visually impaired
==============================================
Virtucane is an assistive app for the visually impaired. Once complete, it will assist blind people with everyday tasks. One of which is environmental text recognition. Another is detecting everyday objects such as power points. In the future, this may extend to localising an individual in a large environment (such as a shopping centre or airport).

There are two main aspects of this project:

* A set of tools for text recognition and object detection. These are programmed in C++ and use the [OpenCV](http://www.opencv.org/) computer vision library.
* A implementation of these computer vision tasks for [Android](http://www.android.com/) smartphones. This will have a user interface designed for use by someone blind or visually impaired.

See the [wiki](https://github.com/dtcristo/virtucane/wiki) for extensive documentation on the project.

Identification tools
--------------------
The programs in the *tools* subfolder are used for testing various text localisation/identification and object detection algorithms before final implementation in Android. For past developments in text recognition, see the [dtcristo/read_text](https://github.com/dtcristo/read_text) repository.

Android implementation
----------------------
The subfolder *android* is an implementation of the project on Android. Current development status is **incomplete**.

License
-------
Virtucane is licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).

	/*
	 * Copyright 2012 David Cristofaro, Monash University
	 * 
	 * Licensed under the Apache License, Version 2.0 (the "License");
	 * you may not use this file except in compliance with the License.
	 * You may obtain a copy of the License at
	 * 
	 *     http://www.apache.org/licenses/LICENSE-2.0
	 * 
	 * Unless required by applicable law or agreed to in writing, software
	 * distributed under the License is distributed on an "AS IS" BASIS,
	 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	 * See the License for the specific language governing permissions and
	 * limitations under the License.
	 */