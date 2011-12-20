/*
 * Copyright 2011 David Cristofaro
 * Copyright 2011 Monash University
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

package com.dtcristo.virtucane;

import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public abstract class CvCameraView extends SurfaceView implements SurfaceHolder.Callback, Runnable {
    private static final String TAG      = "CvCameraView";

    private SurfaceHolder       mHolder;
    private VideoCapture        mCamera;

    int                         mFrameWidth;
    int                         mFrameHeight;

    boolean                     portrait = true;

    public CvCameraView(Context context) {
        super(context);
        Log.i(TAG, "CvCameraView()");

        mHolder = getHolder();
        mHolder.addCallback(this);
    }

    public void surfaceChanged(SurfaceHolder _holder, int format, int width, int height) {
        Log.i(TAG, "surfaceChanged()");

        synchronized (this) {
            if (mCamera != null && mCamera.isOpened()) {

                mFrameWidth = width;
                mFrameHeight = height;

                Log.i(TAG, "mFrameWidth = " + mFrameWidth);
                Log.i(TAG, "mFrameHeight = " + mFrameHeight);

                // Selecting optimal camera preview size
                // sizes = [1280x720, 800x480, 768x432, 720x480, 640x480,
                // 576x432, 480x320, 400x240, 384x288, 352x288, 320x240,
                // 272x272, 240x240, 240x160, 176x144]
                //
                // List<Size> sizes = mCamera.getSupportedPreviewSizes();
                //
                // {
                // double minDiff = Double.MAX_VALUE;
                // for (Size size : sizes) {
                // if (Math.abs(size.width - width) < minDiff) {
                // mFrameWidth = (int) size.width;
                // mFrameHeight = (int) size.height;
                // minDiff = Math.abs(size.width - width);
                // }
                // }
                // }

                if (portrait) {
                    mCamera.set(Highgui.CV_CAP_PROP_FRAME_WIDTH, mFrameHeight);
                    mCamera.set(Highgui.CV_CAP_PROP_FRAME_HEIGHT, mFrameWidth);
                } else {
                    mCamera.set(Highgui.CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
                    mCamera.set(Highgui.CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);
                }
            }
        }
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated()");

        mCamera = new VideoCapture(Highgui.CV_CAP_ANDROID);
        if (mCamera.isOpened()) {
            (new Thread(this)).start();
        } else {
            mCamera.release();
            mCamera = null;
            Log.e(TAG, "mCamera.isOpened() failed");
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed()");

        if (mCamera != null) {
            synchronized (this) {
                mCamera.release();
                mCamera = null;
            }
        }
    }

    protected abstract Bitmap processFrame(VideoCapture capture);

    public void run() {
        Log.i(TAG, "run()");

        while (true) {
            Bitmap bmp = null;

            synchronized (this) {
                if (mCamera == null) break;

                if (!mCamera.grab()) {
                    Log.e(TAG, "mCamera.grab() failed");
                    break;
                }
                bmp = processFrame(mCamera);
            }

            if (bmp != null) {
                Canvas canvas = mHolder.lockCanvas();
                if (canvas != null) {
                    canvas.drawBitmap(bmp, (canvas.getWidth() - bmp.getWidth()) / 2,
                                      (canvas.getHeight() - bmp.getHeight()) / 2, null);

                    mHolder.unlockCanvasAndPost(canvas);
                }
                bmp.recycle();
            }
        }
        Log.i(TAG, "run() complete");
    }
}
