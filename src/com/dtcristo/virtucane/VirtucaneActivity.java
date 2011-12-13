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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class VirtucaneActivity extends Activity {
    private static final String TAG                 = "VirtucaneActivity";

    public static final int     VIEW_MODE_RGBA      = 0;
    public static final int     VIEW_MODE_THRESHOLD = 1;
    public static final int     VIEW_MODE_GREY      = 2;
    public static final int     VIEW_MODE_CANNY     = 3;

    private MenuItem            mMenuItemRgba;
    private MenuItem            mMenuItemThreshold;
    private MenuItem            mMenuItemGrey;
    private MenuItem            mMenuItemCanny;

    public static int           viewMode            = VIEW_MODE_RGBA;

    public VirtucaneActivity() {
        Log.i(TAG, "VirtualCaneActivity()");
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate()");

        setContentView(new VirtucaneView(this));
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

}
