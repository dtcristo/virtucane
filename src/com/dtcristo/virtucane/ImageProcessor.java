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

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.SurfaceHolder;

public class ImageProcessor implements PreviewCallback, AutoFocusCallback, Runnable {
    private static final String TAG        = "ImageProcessor";

    private OutputHandler       mHandler;
    private SurfaceHolder       mHolder;
    private int                 mWidth;
    private int                 mHeight;
    private Mat                 mYuv;
    private Mat                 mGraySubmat;
    private Mat                 mRgba;

    private byte[]              mFrame;
    private Camera              mCamera;

    private boolean             mSaveFrame;
    private boolean             mThreadRun;
    private int                 count      = 0;

    public ImageProcessor(Context context, SurfaceHolder holder, int w, int h) {
        Log.i(TAG, "ImageProcessor()");

        mHandler = new OutputHandler(context);
        mHolder = holder;
        
        mWidth = w;
        mHeight = h;

        mYuv = new Mat(mHeight + mHeight / 2, mWidth, CvType.CV_8UC1);
        mGraySubmat = mYuv.submat(0, mHeight, 0, mWidth);
        mRgba = new Mat();
    }
    
    public void onPreviewFrame(byte[] data, Camera camera) {

        synchronized (ImageProcessor.this) {
            mFrame = data;
            mCamera = camera;
            ImageProcessor.this.notify();
        }
    }

    private Bitmap processFrame(byte[] data, Camera camera) {

        count++;
        Log.i(TAG, "Processing frame No. " + count);

        mSaveFrame = false;

        mYuv.put(0, 0, data);

        Imgproc.cvtColor(mYuv, mRgba, Imgproc.COLOR_YUV420sp2RGB, 4);

        Bitmap bmp = Bitmap.createBitmap(mRgba.cols(), mRgba.rows(), Bitmap.Config.ARGB_8888);

        if (Utils.matToBitmap(mRgba, bmp)) {
            Log.i(TAG, "Utils.matToBitmap()");
            return bmp;
        }

        bmp.recycle();
        return null;
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
            Bitmap bmp = null;

            synchronized (this) {
                try {
                    this.wait();
                    bmp = processFrame(mFrame, mCamera);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            
            // displaying result
            if (bmp != null) {
                Canvas canvas = mHolder.lockCanvas();

                if (canvas != null) {
                    canvas.drawBitmap(bmp, (canvas.getWidth() - mWidth) / 2,
                                      (canvas.getHeight() - mHeight) / 2, null);

                    Paint blue = new Paint();
                    blue.setARGB(100, 0, 0, 255);

                    canvas.drawCircle(400, 240, 10, blue);

                    mHolder.unlockCanvasAndPost(canvas);
                }
                bmp.recycle();
            }
        }
    }
}
