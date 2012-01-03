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

import java.io.IOException;
import java.util.List;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

class CameraView extends SurfaceView implements SurfaceHolder.Callback {
    private static final String TAG = "CameraView";

    private SurfaceHolder       mHolder;
    Camera                      mCamera;
    ImageProcessor              mProcessor;

    /*private boolean             aeLock  = false;
    private boolean             awbLock = false;
    private Toast               mToast;*/

    CameraView(Context context) {
        super(context);
        Log.i(TAG, "CameraView()");

        mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        // Used for processing individual frames.
        mProcessor = new ImageProcessor(context);
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated()");

        mCamera = Camera.open();
        mCamera.setPreviewCallback(mProcessor);
        try {
            mCamera.setPreviewDisplay(holder);
        } catch (IOException e) {
            Log.e(TAG, "mCamera.setPreviewDisplay() fails: " + e);
            mCamera.release();
            mCamera = null;
        }
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        Log.i(TAG, "surfaceChanged()");

        // Portrait orientation.
        mCamera.setDisplayOrientation(90);

        Camera.Parameters params = mCamera.getParameters();
        List<Size> sizes = params.getSupportedPreviewSizes();
        Size optimalSize = getOptimalPreviewSize(sizes, w, h);

        int width = optimalSize.width;
        int height = optimalSize.height;

        // Video autofocus.
        // TODO: Change to FOCUS_MODE_CONTINUOUS_PICTURE with API Level 14.
        params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        mCamera.setParameters(params);
        
        params.setPreviewSize(width, height);
        mCamera.setParameters(params);
        mCamera.startPreview();
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed()");

        mCamera.stopPreview();
        mCamera.release();
        mCamera = null;
    }

    private Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
        Log.i(TAG, "getOptimalPreviewSize()");

        final double ASPECT_TOLERANCE = 0.05;
        double targetRatio = (double) w / h;
        if (sizes == null) return null;

        Size optimalSize = null;
        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        for (Size size : sizes) {
            double ratio = (double) size.width / size.height;
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) continue;
            if (Math.abs(size.height - targetHeight) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.height - targetHeight);
            }
        }

        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Size size : sizes) {
                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;
    }

    /*public void toggleAwbLock(Context context) {
        Log.i(TAG, "toggleAwbLock()");

        Camera.Parameters params = mCamera.getParameters();
        if (params.isAutoWhiteBalanceLockSupported()) {
            if (awbLock) {
                params.setAutoWhiteBalanceLock(false);
                awbLock = false;
                mToast.cancel();
                mToast = Toast.makeText(context, "AWB lock off", Toast.LENGTH_SHORT);
                mToast.show();
            }
            else {
                params.setAutoWhiteBalanceLock(true);
                awbLock = true;
                mToast.cancel();
                mToast = Toast.makeText(context, "AWB lock on", Toast.LENGTH_SHORT);
                mToast.show();
            }
            mCamera.setParameters(params);
        }
    }*/

    /*public void toggleAeLock(Context context) {
        Log.i(TAG, "toggleAeLock()");
        
        Camera.Parameters params = mCamera.getParameters();
        if (params.isAutoExposureLockSupported()) {
            if (aeLock) {
                params.setAutoExposureLock(false);
                aeLock = false;
                mToast.cancel();
                mToast = Toast.makeText(context, "AE lock off", Toast.LENGTH_SHORT);
                mToast.show();
            }
            else {
                params.setAutoExposureLock(true);
                aeLock = true;
                mToast.cancel();
                mToast = Toast.makeText(context, "AE lock on", Toast.LENGTH_SHORT);
                mToast.show();
            }
            mCamera.setParameters(params);
        }
    }*/
}
