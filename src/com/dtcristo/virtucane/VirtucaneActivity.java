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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;

public class VirtucaneActivity extends Activity {
    private static final String TAG                 = "VirtucaneActivity";

    static final int            VIEW_MODE_RGBA      = 0;
    static final int            VIEW_MODE_THRESHOLD = 1;
    static final int            VIEW_MODE_GREY      = 2;
    static final int            VIEW_MODE_CANNY     = 3;
    private MenuItem            mMenuItemRgba;
    private MenuItem            mMenuItemThreshold;
    private MenuItem            mMenuItemGrey;
    private MenuItem            mMenuItemCanny;
    static int                  viewMode            = VIEW_MODE_RGBA;

    private CameraView          mCameraView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate()");

        mCameraView = new CameraView(this);
        setContentView(mCameraView);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "onCreateOptionsMenu()");

        mMenuItemRgba = menu.add("RGBA");
        mMenuItemThreshold = menu.add("Threshold");
        mMenuItemGrey = menu.add("Grey");
        mMenuItemCanny = menu.add("Canny");
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "onOptionsItemSelected(" + item + ")");

        if (item == mMenuItemRgba) viewMode = VIEW_MODE_RGBA;
        else if (item == mMenuItemThreshold) viewMode = VIEW_MODE_THRESHOLD;
        else if (item == mMenuItemGrey) viewMode = VIEW_MODE_GREY;
        else if (item == mMenuItemCanny) viewMode = VIEW_MODE_CANNY;
        return true;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        Log.i(TAG, "onKeyDown(" + keyCode + ")");

        switch (keyCode) {
            
            case KeyEvent.KEYCODE_DPAD_CENTER:
                // Start autofocus.
                mCameraView.mCamera.autoFocus(mCameraView.mProcessor);
                return true;
                
            case KeyEvent.KEYCODE_VOLUME_UP:
                //mCameraView.toggleAwbLock(this);
                return true;
                
            case KeyEvent.KEYCODE_VOLUME_DOWN:
                //mCameraView.toggleAeLock(this);
                return true;
        }
        return false;
    }
}
