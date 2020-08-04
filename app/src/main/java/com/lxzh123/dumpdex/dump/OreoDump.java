package com.lxzh123.dumpdex.dump;

import android.util.Log;

import com.lxzh123.dumpdex.BuildConfig;
import com.lxzh123.dumpdex.Constants;
import com.lxzh123.dumpdex.Native;

import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

/**
 * OreoDump
 *
 * @author lxzh123
 * @since 2018/3/23
 */
public class OreoDump {

    public static void log(String txt) {
        if (!BuildConfig.DEBUG) {
            return;
        }
        XposedBridge.log(Constants.TAG + ":" + txt);
    }

    public static void init(final XC_LoadPackage.LoadPackageParam lpparam) {
        Log.d(Constants.TAG, "init packageName:" + lpparam.packageName);
        Native.dump(lpparam.packageName);
    }
}
