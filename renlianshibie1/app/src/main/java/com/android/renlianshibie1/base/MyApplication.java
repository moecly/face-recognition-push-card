package com.android.renlianshibie1.base;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.graphics.Color;
import android.os.Build;
import android.view.View;
import android.view.WindowManager;

public class MyApplication extends Application {

  private static Context mContext;

  public static String getServletPhoneLoginUrl() {
    return servletPhoneLoginUrl;
  }

  public static void setServletPhoneLoginUrl(String servletPhoneLoginUrl) {
    MyApplication.servletPhoneLoginUrl = servletPhoneLoginUrl;
  }

  public static String getServletPhoneAdmitUrl() {
    return servletPhoneAdmitUrl;
  }

  public static void setServletPhoneAdmitUrl(String servletPhoneAdmitUrl) {
    MyApplication.servletPhoneAdmitUrl = servletPhoneAdmitUrl;
  }

  private static String servletPhoneAdmitUrl = "http://119.23.179.64:80/Servlet_Phone_Admit";
  private static String servletPhoneLoginUrl = "http://119.23.179.64:80/Servlet_Phone_Login";

  public static String getServletPhoneFaceUrl() {
    return servletPhoneFaceUrl;
  }

  public static void setServletPhoneFaceUrl(String servletPhoneFaceUrl) {
    MyApplication.servletPhoneFaceUrl = servletPhoneFaceUrl;
  }

  private static String servletPhoneFaceUrl = "http://119.23.179.64:80/Servlet_Phone_Face";



  public static String getKey() {
    return key;
  }

  public static void setKey(String key) {
    MyApplication.key = key;
  }

  private static String key = "Moecly";

  public static String getKey2() {
    return key2;
  }

  public static void setKey2(String key2) {
    MyApplication.key2 = key2;
  }

  private static String key2 = "Moecly324125";

  @Override
  public void onCreate() {
    super.onCreate();
    mContext = getApplicationContext();
//    LitePal.initialize(mContext);
  }

  public static Context getContext() {
    return mContext;
  }

  public static void hideStatusBar(Activity activity) {
    if (Build.VERSION.SDK_INT >= 21) {
      View decorView = activity.getWindow().getDecorView();
//      decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
      decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                                      View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
      activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
      activity.getWindow().setStatusBarColor(Color.TRANSPARENT);
    }
  }

}
