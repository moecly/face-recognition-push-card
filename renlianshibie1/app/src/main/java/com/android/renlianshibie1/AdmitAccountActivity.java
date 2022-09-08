package com.android.renlianshibie1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.method.DigitsKeyListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.renlianshibie1.base.MyApplication;
import com.android.renlianshibie1.util.Base64Util;
import com.android.renlianshibie1.util.EditTextUtil;
import com.android.renlianshibie1.util.HttpUtil;

public class AdmitAccountActivity extends AppCompatActivity {

  private TextView phoneAdmitTextView;
  private TextView passwordAdmitTextView;
  private TextView nameAdmitTextView;
  private EditText phoneAdmitEditText;
  private EditText passwordAdmitEditText;
  private EditText nameAdmitEditText;
  private Button admitButton;
  private String name = "";
  private String pass = "";
  private String phone = "";
  private String httpCode = "";

  @Override
  protected void onCreate(Bundle savedInstanceState) {

    super.onCreate(savedInstanceState);
    MyApplication.hideStatusBar(this);
    setContentView(R.layout.activity_admit_account);
    initAllControl();
//    setNameAdmitEditText();
    EditTextUtil.setNameAdmitEditText(nameAdmitEditText);
    EditTextUtil.setEditTextLogin(phoneAdmitEditText);
    EditTextUtil.setEditTextPassword(passwordAdmitEditText);
//    setPhoneAdmitEditText();
//    setPasswordAdmitEditText();
    admitButton();

  }

  /**
   * Init all control.
   */
  private void initAllControl() {

    phoneAdmitEditText = (EditText) findViewById(R.id.phone_admit_edittext);
    passwordAdmitEditText = (EditText) findViewById(R.id.password_admit_edittext);
    nameAdmitEditText = (EditText) findViewById(R.id.name_admit_edittext);
    phoneAdmitTextView = (TextView) findViewById(R.id.phone_admit_textview);
    passwordAdmitTextView = (TextView) findViewById(R.id.password_admit_textview);
    nameAdmitTextView = (TextView) findViewById(R.id.name_admit_textview);
    admitButton = (Button) findViewById(R.id.admit_button);

  }

  /**
   * Edit text login init.
   */
  private void setPhoneAdmitEditText() {

    /*  Set input digits  */
    String digits = "0123456789-+";
    phoneAdmitEditText.setKeyListener(DigitsKeyListener.getInstance(digits));
    phoneAdmitEditText.addTextChangedListener(new TextWatcher() {
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
        selectionStart = phoneAdmitEditText.getSelectionStart();
        selectionEnd = phoneAdmitEditText.getSelectionEnd();

        /*  If length more 11, delete and toast  */
        if (temp.length() > 11) {
          Toast.makeText(AdmitAccountActivity.this, "You entered more than 11 characters", Toast.LENGTH_SHORT).show();
          s.delete(selectionStart-1, selectionEnd);
          int tempSelection = selectionStart;
          phoneAdmitEditText.setText(s);
          phoneAdmitEditText.setSelection(tempSelection);
        }

      }
    });
  }

  /**
   * Edit text password init.
   */
  private void setPasswordAdmitEditText () {

    /*  Edit text listener  */
    passwordAdmitEditText.addTextChangedListener(new TextWatcher() {
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
        selectionStart = passwordAdmitEditText.getSelectionStart();
        selectionEnd = passwordAdmitEditText.getSelectionEnd();

        /*  If length more 20, delete and toast  */
        if (temp.length() > 20) {
          Toast.makeText(AdmitAccountActivity.this, "The password you entered is too long", Toast.LENGTH_SHORT).show();
          s.delete(selectionStart-1, selectionEnd);
          int tempSelection = selectionStart;
          passwordAdmitEditText.setText(s);
          passwordAdmitEditText.setSelection(tempSelection);
        }

      }
    });
  }

  /**
   * Edit text name edittext init.
   */
  private void setNameAdmitEditText () {

    /*  Edit text listener  */
//    String digits = "0123456789qwertyuiopasdfghjklzxcvbnm ";
//    nameAdmitEditText.setKeyListener(DigitsKeyListener.getInstance(digits));
    nameAdmitEditText.addTextChangedListener(new TextWatcher() {
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
        selectionStart = nameAdmitEditText.getSelectionStart();
        selectionEnd = nameAdmitEditText.getSelectionEnd();

        /*  If length more 20, delete and toast  */
        if (temp.length() > 20) {
          Toast.makeText(AdmitAccountActivity.this, "The name you entered is too long", Toast.LENGTH_SHORT).show();
          s.delete(selectionStart-1, selectionEnd);
          int tempSelection = selectionStart;
          nameAdmitEditText.setText(s);
          nameAdmitEditText.setSelection(tempSelection);
        }

      }
    });
  }

  /**
   * Create and admit a user button.
   */
  private void admitButton() {

    admitButton.setOnClickListener((v -> {
      name = "";
      pass = "";
      phone = "";
      name = nameAdmitEditText.getText().toString();
      pass = passwordAdmitEditText.getText().toString();
      phone = phoneAdmitEditText.getText().toString();
      if (name == null || pass == null || phone == null || name.equals("") || pass.equals("") || phone.equals("") || pass.length() < 8) {
        Toast.makeText(AdmitAccountActivity.this, "You input data error", Toast.LENGTH_SHORT).show();
      } else {
        new Thread(() -> {
          httpCode = HttpUtil.sendAdmitJsonData(name, phone, Base64Util.encodeToString(pass), MyApplication.getServletPhoneAdmitUrl());
          try {
            Thread.sleep(500);
          } catch (InterruptedException e) {
            e.printStackTrace();
          }
          runOnUiThread(() -> {
            if (httpCode.equals("400")) {
                Toast.makeText(this, "Account exist !", Toast.LENGTH_SHORT).show();
              } else if (httpCode.equals("500")) {
                Toast.makeText(this, "httpCode: " + httpCode, Toast.LENGTH_SHORT).show();
              } else {
                Toast.makeText(this, httpCode, Toast.LENGTH_SHORT).show();
              }
          });
        }).start();

      }
    }));
  }

}