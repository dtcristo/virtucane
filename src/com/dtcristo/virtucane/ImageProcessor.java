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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.Date;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.os.Environment;
import android.util.Log;

public class ImageProcessor implements PreviewCallback, AutoFocusCallback, Runnable {
    private static final String TAG        = "ImageProcessor";

    private Date                mDate;
    private OutputHandler       mHandler;

    private byte[]              mFrame;
    private Camera              mCamera;

    private boolean             mThreadRun;
    private boolean             mSaveFrame = false;
    private int                 count      = 0;

    //private Mat                 mYuv;
    //private Mat                 mRgba;

    public ImageProcessor(Context context) {
        Log.i(TAG, "ImageProcessor()");

        mDate = new Date();
        mHandler = new OutputHandler(context);

        //mYuv = new Mat(mHeight + mHeight / 2, mWidth, CvType.CV_8UC1);
        //mRgba = new Mat();
    }

    public void onPreviewFrame(byte[] data, Camera camera) {

        synchronized (ImageProcessor.this) {
            mFrame = data;
            mCamera = camera;
            ImageProcessor.this.notify();
        }
    }

    private void processFrame(byte[] data, Camera camera) {

        count++;
        Log.i(TAG, "Processing frame No. " + count);

        /*
        try {
            Camera.Parameters param = camera.getParameters();
            Size size = param.getPreviewSize();
            YuvImage yuv = new YuvImage(data, param.getPreviewFormat(), size.width,
                                        size.height, null);
            String path = Environment.getExternalStorageDirectory().toString();
            File file = new File(path + "/" + date.getTime() + ".jpg");
            FileOutputStream out = new FileOutputStream(file);
            yuv.compressToJpeg(new Rect(0, 0, yuv.getWidth(), yuv.getHeight()), 90, out);
        } catch (FileNotFoundException e) {
            Log.e(TAG, e.getMessage());
        }*/

        /*
        if (saveFlag) {
            Log.i(TAG, "saveFlag = true");

            mYuv.put(0, 0, mFrame);
            Log.i(TAG, "mYuv.put()");

            Imgproc.cvtColor(mYuv, mRgba, Imgproc.COLOR_YUV420sp2RGB, 4);
            Log.i(TAG, "Imgproc.cvtColor()");

            Bitmap bmp = Bitmap.createBitmap(mWidth, mHeight, Bitmap.Config.ARGB_8888);
            Log.i(TAG, "Bitmap.createBitmap()");

            Utils.matToBitmap(mRgba, bmp);
            Log.i(TAG, "matToBitmap()");

            String path = Environment.getExternalStorageDirectory().toString();
            Log.i(TAG, "Output path: " + path);

            try {
                File file = new File(path, date.getTime() + ".png");
                FileOutputStream out = new FileOutputStream(file);
                bmp.compress(Bitmap.CompressFormat.PNG, 100, out);
            } catch (Exception e) {
                e.printStackTrace();
            }

            saveFlag = false;
        }*/

        /*
        mYuv.put(0, 0, data);

        switch (VirtucaneActivity.viewMode) {

            case VirtucaneActivity.VIEW_MODE_RGBA:
                Imgproc.cvtColor(mYuv, mRgba, Imgproc.COLOR_YUV420sp2RGB, 4);
                //if (portrait) rotate(mRgba);
                beep = false;
                break;

            case VirtucaneActivity.VIEW_MODE_THRESHOLD:
                if (portrait) rotate(mGraySubmat);

                Imgproc.threshold(mGraySubmat, mTemp, 80, 255, Imgproc.THRESH_BINARY);

                // 0 for black; 255 for white
                int pixelValue = (int) mTemp.get(mFrameHeight / 2, mFrameWidth / 2)[0];

                // Decide if to beep or not
                if (pixelValue == 0) beep = true;
                else beep = false;
                Imgproc.cvtColor(mTemp, mRgba, Imgproc.COLOR_GRAY2RGBA, 4);
                break;

            case VirtucaneActivity.VIEW_MODE_GREY:
                //if (portrait) rotate(mGraySubmat);
                beep = false;
                Imgproc.cvtColor(mGraySubmat, mRgba, Imgproc.COLOR_GRAY2RGBA, 4);
                break;

            case VirtucaneActivity.VIEW_MODE_CANNY:
                //if (portrait) rotate(mGraySubmat);
                Imgproc.Canny(mGraySubmat, mTemp, 80, 100);
                beep = false;
                Imgproc.cvtColor(mTemp, mRgba, Imgproc.COLOR_GRAY2BGRA, 4);
                break;
        }

        // Play sound
        if (isBeeping) {
            if (!beep) {
                isBeeping = false;
                mMediaPlayer.pause();
            }
        } else {
            if (beep) {
                isBeeping = true;
                mMediaPlayer.start();
            }
        }

        // Draw blue reticle
        Core.circle(mRgba, new Point(mFrameWidth / 2, mFrameHeight / 2), 8, new Scalar(0, 0, 255,
                                                                                       255));

        Bitmap bmp = Bitmap.createBitmap(mRgba.cols(), mRgba.rows(), Bitmap.Config.ARGB_8888);

        if (Utils.matToBitmap(mRgba, bmp)) return bmp;

        bmp.recycle();
        return null;*/
    }

    public void onAutoFocus(boolean success, Camera camera) {
        Log.i(TAG, "onAutoFocus()");

        mSaveFrame = true;
        //ocrFrame();
    }

    public void ocrFrame() {
        Log.i(TAG, "ocrFrame()");

        // TODO OCR the current frame

        String ocrResult = "This text is a sample. OCR text would normally appear here.";
        mHandler.showOcrDialog(ocrResult);
    }

    public void run() {
        mThreadRun = true;
        Log.i(TAG, "Starting processing thread");
        while (mThreadRun) {
            //Bitmap bmp = null;

            synchronized (this) {
                try {
                    this.wait();
                    processFrame(mFrame, mCamera);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            // displaying result
            /*
            if (bmp != null) {
                Canvas canvas = mHolder.lockCanvas();
                if (canvas != null) {
                    canvas.drawBitmap(bmp, (canvas.getWidth() - getFrameWidth()) / 2,
                                      (canvas.getHeight() - getFrameHeight()) / 2, null);
                    mHolder.unlockCanvasAndPost(canvas);
                }
                bmp.recycle();
            }*/
        }
    }
}
