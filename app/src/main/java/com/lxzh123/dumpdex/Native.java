package com.lxzh123.dumpdex;

import android.util.Log;

import de.robv.android.xposed.XposedBridge;

/**
 * Native
 *
 * @author lxzh123
 * @since 2018/3/23
 */
public class Native {
    public static void log(String txt) {
        XposedBridge.log(Constants.TAG + ":" + txt);
    }
    static {
        if(!loadLibrary("nativeDump")) {
            log("load library failed;");
            Log.d(Constants.TAG, "loadLibrary error");
        }
    }

    private static boolean loadLibrary(String path) {
        boolean result;
        try {
            System.loadLibrary(path);
            result = true;
        } catch (Throwable t) {
            t.printStackTrace();
            Log.d(Constants.TAG, "loadLibrary error:" + t.toString());
            result = false;
        }
        log("loadLibrary path=" + path + ", result=" + result);
        return result;
    }

    private static boolean load(String path) {
        boolean result;
        try {
            System.load(path);
            result = true;
        } catch (Throwable t) {
            t.printStackTrace();
            Log.d(Constants.TAG, "load library error:" + t.toString());
            result = false;
        }
        log("load path=" + path + ", result=" + result);
        return result;
    }

    public static native void dump(String packageName);
}
