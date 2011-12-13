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

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;

import android.content.Context;
import android.graphics.Bitmap;
import android.media.MediaPlayer;
import android.util.Log;
import android.view.SurfaceHolder;

class VirtucaneView extends CvCameraView {
    private static final String TAG       = "VirtucaneView";

    private MediaPlayer         mMediaPlayer;

    private boolean             isBeeping = false;
    private boolean             beep      = false;

    private Mat                 mRgba;
    private Mat                 mGrey;
    private Mat                 mTemp;

    public VirtucaneView(Context context) {
        super(context);
        Log.i(TAG, "VirtucaneView()");

        // Initialize MediaPlayer
        mMediaPlayer = MediaPlayer.create(context, R.raw.sine);
        mMediaPlayer.setLooping(true);
    }

    @Override
    public void surfaceChanged(SurfaceHolder _holder, int format, int width, int height) {
        super.surfaceChanged(_holder, format, width, height);
        Log.i(TAG, "surfaceChanged()");

        synchronized (this) {
            // Initialize Mats before usage
            mRgba = new Mat();
            mGrey = new Mat();
            mTemp = new Mat();
        }
    }

    @Override
    protected Bitmap processFrame(VideoCapture capture) {

        switch (VirtucaneActivity.viewMode) {

            case VirtucaneActivity.VIEW_MODE_RGBA:
                capture.retrieve(mRgba, Highgui.CV_CAP_ANDROID_COLOR_FRAME_RGBA);
                if (portrait) rotate(mRgba);
                beep = false;
                break;

            case VirtucaneActivity.VIEW_MODE_THRESHOLD:
                capture.retrieve(mGrey, Highgui.CV_CAP_ANDROID_GREY_FRAME);
                if (portrait) rotate(mGrey);

                Imgproc.threshold(mGrey, mTemp, 80, 255, Imgproc.THRESH_BINARY);

                // 0 for black; 255 for white
                int pixelValue = (int) mTemp.get(mFrameHeight / 2, mFrameWidth / 2)[0];

                // Decide if to beep or not
                if (pixelValue == 0) beep = true;
                else beep = false;
                Imgproc.cvtColor(mTemp, mRgba, Imgproc.COLOR_GRAY2RGBA, 4);
                break;

            case VirtucaneActivity.VIEW_MODE_GREY:
                capture.retrieve(mGrey, Highgui.CV_CAP_ANDROID_GREY_FRAME);
                if (portrait) rotate(mGrey);
                beep = false;
                Imgproc.cvtColor(mGrey, mRgba, Imgproc.COLOR_GRAY2RGBA, 4);
                break;

            case VirtucaneActivity.VIEW_MODE_CANNY:
                capture.retrieve(mGrey, Highgui.CV_CAP_ANDROID_GREY_FRAME);
                if (portrait) rotate(mGrey);
                Imgproc.Canny(mGrey, mTemp, 80, 100);
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

        // Draw reticle
        Core.circle(mRgba, new Point(mFrameWidth / 2, mFrameHeight / 2), 8, new Scalar(255, 0, 0,
                                                                                       255));

        Bitmap bmp = Bitmap.createBitmap(mRgba.cols(), mRgba.rows(), Bitmap.Config.ARGB_8888);

        if (Utils.matToBitmap(mRgba, bmp)) return bmp;

        bmp.recycle();
        return null;
    }

    @Override
    public void run() {
        super.run();
        Log.i(TAG, "run() complete");

        synchronized (this) {
            // Explicitly deallocate Mats
            if (mRgba != null) mRgba.release();
            if (mGrey != null) mGrey.release();
            if (mTemp != null) mTemp.release();

            mRgba = null;
            mGrey = null;
            mTemp = null;
        }
    }

    /** Rotates a matrix 90 degrees clockwise. */
    private static void rotate(Mat mat) {
        Core.transpose(mat, mat);
        Core.flip(mat, mat, 1);
    }
}
