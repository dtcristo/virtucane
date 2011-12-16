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

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.media.MediaPlayer;
import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;
import android.util.Log;

public class OutputHandler implements OnInitListener {
    private static final String TAG = "OutputHandler";

    private MediaPlayer         mMediaPlayer;
    private TextToSpeech        mTts;
    private AlertDialog.Builder mBuilder;

    private String              ocrResult;

    public OutputHandler(Context context) {
        Log.i(TAG, "OutputHandler()");

        // Initialize MediaPlayer
        mMediaPlayer = MediaPlayer.create(context, R.raw.sine);
        mMediaPlayer.setLooping(true);

        // Initialize TextToSpeech
        mTts = new TextToSpeech(context, this);

        // Initialize AlertDialog
        mBuilder = new AlertDialog.Builder(context);
        mBuilder.setPositiveButton("Speak", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                mTts.speak(ocrResult, TextToSpeech.QUEUE_ADD, null);
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

    public void showOcrDialog(String string) {
        Log.i(TAG, "showOcrDialog(\"" + string + "\")");

        ocrResult = string;

        mBuilder.setMessage(ocrResult);
        AlertDialog dialog = mBuilder.create();
        dialog.show();
    }
}
