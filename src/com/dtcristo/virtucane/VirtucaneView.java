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

import java.util.Locale;

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.media.MediaPlayer;
import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;
import android.util.Log;
import android.view.SurfaceHolder;

class VirtucaneView extends CameraView implements OnInitListener {
    private static final String TAG       = "VirtucaneView";

    private MediaPlayer         mMediaPlayer;
    private TextToSpeech        mTts;
    private AlertDialog.Builder mBuilder;
    private String              mOcrString;

    private boolean             isBeeping = false;
    private boolean             beep      = false;

    private Mat                 mYuv;
    private Mat                 mGraySubmat;

    private Mat                 mRgba;
    private Mat                 mTemp;

    public VirtucaneView(Context context) {
        super(context);
        Log.i(TAG, "VirtucaneView()");

        // Initialize MediaPlayer
        mMediaPlayer = MediaPlayer.create(context, R.raw.sine);
        mMediaPlayer.setLooping(true);

        // Initialize TextToSpeech
        mTts = new TextToSpeech(context, this);

        // Initialize AlertDialog
        mBuilder = new AlertDialog.Builder(context);
        mBuilder.setPositiveButton("Speak", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                mTts.speak(mOcrString, TextToSpeech.QUEUE_ADD, null);
                dialog.cancel();
            }
        });
        mBuilder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.cancel();
            }
        });
    }

    public void onInit(int status) {
        Log.i(TAG, "onInit(" + status + ")");

        if (status == TextToSpeech.SUCCESS) {
            Log.i(TAG, "TextToSpeech successfully loaded");

            if (mTts.isLanguageAvailable(Locale.US) == TextToSpeech.LANG_AVAILABLE) {
                mTts.setLanguage(Locale.US);
            } else Log.e(TAG, "Locale.US is unavailable");

        } else Log.e(TAG, "TextToSpeech engine failed to load");
    }

    @Override
    public void surfaceChanged(SurfaceHolder _holder, int format, int width, int height) {
        super.surfaceChanged(_holder, format, width, height);
        Log.i(TAG, "surfaceChanged()");

        synchronized (this) {
            // Initialize Mats before usage
            mYuv = new Mat(mFrameHeight + mFrameHeight / 2, mFrameWidth, CvType.CV_8UC1);
            mGraySubmat = mYuv.submat(0, mFrameHeight, 0, mFrameWidth);

            mRgba = new Mat();
            mTemp = new Mat();
        }
    }

    @Override
    protected Bitmap processFrame(byte[] data) {

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
    }

    @Override
    public void run() {
        super.run();
        Log.i(TAG, "super.run() complete");

        synchronized (this) {
            // Explicitly deallocate Mats
            if (mYuv != null) mYuv.release();
            if (mGraySubmat != null) mGraySubmat.release();

            if (mRgba != null) mRgba.release();
            if (mTemp != null) mTemp.release();

            mYuv = null;
            mGraySubmat = null;

            mRgba = null;
            mTemp = null;
        }
    }

    public void ocrFrame() {
        Log.i(TAG, "ocrFrame()");

        // TODO OCR the current frame
        mOcrString = "This text is a sample. OCR text would normally appear here.";

        mBuilder.setMessage(mOcrString);
        AlertDialog alert = mBuilder.create();
        alert.show();
    }

    /** Rotates a matrix 90 degrees clockwise. */
    private static void rotate(Mat mat) {
        Core.transpose(mat, mat);
        Core.flip(mat, mat, 0);
    }
}
