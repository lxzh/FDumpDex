package com.lxzh123.dumpdex;

import android.app.ActionBar;
import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends ListActivity {
    public static final String DIR = "dir";
    public static final String KEY = "packagename";
    public static final String SP_NAME = "com.lxzh123.dumpdex";
//    public static final String h = "Ubb6o57-M2ziSR9K3Jy7c7rohXGWVyh0";
    public static final String s = "FDumpDex(1.0)\nFDex2与dumpDex结合升级版-By lxzh\n\n所需环境:\n1.安卓4.4以上\n2.Xposed框架\n3.输出哪看软件提示\n\n安卓9.0 & 10.0测试通过";
    List<AppInfo> apps;
    ActionBar bar;
    PackageManager manager;
    SharedPreferences sp;
    private String selectPackageName;

    class MyAdapter extends BaseAdapter {
        private final MainActivity context;

        public MyAdapter(MainActivity mainActivity) {
            this.context = mainActivity;
        }

        @Override
        public int getCount() {
            return this.context.apps.size();
        }

        @Override
        public Object getItem(int i) {
            return this.context.apps.get(i);
        }

        @Override
        public long getItemId(int i) {
            int i2 = i;
            return (long) 0;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {
            int i2 = i;
            TextView textView = new TextView(context);
            textView.setTextSize((float) 18);
            textView.setText(context.apps.get(i2).getName());
            if (context.apps.get(i2).getPackageName().equals(selectPackageName)) {
                textView.setTextSize((float) 22);
                textView.setTextColor(-65536);
            }
            return textView;
        }
    }

    public MainActivity() {
    }

    @Override
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        this.manager = getPackageManager();
        this.sp = getSharedPreferences(SP_NAME + "_preferences", MODE_PRIVATE);
        PackageManager packageManager = getPackageManager();
        List<PackageInfo> infos = packageManager.getInstalledPackages(0);
        selectPackageName = sp.getString("packagename", "");
        apps = new ArrayList<>();

        for(PackageInfo info : infos){
            if((info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0){
                AppInfo i = new AppInfo();
                i.setName(info.applicationInfo.loadLabel(packageManager).toString());
                i.setPackageName(info.packageName);
                i.setDataDir(info.applicationInfo.dataDir);
                i.setVersion(info.versionName);
                i.setIcon(info.applicationInfo.loadIcon(packageManager));

                apps.add(i);
            }
        }
        ListAdapter listAdapter = new MyAdapter(this);
        setListAdapter(listAdapter);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return true;
    }

    /* access modifiers changed from: protected */
    @Override
    public void onListItemClick(ListView listView, View view, int i, long j) {
        AppInfo appInfo = this.apps.get(i);
        selectPackageName = appInfo.getPackageName();
        Log.d(Constants.TAG, "onListItemClick selectPackageName=" + selectPackageName);
        this.sp.edit().putString("packagename", selectPackageName).commit();
        this.sp.edit().putString("appName", appInfo.getName()).commit();
        this.sp.edit().putString("dir", appInfo.getDataDir()).commit();
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        String path = Constants.SAVE_PREFIX + selectPackageName + Constants.SAVE_SUFFIX;
        AlertDialog show = builder.setMessage("设置保存成功，请重新打开目标软件，hook包名" + "\n\ndex输出目录:" + path)
                .setPositiveButton("OK", (DialogInterface.OnClickListener) null)
                .show();
        ((MyAdapter) listView.getAdapter()).notifyDataSetChanged();
    }
}