package com.android.renlianshibie1;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.method.DigitsKeyListener;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.PopupWindow;
import android.widget.Toast;

import androidx.cardview.widget.CardView;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.android.renlianshibie1.base.BaseActivity;
import com.android.renlianshibie1.base.MyApplication;
import com.android.renlianshibie1.data.People;
import com.android.renlianshibie1.util.Base64Util;
import com.android.renlianshibie1.util.EditTextUtil;
import com.android.renlianshibie1.util.HttpUtil;
import com.android.renlianshibie1.util.LogUtil;
import com.android.renlianshibie1.view.adapter.PhoneRecyclerviewAdapter;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends BaseActivity implements View.OnClickListener, View.OnTouchListener, CompoundButton.OnCheckedChangeListener {

  private String TAG = "MainActivity";
  private FloatingActionButton buttonSendMain;
  private Button displayPasswordButton;
  private static Button displayMoreButton;
  private static List<People> stringList = new ArrayList<>();
  private Button clearPasswordButton;
  private static EditText editTextPhoneNumberMain;
  private static EditText editTextPasswordMain;
  private String jsonData = "" ;
  private static CheckBox savePasswordCheckBox;
  private static CheckBox saveAccountCheckBox;
  private static CheckBox autoLoginCheckBox;
  private static PopupWindow popupWindow;
  private CardView mainCardView;
  private static int MAX_PHONE_NUMBER = 4;
  private Button admitAccountButton;
  private Button retrievePasswordButton;
  private String responseData = "";
  private boolean showPwd = true;
  private boolean saveAcc = false;
  private static boolean saveHaveAcc = false;
  private static PhoneRecyclerviewAdapter adapter;
  private boolean savePwd = false;
  private boolean autoLog = false;

  @Override
  protected void onCreate(Bundle savedInstanceState) {

    super.onCreate(savedInstanceState);
//    hideStatusBar();
    MyApplication.hideStatusBar(this);
    setContentView(R.layout.activity_main);
    checkAutoLogin();
    initAllControl();
    initRecyclerview(adapter);
    getStringList(stringList);
    initAllButton();
    initAllEditText();
    initCheckBox();
//    setEditTextLogin(editTextPhoneNumberMain);
//    setEditTextPassword(editTextPasswordMain);
    EditTextUtil.setEditTextLogin(editTextPhoneNumberMain);
    EditTextUtil.setEditTextPassword(editTextPasswordMain);

  }

  private void checkAutoLogin() {
    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
    if (prefs.getBoolean("autoLogin", false)) {
      LogUtil.e(TAG, "autoLogin yes");
      Intent intent = new Intent(MainActivity.this, AfterLoginThree.class);
      startActivity(intent);
      finish();
    }
  }

  @Override
  public boolean onTouch(View v, MotionEvent event) {

    int action = event.getAction();
    switch (v.getId()) {

      /*  Set touch event  */
      case R.id.display_password_button:
        if (action == MotionEvent.ACTION_DOWN) {
          displayPasswordButton.setBackgroundResource(R.drawable.hide_password);
          int temp = editTextPasswordMain.getSelectionStart();
          editTextPasswordMain.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
          editTextPasswordMain.setSelection(temp);

        } else if (action == MotionEvent.ACTION_UP) {
          displayPasswordButton.setBackgroundResource(R.drawable.display_password);
          int temp = editTextPasswordMain.getSelectionStart();
          editTextPasswordMain.setTransformationMethod(PasswordTransformationMethod.getInstance());
          editTextPasswordMain.setSelection(temp);
        }
        break;

    }
    return true;

  }

  public static List<People> getStringList() {
    return stringList;
  }

  public static void setStringList(List<People> stringList) {
    MainActivity.stringList = stringList;
  }

  public static Button getDisplayMoreButton() {
    return displayMoreButton;
  }

  public static void setDisplayMoreButton(Button displayMoreButton) {
    MainActivity.displayMoreButton = displayMoreButton;
  }

  public static PhoneRecyclerviewAdapter getAdapter() {
    return adapter;
  }

  public static void setAdapter(PhoneRecyclerviewAdapter adapter) {
    MainActivity.adapter = adapter;
  }

  public static EditText getEditTextPhoneNumberMain() {
    return editTextPhoneNumberMain;
  }

  public static void setEditTextPhoneNumberMain(EditText editTextPhoneNumberMain) {
    MainActivity.editTextPhoneNumberMain = editTextPhoneNumberMain;
  }

  public static EditText getEditTextPasswordMain() {
    return editTextPasswordMain;
  }

  public static void setEditTextPasswordMain(EditText editTextPasswordMain) {
    MainActivity.editTextPasswordMain = editTextPasswordMain;
  }

  public static PopupWindow getPopupWindow() {
    return popupWindow;
  }

  public static void setPopupWindow(PopupWindow popupWindow) {
    MainActivity.popupWindow = popupWindow;
  }

  /**
   * Button clicked event.
   */
  @Override
  public void onClick(View v) {

//    Intent intent;
    switch (v.getId()) {

      /*  Send admit data  */
      case R.id.button_send_main:
        LogUtil.e("HttpUtil", "start send");
        String phone = "";
        String pass = "";
        phone = editTextPhoneNumberMain.getText().toString();
        pass = editTextPasswordMain.getText().toString();
        if (pass == null || phone == null || pass.equals("") || phone.equals("") || pass.length() < 8) {
          Toast.makeText(MainActivity.this, "You input data error", Toast.LENGTH_SHORT).show();
//          return ;
          break;
        }
        jsonData = "{\"num\": \"" + phone + "\", \"password\": \"" + Base64Util.encodeToString(pass) + "\"}";
        LogUtil.e("HttpUtil", jsonData);
//        Intent intent = new Intent(MainActivity.this, AfterLogin.class);
//        intent = new Intent(MainActivity.this, AfterLogin.class);
//        startActivity(intent);
//        saveJudge(saveAcc, savePwd, autoLog, editTextPhoneNumberMain.getText().toString(), editTextPasswordMain.getText().toString());
//        finish();

        new Thread(() -> {
          responseData = HttpUtil.sendPostJson(jsonData, MyApplication.getServletPhoneLoginUrl());
          runOnUiThread(() -> {
            if (responseData.equals("400")) {
              Toast.makeText(MainActivity.this, "Incorrect account or password", Toast.LENGTH_SHORT).show();
            } else if (responseData.equals("200")) {
              SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(this).edit();
              editor.putString("loginPhone", editTextPhoneNumberMain.getText().toString());
              editor.putString("loginPass", editTextPasswordMain.getText().toString());
              editor.apply();
              Intent intent = new Intent(MainActivity.this, AfterLoginThree.class);
              startActivity(intent);
              finish();
              saveJudge(saveAcc, savePwd, autoLog, editTextPhoneNumberMain.getText().toString(), editTextPasswordMain.getText().toString());
            } else {
//              Intent intent = new Intent(MainActivity.this, AfterLogin.class);
//              startActivity(intent);
//              finish();
//              saveJudge(saveAcc, savePwd, autoLog, editTextPhoneNumberMain.getText().toString(), editTextPasswordMain.getText().toString());
            }
          });
        }).start();

        break;

      case R.id.clear_password_button:
        editTextPasswordMain.setText("");
        break;

      case R.id.display_more_main:
        showPopupWindow(popupWindow, mainCardView);
        break;

      case R.id.editText_password_main:
        LogUtil.e(TAG, "onclickPass");
//        editTextPasswordMain.setFocusableInTouchMode(true);
        editTextPasswordMain.requestFocus();
        editTextPasswordMain.requestFocusFromTouch();
        clearPasswordButton.setVisibility(View.VISIBLE);
        displayPasswordButton.setVisibility(View.VISIBLE);
        displayMoreButton.setVisibility(View.INVISIBLE);
        hideKeyBoard(editTextPasswordMain, false);
        break;

      case R.id.editText_phoneNumber_main:
        LogUtil.e(TAG, "onclickPhone");
//        editTextPhoneNumberMain.setFocusableInTouchMode(true);
        editTextPhoneNumberMain.requestFocus();
        editTextPhoneNumberMain.requestFocusFromTouch();
//        savePasswordCheckBox.setVisibility(View.INVISIBLE);
        clearPasswordButton.setVisibility(View.INVISIBLE);
        displayPasswordButton.setVisibility(View.INVISIBLE);
        hideKeyBoard(editTextPhoneNumberMain, false);
        if (saveHaveAcc) {
          displayMoreButton.setVisibility(View.VISIBLE);
        }
        break;

      case R.id.admit_account_button:
        Intent intent = new Intent(MainActivity.this, AdmitAccountActivity.class);
//        intent = new Intent(MainActivity.this, AdmitAccountActivity.class);
        startActivity(intent);
        break;

      case R.id.retrieve_password_button:
        Intent intent1 = new Intent(MainActivity.this, RetrievePassword.class);
        startActivity(intent1);
        break;

    }

  }

  private void initAllEditText() {
    editTextPasswordMain.setOnClickListener(this);
    editTextPhoneNumberMain.setOnClickListener(this);
  }

  /**
   * Edit text login init.
   */
  private void setEditTextLogin(EditText editTextPhoneNumberMain) {

    /*  Set input digits  */
    String digits = "0123456789-+";
    editTextPhoneNumberMain.setKeyListener(DigitsKeyListener.getInstance(digits));
    editTextPhoneNumberMain.addTextChangedListener(new TextWatcher() {
      private CharSequence temp;
      private int selectionStart;
      private int selectionEnd;
      @Override
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
        temp = s;
//        displayPasswordButton.setVisibility(View.INVISIBLE);
//        if (saveHaveAcc) {
//          displayMoreButton.setVisibility(View.VISIBLE);
//        }
//        clearPasswordButton.setVisibility(View.INVISIBLE);
      }

      @Override
      public void onTextChanged(CharSequence s, int start, int before, int count) {
      }

      @Override
      public void afterTextChanged(Editable s) {

        /*  Get selection  */
        selectionStart = editTextPhoneNumberMain.getSelectionStart();
        selectionEnd = editTextPhoneNumberMain.getSelectionEnd();

        /*  If length more 11, delete and toast  */
        if (temp.length() > 11) {
          Toast.makeText(MainActivity.this, "You entered more than 11 characters", Toast.LENGTH_SHORT).show();
          s.delete(selectionStart-1, selectionEnd);
          int tempSelection = selectionStart;
          editTextPhoneNumberMain.setText(s);
          editTextPhoneNumberMain.setSelection(tempSelection);
        }

      }
    });
  }

  /**
   * Edit text password init.
   */
  private void setEditTextPassword(EditText editTextPasswordMain) {

    /*  Edit text listener  */
    editTextPasswordMain.setTransformationMethod(PasswordTransformationMethod.getInstance());
    editTextPasswordMain.addTextChangedListener(new TextWatcher() {
      private CharSequence temp;
      private int selectionStart;
      private int selectionEnd;
      @Override
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
        temp = s;
//        displayPasswordButton.setVisibility(View.VISIBLE);
//        clearPasswordButton.setVisibility(View.VISIBLE);
//        displayMoreButton.setVisibility(View.INVISIBLE);
      }

      @Override
      public void onTextChanged(CharSequence s, int start, int before, int count) {
      }

      @Override
      public void afterTextChanged(Editable s) {

        /*  Get selection  */
        selectionStart = editTextPasswordMain.getSelectionStart();
        selectionEnd = editTextPasswordMain.getSelectionEnd();

        /*  If length more 20, delete and toast  */
        if (temp.length() > 20) {
          Toast.makeText(MainActivity.this, "The password you entered is too long", Toast.LENGTH_SHORT).show();
          s.delete(selectionStart-1, selectionEnd);
          int tempSelection = selectionStart;
          editTextPasswordMain.setText(s);
          editTextPasswordMain.setSelection(tempSelection);
        }

      }
    });
  }

  /**
   * Set send button init.
   */
  private void initAllButton() {
    buttonSendMain.setBackgroundResource(R.drawable.duihao1);
    buttonSendMain.setOnClickListener(this);
    admitAccountButton.setOnClickListener(this);
    retrievePasswordButton.setOnClickListener(this);
    displayPasswordButton.setOnTouchListener(this);
    clearPasswordButton.setOnClickListener(this);
    displayMoreButton.setOnClickListener(this);
  }

  /**
   * Hide UI flag.
   */
  private void hideStatusBar() {
    if (Build.VERSION.SDK_INT >= 21) {
      View decorView = getWindow().getDecorView();
      decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
      getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
      getWindow().setStatusBarColor(Color.TRANSPARENT);
    }
  }

  /**
   * Init all Control.
   */
  private void initAllControl() {

    mainCardView = (CardView) findViewById(R.id.card_main);
    buttonSendMain = (FloatingActionButton) findViewById(R.id.button_send_main);
    editTextPasswordMain = (EditText) findViewById(R.id.editText_password_main);
    displayPasswordButton = (Button) findViewById(R.id.display_password_button);
    displayMoreButton = (Button) findViewById(R.id.display_more_main);
    clearPasswordButton = (Button) findViewById(R.id.clear_password_button);
    editTextPhoneNumberMain = (EditText) findViewById(R.id.editText_phoneNumber_main);
    savePasswordCheckBox = (CheckBox) findViewById(R.id.save_password_checkbox);
    saveAccountCheckBox = (CheckBox) findViewById(R.id.save_account_checkbox);
    autoLoginCheckBox = (CheckBox) findViewById(R.id.auto_login_checkbox);
    retrievePasswordButton = (Button) findViewById(R.id.retrieve_password_button);
    admitAccountButton = (Button) findViewById(R.id.admit_account_button);

  }

  /**
   * Show popup window.
   */
  private void showPopupWindow(PopupWindow popupWindow, CardView mainCardView) {

    popupWindow.showAsDropDown(mainCardView, 0, 27);

  }

  /**
   * Init recyclerview.
   */
  private void initRecyclerview(PhoneRecyclerviewAdapter adapter) {

//    View view = LayoutInflater.from(MainActivity.this).inflate(R.layout.recycler_item, null);
    View view = getLayoutInflater().inflate(R.layout.recycler_item, null);
    RecyclerView recyclerViewMain = view.findViewById(R.id.recyclerview_main);
//    stringList.add("2323525232");
//    stringList.add("2378525623");
    adapter = new PhoneRecyclerviewAdapter(stringList);
    LinearLayoutManager manager = new LinearLayoutManager(MyApplication.getContext());
    recyclerViewMain.setAdapter(adapter);
    recyclerViewMain.setLayoutManager(manager);
    popupWindow = new PopupWindow(MyApplication.getContext());
//    LogUtil.e(TAG, "re weight and height:" + recyclerViewMain.getVerticalScrollbarWidth() + recyclerViewMain.getMeasuredWidth());
    popupWindow.setWidth(this.getWindowManager().getDefaultDisplay().getWidth() - 160);
    popupWindow.setHeight(140);
    popupWindow.setContentView(view);
    popupWindow.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
    popupWindow.setOutsideTouchable(false);
//    popupWindow.setTouchable(true);
    popupWindow.setFocusable(true);

  }

  /**
   * Init all checkbox.
   */
  private void initCheckBox() {
    saveAccountCheckBox.setOnCheckedChangeListener(this);
    savePasswordCheckBox.setOnCheckedChangeListener(this);
    autoLoginCheckBox.setOnCheckedChangeListener(this);
  }

  /**
   * Set checked changed event listener.
   */
  @Override
  public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

    switch (buttonView.getId()) {

      case R.id.save_account_checkbox:
        if (isChecked) {
          savePasswordCheckBox.setVisibility(View.VISIBLE);
          saveAcc = true;
          savePwd = false;
          autoLog = false;
        } else if (!isChecked) {
          savePasswordCheckBox.setVisibility(View.INVISIBLE);
          autoLoginCheckBox.setVisibility(View.INVISIBLE);
          savePasswordCheckBox.setChecked(false);
          autoLoginCheckBox.setChecked(false);
          saveAcc = false;
          savePwd = false;
          autoLog = false;
        }
        break;

      case R.id.save_password_checkbox:
        if (isChecked) {
          autoLoginCheckBox.setVisibility(View.VISIBLE);
          saveAcc = true;
          savePwd = true;
          autoLog = false;
        } else if (!isChecked) {
          autoLoginCheckBox.setVisibility(View.INVISIBLE);
          autoLoginCheckBox.setChecked(false);
          saveAcc = true;
          savePwd = false;
          autoLog = false;
        }
        break;

      case R.id.auto_login_checkbox:
        if (isChecked) {
          saveAcc = true;
          savePwd = true;
          autoLog = true;
        } else if (!isChecked) {
          saveAcc = true;
          savePwd = true;
          autoLog = false;
        }
        break;

    }

  }

  /**
   * To operation login logic.
   */
  private void saveJudge(boolean saveAcc, boolean savePwd, boolean autoLog, String acc, String pwd) {

    if (pwd.length() <= 8) {
      Toast.makeText(this, "Password length must more than 8", Toast.LENGTH_SHORT).show();
      return ;
    }
    SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(this).edit();
    if (saveAcc && savePwd && autoLog) {
      LogUtil.e(TAG, "saveAcc && savePwd && autoLog");
      saveData(acc, pwd);
      editor.putBoolean("autoLogin", true);
      editor.apply();
    } else if (saveAcc && savePwd) {
      LogUtil.e(TAG, "saveAcc && savePwd");
      saveData(acc, pwd);
      editor.putBoolean("autoLogin", false);
      editor.apply();
    } else if (saveAcc) {
      LogUtil.e(TAG, "saveAcc");
      saveData(acc, "");
      editor.putBoolean("autoLogin", false);
      editor.apply();
    }

  }

  /**
   * To save data to phone.
   */
  private void saveData(String saveAcc, String savePwd) {
    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
    boolean value = false;
    boolean exist = false;
    int i;
    LogUtil.e(TAG, "start save");
    if (saveAcc.equals("")/* || savePwd.equals("")*/) {
      LogUtil.e(TAG, "return ;");
      return ;
    }
    for (i = 0; i < MAX_PHONE_NUMBER; i ++) {
      String phoneNumber = prefs.getString("phoneNumber" + i, null);
      if (phoneNumber != null){
        if (Base64Util.decode(phoneNumber).equals(saveAcc)) {
          value = true;
          LogUtil.e(TAG, "found value");
          LogUtil.e(TAG, "i = " + i);
          break;
        }
      }
    }
    if (value == true) {
      SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(this).edit();
      editor.putString("phoneNumber" + i, Base64Util.encodeToString(saveAcc));
      editor.putString("password" + i, Base64Util.encodeToString(savePwd));
      editor.apply();
      LogUtil.e(TAG, "save success");
    } else {
      int j;
      LogUtil.e(TAG, "no found value");
      for (j = 0; j < MAX_PHONE_NUMBER; j ++) {
        if (prefs.getString("phoneNumber" + j, null) == null) {
          LogUtil.e(TAG, "exist true");
          LogUtil.e(TAG, "j = " + j);
          exist = true;
          break;
        }
      }
      if (exist) {
        SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(this).edit();
        editor.putString("phoneNumber" + j, Base64Util.encodeToString(saveAcc));
        editor.putString("password" + j, Base64Util.encodeToString(savePwd));
        editor.apply();
        LogUtil.e(TAG, "save success");
      }
    }
  }

  /**
   * Get List<People> value.
   */
   public static void getStringList(List<People> stringList) {

    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(MyApplication.getContext());
    String str = "";
    stringList.clear();
    saveHaveAcc = false;

    /*  Find key value from prefs  */
    for (int i = 0; i < MAX_PHONE_NUMBER; i ++) {
      if ((str = prefs.getString("phoneNumber" + i, null)) != null) {
        saveHaveAcc = true;
        People people = new People();
        people.setPhoneNumber(Base64Util.decode(str));
        LogUtil.e("Phone", "" + Base64Util.decode(str));
        people.setPassword(Base64Util.decode(prefs.getString("password" + i, "")));
        stringList.add(people);
      }
    }

    /*  If no have acc  */
    if (!saveHaveAcc) {
      if (MainActivity.displayMoreButton != null) {
        MainActivity.displayMoreButton.setVisibility(View.INVISIBLE);
      }
    }
   }

  /**
   * Show hide soft key board.
   */
  private void hideKeyBoard(View view, boolean b) {
    if (b) {
      ((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
    } else if (!b) {
      ((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE)).showSoftInput(view, 0);
    }
  }

  /**
   * Find key value from prefs.
   */
  public static int findKeyValue(String key) {
    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(MyApplication.getContext());
    String result;
    String decodeResult;
    for (int i = 0; i < MAX_PHONE_NUMBER; i ++) {
      result = prefs.getString("phoneNumber" + i, null);
      if (result != null) {
        decodeResult = Base64Util.decode(result);
        if (decodeResult.equals(key)) {
          return i;
        }
      }
    }
    return -1;
  }

  static public CheckBox getSavePasswordCheckBox() {
    return savePasswordCheckBox;
  }

  static public CheckBox getSaveAccountCheckBox() {
    return saveAccountCheckBox;
  }

  static public CheckBox getAutoLoginCheckBox() {
    return autoLoginCheckBox;
  }

}