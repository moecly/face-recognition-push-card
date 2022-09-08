package com.android.renlianshibie1;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import com.android.renlianshibie1.base.MyApplication;
import com.android.renlianshibie1.util.HttpUtil;

public class AfterLoginSecond extends Fragment implements View.OnClickListener {

  private Button admitAfterLoginButton;
  private Button afterLoginButtonExit;

//  @Override
//  protected void onCreate(Bundle savedInstanceState) {
//    super.onCreate(savedInstanceState);
//    setContentView(R.layout.activity_after_login_second);
//    initAllControl();
//    initAllButton();
//  }


  @Nullable
  @Override
  public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
    View view = inflater.inflate(R.layout.activity_after_login_second, container, false);
    initAllControl(view);
    return view;
  }

  @Override
  public void onActivityCreated(@Nullable Bundle savedInstanceState) {
    super.onActivityCreated(savedInstanceState);
    initAllButton();
  }

  private void initAllControl(View view) {
    admitAfterLoginButton = (Button) view.findViewById(R.id.admit_after_login_button);
    afterLoginButtonExit = (Button) view.findViewById(R.id.after_login_button_exit);
  }

  private void initAllButton() {
    admitAfterLoginButton.setOnClickListener(this);
    afterLoginButtonExit.setOnClickListener(this);
  }

  @Override
  public void onClick(View v) {

    switch (v.getId()) {
      case R.id.admit_after_login_button:
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getContext());
        String number = prefs.getString("loginPhone", null);
        if (number != null) {
          new Thread(() -> {
            String httpCode = HttpUtil.sendAdmitFaceData(number, MyApplication.getServletPhoneFaceUrl());
            getActivity().runOnUiThread(() -> {
              if (httpCode.equals("200")) {
                Intent intent = new Intent(getActivity(), AdmitFace.class);
                startActivity(intent);
              } //else if (httpCode.equals("302")) {
//                Toast.makeText(getContext(), "有人正在录制人脸请稍等", Toast.LENGTH_SHORT).show();
//              }
              Toast.makeText(getContext(), httpCode, Toast.LENGTH_SHORT).show();
            });
          }).start();
        } else {
          Toast.makeText(getContext(), "unknown error !", Toast.LENGTH_SHORT).show();
        }
        break;

      case R.id.after_login_button_exit:
        Intent intent = new Intent(getActivity(), MainActivity.class);
        SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getContext()).edit();
        editor.putString("loginPhone", "");
        editor.putString("loginPass", "");
        editor.putBoolean("autoLogin", false);
        editor.apply();
        startActivity(intent);
        getActivity().finish();
        break;

    }

  }

}