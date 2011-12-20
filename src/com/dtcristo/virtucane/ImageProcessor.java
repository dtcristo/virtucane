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

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;

public class ImageProcessor implements PreviewCallback, AutoFocusCallback {
    private static final String TAG = "ImageProcessor";

    private OutputHandler       mHandler;
    
    @SuppressWarnings("unused")
    private byte[]              mFrame;

    public ImageProcessor(Context context) {
        Log.i(TAG, "ImageProcessor()");

        mHandler = new OutputHandler(context);
    }

    public void onPreviewFrame(byte[] data, Camera camera) {
        mFrame = data;
        // TODO Deal with the current frame.
    }

    /*protected Bitmap processFrame(byte[] data) {

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
        return null;
    }*/

    public void onAutoFocus(boolean success, Camera camera) {
        Log.i(TAG, "onAutoFocus()");

        ocrFrame();
    }
    
    public void ocrFrame() {
        Log.i(TAG, "ocrFrame()");

        // TODO OCR the current frame
        
        String ocrResult = "This text is a sample. OCR text would normally appear here.";
        mHandler.showOcrDialog(ocrResult);
    }
}
