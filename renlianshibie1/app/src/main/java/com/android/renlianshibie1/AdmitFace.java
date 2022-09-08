package com.android.renlianshibie1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.android.renlianshibie1.base.MyApplication;
import com.android.renlianshibie1.util.HttpUtil;
import com.android.renlianshibie1.util.LogUtil;

public class AdmitFace extends AppCompatActivity {

//  @Override
//  protected void onDestroy() {
//    super.onDestroy();
//    String httpCode = HttpUtil.sendAdmitFaceData("", MyApplication.getServletPhoneFaceUrl());
//    LogUtil.e("AdmitFace", "httpCode = " + httpCode);
//  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    MyApplication.hideStatusBar(this);
    setContentView(R.layout.activity_admit_face);
  }
}