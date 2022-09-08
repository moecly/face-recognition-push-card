package com.android.renlianshibie1.data;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class Dao1 {
    public String getWorktime(String date,String num){
        Connection conn=null;
        PreparedStatement pr=null;
        ResultSet rs=null;
        String sql="select time_work from `"+date+"` where num =?";
        try {
            conn= Druid_until_1.getConnection();
            pr = conn.prepareStatement(sql);
            //pr.setString(1,date);
            pr.setString(1,num);
            rs = pr.executeQuery();
            if (rs.next()){
                String time = rs.getString("time_work");
                if (time!=null)
                    return time;
                if (time==null){
                    return null;
                }
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }finally {
            Druid_until_1.close(rs,pr,conn);
        }return "错误";


    }
    public String getuptime(String date,String num){
        Connection conn=null;
        PreparedStatement pr=null;
        ResultSet rs=null;
        String sql="select time_up from `"+date+"` where num =?";
        try {
            conn= Druid_until_1.getConnection();
            pr = conn.prepareStatement(sql);
            //pr.setString(1,date);
            pr.setString(1,num);
            rs = pr.executeQuery();
            if (rs.next()){
                String time = rs.getString("time_up");
                if (time!=null)
                    return time;
                if (time==null){
                    return null;
                }
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }finally {
            Druid_until_1.close(rs,pr,conn);
        }return "错误";


    }
    public Map<Integer,Map<String,String>> getnealytime(String date,String num){
        Map<Integer,Map<String,String>> map1=new HashMap<>();
        Connection conn=null;
        PreparedStatement pr=null;
        ResultSet rs=null;
        for (int i=0;i<8;i++){
            Map<String,String> map=new HashMap<>();
           String time=subDay(date,i);
            String sql="select * from `"+time+"` where num =?";
            try {
                conn= Druid_until_1.getConnection();
                pr = conn.prepareStatement(sql);
                //pr.setString(1,date);
                pr.setString(1,num);
                rs = pr.executeQuery();
                if (rs.next()){
                    String up = rs.getString("time_up");
                    System.out.println("up===="+up);
                    String work=rs.getString("time_work");
                    System.out.println(work);
                    map.put("up",up);
                    map.put("work",work);
                    map1.put(i,map);
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }finally {
                Druid_until_1.close(rs,pr,conn);
            }
        }
        Druid_until_1.close(rs,pr,conn);


        //map

        return map1;
    }

    private String subDay(String date,int i)  {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Date dt = null;
        try {
            dt = sdf.parse(date);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        Calendar rightNow = Calendar.getInstance();
        rightNow.setTime(dt);
        rightNow.add(Calendar.DAY_OF_MONTH, -i);
        Date dt1 = rightNow.getTime();
        String reStr = sdf.format(dt1);
        return reStr;
    }
}
