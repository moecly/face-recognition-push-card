package com.android.renlianshibie1;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class AfterLoginThree extends AppCompatActivity implements View.OnClickListener {

  private Button afterLoginButtonRecord;
  private Button afterLoginButtonPerson;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_after_login_three);
    initAllControl();
    initAllButton();
  }

  private void initAllControl() {
    afterLoginButtonRecord = (Button) findViewById(R.id.after_login_button_record);
    afterLoginButtonPerson = (Button) findViewById(R.id.after_login_button_person);
  }

  private void initAllButton() {
    afterLoginButtonRecord.setOnClickListener(this);
    afterLoginButtonPerson.setOnClickListener(this);
  }

  @Override
  public void onClick(View v) {
    if (v.getId() == R.id.after_login_button_person) {
      replaceFragment(new AfterLoginSecond());
    } else {
      replaceFragment(new AfterLogin());
    }
  }

  private void replaceFragment(Fragment fragment) {
    FragmentManager fragmentManager = getSupportFragmentManager();
    FragmentTransaction transaction = fragmentManager.beginTransaction();
    transaction.replace(R.id.after_login_fragment_record, fragment);
    transaction.commit();
  }

}