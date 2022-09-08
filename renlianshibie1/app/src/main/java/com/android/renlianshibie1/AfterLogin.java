package com.android.renlianshibie1;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.android.renlianshibie1.data.Dao1;
import com.android.renlianshibie1.data.Druid_until_1;
import com.android.renlianshibie1.base.MyApplication;
import com.android.renlianshibie1.data.AfterLoginData;
import com.android.renlianshibie1.util.HttpUtil;
import com.android.renlianshibie1.util.LogUtil;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;

public class AfterLogin extends Fragment  {

  private String TAG = "AfterLogin";
//  private Button admitAfterLoginButton;
//  private Button afterLoginButtonRecord;
//  private Button afterLoginButtonPerson;
  private List<AfterLoginData> list = new ArrayList<>();
  private AfterLoginAdapter adapter;
  private SwipeRefreshLayout afterLoginSwipeRecord;

//  @Override
//  protected void onCreate(Bundle savedInstanceState) {
//    super.onCreate(savedInstanceState);
//    MyApplication.hideStatusBar(this);
//    setContentView(R.layout.activity_after_login);
//    initAfterLoginRecyclerView();
//    getAfterLoginDataList();
//  }


  @Nullable
  @Override
  public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
    View view = inflater.inflate(R.layout.activity_after_login, container, false);
    afterLoginSwipeRecord = (SwipeRefreshLayout) view.findViewById(R.id.after_login_swipe_record);
    initAfterLoginRecyclerView(view);
    return view;
  }

  @Override
  public void onActivityCreated(@Nullable Bundle savedInstanceState) {
    super.onActivityCreated(savedInstanceState);
    afterLoginSwipeRecord.setColorSchemeResources(R.color.design_default_color_primary);
    afterLoginSwipeRecord.setOnRefreshListener(() -> {
      getAfterLoginDataList();
      afterLoginSwipeRecord.setRefreshing(false);
    });
    getAfterLoginDataList();
  }

  private void getAfterLoginDataList() {
    new Thread(() -> {
      SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(MyApplication.getContext());
      String num = prefs.getString("loginPhone", "");
      Date date = new Date();
      SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
      LogUtil.e(TAG, "date: " + dateFormat.format(date));
      Druid_until_1.getConnection();
      Dao1 dao = new Dao1();
      Map<Integer, Map<String, String>> mp = null;
      if (!num.equals("")) {
//        mp = dao.getnealytime("2021-12-12", "123");
        mp = dao.getnealytime(dateFormat.format(date), num);
      }
      if (mp != null && !mp.isEmpty()) {
        list.clear();
        for (int i = 0; i < 7; i ++) {
          AfterLoginData up = new AfterLoginData();
          AfterLoginData work = new AfterLoginData();
          Map<String, String> mtmp;
          LogUtil.e(TAG, "test");
          mtmp = mp.get(i);
          if (mtmp == null || mtmp.isEmpty()) continue;
          LogUtil.e(TAG, "no null");
          String upTime = mtmp.get("up");
          if (!upTime.equals("")) {
            up.setData(upTime);
            up.setStatus("up");
            LogUtil.e(TAG, up.getStatus());
            LogUtil.e(TAG, up.getDate());
            list.add(up);
          }
          String workTime = mtmp.get("work");
          if (workTime != null && !workTime.equals("")) {
            work.setData(upTime);
            work.setStatus("work");
            LogUtil.e(TAG, work.getStatus());
            LogUtil.e(TAG, work.getDate());
            list.add(work);
          }
        }
        getActivity().runOnUiThread(() -> {
          adapter.notifyDataSetChanged();
          Toast.makeText(getContext(), "Refresh success", Toast.LENGTH_SHORT).show();
        });
      }
      getActivity().runOnUiThread(() -> {
        afterLoginSwipeRecord.setRefreshing(false);
      });
    }).start();
  }

  private void initAfterLoginRecyclerView(View view) {
//    View view = LayoutInflater.from(this).inflate(R.layout.activity_after_login, null);
    adapter = new AfterLoginAdapter(list);
    LinearLayoutManager manager = new LinearLayoutManager(getContext());
    RecyclerView recyclerView = (RecyclerView) view.findViewById(R.id.admit_after_login_recyclerview);
    recyclerView.setAdapter(adapter);
    recyclerView.setLayoutManager(manager);
  }

//  private void initAllControl() {
//    afterLoginButtonRecord = (Button) findViewById(R.id.after_login_button_record);
//    afterLoginButtonPerson = (Button) findViewById(R.id.after_login_button_person);
//  }
//
//  private void initAllButton() {
//    afterLoginButtonRecord.setOnClickListener(this);
//    afterLoginButtonPerson.setOnClickListener(this);
//  }

//  @Override
//  public void onClick(View v) {
//
//    switch (v.getId()) {
//      case R.id.admit_after_login_button:
//        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
//        String number = prefs.getString("loginPhone", null);
//        if (number != null) {
//          new Thread(() -> {
//            String httpCode = HttpUtil.sendAdmitFaceData(number, MyApplication.getServletPhoneFaceUrl());
//            runOnUiThread(() -> {
//              if (httpCode.equals("200")) {
//                Intent intent = new Intent(AfterLogin.this, AdmitFace.class);
//                startActivity(intent);
//              }
//              Toast.makeText(this, httpCode, Toast.LENGTH_SHORT).show();
//            });
//          }).start();
//        } else {
//          Toast.makeText(this, "unknown error !", Toast.LENGTH_SHORT).show();
//        }
//
//        break;
//    }
//
//  }


//  @Override
//  public void onClick(View v) {
//    if (v.getId() == R.id.after_login_button_person) {
//      Intent
//    }
//  }

  public class AfterLoginAdapter extends RecyclerView.Adapter<AfterLoginAdapter.ViewHolder> {

    List<AfterLoginData> list = new ArrayList<>();

    public AfterLoginAdapter(List<AfterLoginData> list) {
      this.list = list;
      LogUtil.e("AfterLoginAdapter", "this.list.size() = " + this.list.size());
    }

    @NonNull
    @Override
    public AfterLoginAdapter.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
      LogUtil.e("AfterLoginAdapter", "on Create View Holder");
      View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.admit_after_login_recyclerview_item, parent, false);
      ViewHolder viewHolder = new ViewHolder(view);
      return viewHolder;
    }

    @Override
    public void onBindViewHolder(@NonNull AfterLoginAdapter.ViewHolder holder, int position) {
//      if (!this.list.isEmpty()) {
        LogUtil.e("AfterLoginAdapter", "on Bind View Holder");
        String status = this.list.get(holder.getAdapterPosition()).getStatus();
        String date = this.list.get(holder.getAdapterPosition()).getDate();
        holder.admitAfterLoginItemStatusTextView.setText("status: " + status);
        holder.admitAfterLoginItemDateTextView.setText("Date: " + date);
//      }
    }

    @Override
    public int getItemCount() {
      LogUtil.e("AfterLoginAdapter", "list.size = " + this.list.size());
      return this.list.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {

      private TextView admitAfterLoginItemStatusTextView;
      private TextView admitAfterLoginItemDateTextView;

      public ViewHolder(@NonNull View itemView) {
        super(itemView);
        admitAfterLoginItemStatusTextView = (TextView) itemView.findViewById(R.id.admit_after_login_item_status_textview);
        admitAfterLoginItemDateTextView = (TextView) itemView.findViewById(R.id.admit_after_login_item_date_textview);
      }
    }

  }
}