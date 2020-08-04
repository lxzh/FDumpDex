package com.lxzh123.dumpdex;

import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import com.lxzh123.dumpdex.dump.LowSdkDump;
import com.lxzh123.dumpdex.dump.OreoDump;
import com.lxzh123.dumpdex.util.DeviceUtils;

import java.io.File;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XSharedPreferences;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

/**
 * XposedInit
 *
 * @author lxzh123
 * @since 2018/3/20
 */
public class XposedInit implements IXposedHookLoadPackage {

    public static void log(String txt) {
        XposedBridge.log(Constants.TAG + ":XposedInit-> " + txt);
    }

    public static void log(Throwable t) {
        XposedBridge.log(t);
    }

    @Override
    public void handleLoadPackage(final XC_LoadPackage.LoadPackageParam lpparam) {
        XSharedPreferences preferences = new XSharedPreferences(MainActivity.SP_NAME);
        String packageName = preferences.getString("packagename", null);
        String appName = preferences.getString("appName", null);
        Log.d(Constants.TAG, "handleLoadPackage packageName=" + packageName + ", appName=" + appName);
        if (TextUtils.isEmpty(packageName))
            return;
        PackerInfo.Type type = PackerInfo.find(lpparam);
        Log.d(Constants.TAG, "handleLoadPackage packageName=" + packageName +
                ", lpparam.packageName=" + lpparam.packageName +
                ", lpparam.classLoader=" + lpparam.classLoader +
                ", type=" + type);
        if (type == null) {
            return;
        }
        if (packageName.equals(lpparam.packageName)) {
//            String path = "/data/data/" + packageName + "/dump";
            String path = Constants.SAVE_PREFIX + packageName + Constants.SAVE_SUFFIX;
            Log.d(Constants.TAG, "handleLoadPackage path=" + path);
            File parent = new File(path);
            boolean dirRst = true;
            if (!parent.exists() || !parent.isDirectory()) {
                dirRst = parent.mkdirs();
            }
            Log.d(Constants.TAG, "handleLoadPackage dirRst=" + dirRst);
            Log.d(Constants.TAG, "handleLoadPackage sdk version=" + Build.VERSION.SDK_INT);
            log("sdk version:" + Build.VERSION.SDK_INT);
            if (DeviceUtils.isOreo() || DeviceUtils.isPie() || DeviceUtils.isQ()) {
                OreoDump.init(lpparam);
            } else {
                LowSdkDump.init(lpparam, type);
            }
        }
    }
}
