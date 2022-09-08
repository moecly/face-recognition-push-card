package com.android.renlianshibie1.util;

import android.util.Base64;

public class Base64Util {

  public static String encodeToString(String str) {
    LogUtil.e("Base64Util", "encodeToString str = " + str);
    return Base64.encodeToString(str.getBytes(), Base64.DEFAULT);
  }

  public static String decode(String str) {
    LogUtil.e("Base64Util", "decode str = " + str);
    return new String(Base64.decode(str.getBytes(), Base64.DEFAULT));
  }

}
