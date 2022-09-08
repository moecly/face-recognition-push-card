package com.android.renlianshibie1.base;

import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import org.litepal.util.LogUtil;

public class BaseActivity extends AppCompatActivity {

  /*  Set TAG  */
  private String TAG = "BaseActivity";

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    LogUtil.d(TAG, getClass().getSimpleName());

    /*  Activity list add activity  */
    ActivityCollector.addActivity(this);
  }

  @Override
  protected void onDestroy() {
    super.onDestroy();

    /*  Activity list remove activity  */
    ActivityCollector.removeActivity(this);
  }

}
