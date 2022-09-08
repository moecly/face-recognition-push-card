package com.android.renlianshibie1.util;

import android.text.Editable;
import android.text.TextWatcher;
import android.text.method.DigitsKeyListener;
import android.text.method.PasswordTransformationMethod;
import android.widget.EditText;
import android.widget.Toast;

import com.android.renlianshibie1.AdmitAccountActivity;
import com.android.renlianshibie1.MainActivity;
import com.android.renlianshibie1.base.MyApplication;

public class EditTextUtil {
  /**
   * Edit text login init.
   */
   public static void setEditTextLogin(EditText editTextPhoneNumberMain) {

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
          Toast.makeText(MyApplication.getContext(), "You entered more than 11 characters", Toast.LENGTH_SHORT).show();
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
  public static void setEditTextPassword(EditText editTextPasswordMain) {

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
          Toast.makeText(MyApplication.getContext(), "The password you entered is too long", Toast.LENGTH_SHORT).show();
          s.delete(selectionStart-1, selectionEnd);
          int tempSelection = selectionStart;
          editTextPasswordMain.setText(s);
          editTextPasswordMain.setSelection(tempSelection);
        }

      }
    });
  }

  /**
   * Edit text name edittext init.
   */
  public static void setNameAdmitEditText (EditText nameAdmitEditText) {

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
          Toast.makeText(MyApplication.getContext(), "The name you entered is too long", Toast.LENGTH_SHORT).show();
          s.delete(selectionStart-1, selectionEnd);
          int tempSelection = selectionStart;
          nameAdmitEditText.setText(s);
          nameAdmitEditText.setSelection(tempSelection);
        }

      }
    });
  }
}
