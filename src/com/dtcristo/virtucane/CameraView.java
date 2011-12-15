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

import java.io.IOException;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public abstract class CameraView extends SurfaceView implements SurfaceHolder.Callback, Runnable {
    private static final String TAG      = "CameraView";

    private SurfaceHolder       mHolder;
    Camera                      mCamera;

    int                         mFrameWidth;
    int                         mFrameHeight;

    boolean                     portrait = true;

    private byte[]              mFrame;
    private boolean             mThreadRun;

    public CameraView(Context context) {
        super(context);
        Log.i(TAG, "CameraView()");

        mHolder = getHolder();
        mHolder.addCallback(this);
    }

    public void surfaceChanged(SurfaceHolder _holder, int format, int width, int height) {
        Log.i(TAG, "surfaceChanged()");

        if (mCamera != null) {

            Camera.Parameters params = mCamera.getParameters();

            mFrameWidth = width;
            mFrameHeight = height;

            Log.i(TAG, "mFrameWidth = " + mFrameWidth);
            Log.i(TAG, "mFrameHeight = " + mFrameHeight);

            // List<Camera.Size> sizes = params.getSupportedPreviewSizes();

            // selecting optimal camera preview size
            // {
            // double minDiff = Double.MAX_VALUE;
            // for (Camera.Size size : sizes) {
            // if (Math.abs(size.height - height) < minDiff) {
            // mFrameWidth = size.width;
            // mFrameHeight = size.height;
            // minDiff = Math.abs(size.height - height);
            // }
            // }
            // }

            if (portrait) {
                params.setPreviewSize(mFrameHeight, mFrameWidth);
            } else {
                params.setPreviewSize(mFrameWidth, mFrameHeight);
            }

            mCamera.setParameters(params);
            try {
                mCamera.setPreviewDisplay(null);
            } catch (IOException e) {
                Log.e(TAG, "mCamera.setPreviewDisplay() fails: " + e);
            }
            mCamera.startPreview();
        }
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated()");

        mCamera = Camera.open();
        mCamera.setPreviewCallback(new PreviewCallback() {
            public void onPreviewFrame(byte[] data, Camera camera) {
                synchronized (CameraView.this) {
                    mFrame = data;
                    CameraView.this.notify();
                }
            }
        });
        (new Thread(this)).start();
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed()");

        mThreadRun = false;
        if (mCamera != null) {
            synchronized (this) {
                mCamera.stopPreview();
                mCamera.setPreviewCallback(null);
                mCamera.release();
                mCamera = null;
            }
        }
    }

    protected abstract Bitmap processFrame(byte[] data);

    public void run() {
        Log.i(TAG, "run()");

        mThreadRun = true;
        while (mThreadRun) {
            Bitmap bmp = null;

            synchronized (this) {
                try {
                    // Wait until new mFrame is available.
                    this.wait();

                    // Process the newly captured frame.
                    bmp = processFrame(mFrame);

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            if (bmp != null) {
                Canvas canvas = mHolder.lockCanvas();
                if (canvas != null) {
                    canvas.drawBitmap(bmp, (canvas.getWidth() - mFrameWidth) / 2,
                                      (canvas.getHeight() - mFrameHeight) / 2, null);
                    mHolder.unlockCanvasAndPost(canvas);
                }
                bmp.recycle();
            }
        }
        Log.i(TAG, "run() complete");
    }
}