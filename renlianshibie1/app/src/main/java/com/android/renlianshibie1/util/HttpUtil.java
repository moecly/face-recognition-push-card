package com.android.renlianshibie1.util;

import android.text.TextUtils;
import android.widget.Toast;

import com.android.renlianshibie1.AdmitAccountActivity;
import com.android.renlianshibie1.base.MyApplication;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;

import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;

public class HttpUtil {

  private String TAG = "HttpUtil";

  public static void sendOkHttpRequest(String address, Callback callback) {

    /*  Create a http client  */
    OkHttpClient client = new OkHttpClient();

    /*  Create a request to request url  */
    Request request = new Request.Builder().url(address).build();

    /*  Open a call  */
    client.newCall(request).enqueue(callback);

  }

  public static String sendPostJson(String json, String urlStr) {

    String result = "";
    BufferedReader reader = null;
    int httpCode = 0;
    try {
      URL url = new URL(urlStr);
      HttpURLConnection conn= (HttpURLConnection) url.openConnection();
      conn.setReadTimeout(1000);
      conn.setDoOutput(true);
      conn.setDoInput(true);
      conn.setUseCaches(false);
      conn.setRequestProperty("Connection", "Keep-Alive");
      conn.setRequestProperty("Charset", "UTF-8");
      conn.setRequestProperty("Content-Type", "application/json;charset=UTF-8");
//      conn.setRequestProperty("accept", "*/*");
      conn.setRequestProperty("accept", "application/json");
      if (json != null && !TextUtils.isEmpty(json)) {
        byte[] writeBytes = json.getBytes();
        conn.setRequestProperty("Content-length", String.valueOf(writeBytes.length));
        OutputStream outputStream = conn.getOutputStream();
        outputStream.write(json.getBytes());
        outputStream.flush();
        outputStream.close();
        httpCode = conn.getResponseCode();
        LogUtil.e("HttpUtil", "do post code = " + httpCode);
      } else {
        LogUtil.e("HttpUtil", "error1");
      }
      if (httpCode == 200) {
        reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
        result = reader.readLine();
        return new String(""+httpCode);
      } else if (httpCode == 400) {
//        Toast.makeText(MyApplication.getContext(), "Account exist !", Toast.LENGTH_SHORT).show();
        return new String(""+httpCode);
      } else if (httpCode == 500) {
//        Toast.makeText(MyApplication.getContext(), "AP error !, code: 500", Toast.LENGTH_SHORT).show();
        return new String(""+httpCode);
      } else {
//        reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
//        result = reader.readLine();
        LogUtil.e("HttpUtil", "error2");
        return new String(""+httpCode);
      }
    } catch (Exception e) {
      e.printStackTrace();
      LogUtil.e("HttpUtil", "error3");
      return new String(""+httpCode);
    } finally {
      if (reader != null) {
        try {
          reader.close();
        } catch (IOException e) {
          e.printStackTrace();
          LogUtil.e("HttpUtil", "error4");
        }
      }
    }
//    LogUtil.e("HttpUtil", "result = " + result);
//    return result;

  }

  /**
   * Send admit data to AP.
   */
  public static String sendAdmitJsonData(String name, String phone, String password, String url) {

    String json = "{\"name\": \"" + name + "\", \"num\": \"" + phone + "\", \"password\": \"" + password + "\"}";
    String httpCode = sendPostJson(json, url);
    return httpCode;

  }

  /**
   * Send admit face data to ap.
   */
  public static String sendAdmitFaceData(String phone, String url) {

    String json = "{\"num\": \"" + phone + "\"}";
    String httpCode = sendPostJson(json, url);
    return httpCode;

  }

}
