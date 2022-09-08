package com.android.renlianshibie1.view.adapter;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.android.renlianshibie1.MainActivity;
import com.android.renlianshibie1.R;
import com.android.renlianshibie1.base.MyApplication;
import com.android.renlianshibie1.data.People;
import com.android.renlianshibie1.util.LogUtil;

import java.util.ArrayList;
import java.util.List;

/**
 * Create a recyclerview adapter.
 */
public class PhoneRecyclerviewAdapter extends RecyclerView.Adapter<PhoneRecyclerviewAdapter.ViewHolder> {

  /*  Get phone number  */
  List <People> stringList = new ArrayList<>();

  public PhoneRecyclerviewAdapter(List<People> stringList) {
    this.stringList = stringList;
  }

  /**
   * Create a ViewHolder class to inflate layout.
   */
  public class ViewHolder extends RecyclerView.ViewHolder {

    private TextView popupWindowTextView;
    private Button popupWindowButton;

    public ViewHolder(@NonNull View itemView) {
      super(itemView);

      /*  Init recyclerView control  */
      popupWindowTextView = (TextView) itemView.findViewById(R.id.popupwindow_textview);
      popupWindowButton = (Button) itemView.findViewById(R.id.popupwindow_button);
    }

  }

  /**
   * To bind ViewHolder.
   */
  @Override
  public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
    String data = stringList.get(holder.getAdapterPosition()).getPhoneNumber();
    holder.popupWindowTextView.setText(data);
  }

  /**
   * To create ViewHolder.
   */
  @NonNull
  @Override
  public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {

    /*  To inflate layout  */
    View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.popupwindow_item0, null);
    ViewHolder viewHolder = new ViewHolder(view);

    /*  Set text click listener  */
    view.setOnClickListener((v) -> {

      People people = stringList.get(viewHolder.getAdapterPosition());
      LogUtil.e("Phone", ""+viewHolder.getAdapterPosition());
//      MainActivity.editTextPhoneNumberMain.setText(people.getPhoneNumber());
//      MainActivity.editTextPasswordMain.setText(people.getPassword());
//      MainActivity.popupWindow.dismiss();
      MainActivity.getEditTextPhoneNumberMain().setText(people.getPhoneNumber());
      MainActivity.getEditTextPasswordMain().setText(people.getPassword());
      MainActivity.getPopupWindow().dismiss();
      if (!people.getPassword().equals("")) {
        MainActivity.getSaveAccountCheckBox().setChecked(true);
        MainActivity.getSavePasswordCheckBox().setChecked(true);
      } else {
        MainActivity.getSaveAccountCheckBox().setChecked(true);
      }

    });

    viewHolder.popupWindowButton.setOnClickListener((v) -> {
      SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(MyApplication.getContext()).edit();
      int position = MainActivity.findKeyValue(viewHolder.popupWindowTextView.getText().toString());
      LogUtil.e("Phone", "delete" + "phoneNumber" + position + "text = " + viewHolder.popupWindowTextView.getText().toString());
      editor.remove("phoneNumber" + position);
      editor.remove("password" + position);
      editor.apply();
      MainActivity.getStringList(MainActivity.getStringList());
      MainActivity.getPopupWindow().dismiss();
//      MainActivity.getAdapter().notifyDataSetChanged();

    });

    return viewHolder;
  }

  /**
   * To return item count.
   */
  @Override
  public int getItemCount() {
    return stringList.size();
  }

}
