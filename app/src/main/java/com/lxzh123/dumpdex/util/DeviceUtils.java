package com.lxzh123.dumpdex.util;

import android.os.Build;

/**
 * DeviceUtils
 *
 * @author suanlafen
 * @since 2018/4/8
 */
public class DeviceUtils {
    private static int sdkInit;

    static {
        sdkInit = Build.VERSION.SDK_INT;
    }

    public static boolean supportNativeHook() {
        return isNougat() || isMarshmallow() || isOreo();
    }

    public static boolean isMarshmallow() {
        return sdkInit == 23;
    }

    public static boolean isNougat() {
        return sdkInit == 24 || sdkInit == 25;
    }

    public static boolean isOreo() {
        return sdkInit == 26 || sdkInit == 27;
    }

    public static boolean isPie() {
        return sdkInit == 28;
    }

    public static boolean isQ() {
        return sdkInit == 29;
    }

    public static boolean isR() {
        return sdkInit == 30;
    }

}
