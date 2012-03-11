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
    
    /* Play sound
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
    }*/
}
