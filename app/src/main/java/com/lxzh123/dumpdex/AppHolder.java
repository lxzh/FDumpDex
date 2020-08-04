package com.lxzh123.dumpdex;

import android.content.Context;
import android.os.Looper;
import android.text.TextUtils;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Method;

/**
 * description 全局 Context 持久类
 * author      Created by lxzh
 * date        2020-03-13
 */
public class AppHolder {
    private static Context mContext;

    public static void init(Context context) {
        if (context == null) {
            mContext = getContext();
        } else {
            mContext = context.getApplicationContext();
        }
    }

    public static Context getContext() {
        if (mContext == null) {
            try {
                Class ActivityThreadClass = Class.forName("android.app.ActivityThread");
                Method currentActivityThreadMethod = ActivityThreadClass.getDeclaredMethod("currentActivityThread", new Class[0]);
                Object currentActivityThread = currentActivityThreadMethod.invoke(ActivityThreadClass, new Object[0]);
                if (currentActivityThread != null) {
                    Method getApplicationMethod = currentActivityThread.getClass().getDeclaredMethod("getApplication", new Class[0]);
                    Context context = (Context) getApplicationMethod.invoke(currentActivityThread, new Object[0]);
                    if (context != null) {
                        mContext = context;
                    }
                }
            } catch (Throwable t) {
                t.printStackTrace();
            }
        }
        return mContext;
    }

    public static void clear() {
        mContext = null;
    }

    //判断是否主进程
//    public static boolean isMainProcess() {
//        try {
//            ActivityManager am = ((ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE));
//            List<ActivityManager.RunningAppProcessInfo> processes = am.getRunningAppProcesses();
//            String mainProcessName = mContext.getPackageName();
//            int myPid = android.os.Process.myPid();
//            if (processes == null || processes.size() == 0) {//手机不允许获取本机运行中的进程
//                return true;
//            }
//            for (ActivityManager.RunningAppProcessInfo info : processes) {
//                if (info.pid == myPid && mainProcessName.equals(info.processName)) {
//                    return true;
//                }
//            }
//            return false;
//        } catch (Exception e) {
//
//        }
//        return true;
//    }
    /**
     * 判断是否在主线程
     *
     * @return true为主线程 false为子线程
     */
    public static boolean isMainThread() {
        return Looper.getMainLooper() == Looper.myLooper();
    }

    public static boolean isMainProcess() {
        // 获取当前包名
        String packageName = mContext.getPackageName();
        // 获取当前进程名
        String processName = getProcessName(android.os.Process.myPid());
        return TextUtils.isEmpty(processName) || processName.equals(packageName);
    }

    /**
     * 获取进程号对应的进程名
     *
     * @param pid 进程号
     * @return 进程名
     */
    private static String getProcessName(int pid) {
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader("/proc/" + pid + "/cmdline"));
            String processName = reader.readLine();
            if (!TextUtils.isEmpty(processName)) {
                processName = processName.trim();
            }
            return processName;
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        } finally {
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException exception) {
                exception.printStackTrace();
            }
        }
        return null;
    }
}
