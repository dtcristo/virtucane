/* 
 * Copyright (C) 2011  David Cristofaro
 * 
 * Email:      d@davidcristofaro.com
 * Homepage:   http://davidcristofaro.com
 * Repository: https://github.com/dtcristo/virtucane
 * 
 * This file is part of virtucane.
 * 
 * Virtucane is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Virtucane is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with virtucane.  If not, see <http://www.gnu.org/licenses/>.
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
        
        // TODO Respond to an autofocus completion.
        //ocrFrame();
    }
    
    public void ocrFrame() {
        Log.i(TAG, "ocrFrame()");

        // TODO OCR the current frame
        
        String ocrResult = "This text is a sample. OCR text would normally appear here.";
        mHandler.showOcrDialog(ocrResult);
    }
}
