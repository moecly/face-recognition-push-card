package com.android.renlianshibie1.data;


import com.android.renlianshibie1.util.LogUtil;

import javax.sql.DataSource;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;

public class Druid_until_1 {
//    private static DataSource ds;
//    static {
//
//        try {
//            Properties pro = new Properties();
//            pro.load(Druid_until_1.class.getClassLoader().getResourceAsStream("druid1.properties"));
//
//            ds = DruidDataSourceFactory.createDataSource(pro);
//        }
//             catch (Exception e) {
//                e.printStackTrace();
//
//        }
//
//    }
//    public static Connection getConnection() throws SQLException {
//          Properties pps = System.getProperties();
//
//// pps.list(System.out);
//
//        String path= Druid_until_1.class.getClassLoader().getResource("Druid1.properties").getPath();
//
//        try {
//
//            FileInputStream in=new FileInputStream(path);
//
//            pps.load(in);
//
//            ds=DruidDataSourceFactory.createDataSource(pps);
//
//            Connection conn = ds.getConnection();
//            System.out.println(conn);
//            return conn;
//    } catch (FileNotFoundException e) {
//            e.printStackTrace();
//        } catch (IOException e) {
//            e.printStackTrace();
//        } catch (Exception e) {
//            e.printStackTrace();
//        }return  null;
 //   }
public static Connection getConnection(){
   String driver = "com.mysql.jdbc.Driver";// MySql驱动

//    private static String url = "jdbc:mysql://localhost:3306/map_designer_test_db";

 String user = "root";// 用户名

    String password = "root";// 密码




    Connection connection = null;
    try{
        Class.forName(driver);// 动态加载类
        String ip = "119.23.179.64";// 写成本机地址，不能写成localhost，同时手机和电脑连接的网络必须是同一个

        // 尝试建立到给定数据库URL的连接
        connection = DriverManager.getConnection("jdbc:mysql://" + ip + ":3306/" + "information",
                user, password);
        LogUtil.e("time_work", "test0");

    }catch (Exception e){
        e.printStackTrace();
    }

    return connection;
}
        public static void close(Statement stmt,Connection conn){
//        if(stmt!=null) {
//            try {
//                stmt.close();
//            } catch (SQLException e) {
//                e.printStackTrace();
//            }
//            if (conn!=null){
//                try {
//                    conn.close();
//                } catch (SQLException e) {
//                    e.printStackTrace();
//                }
//            }
//        }
        close(null,stmt,conn);
    }
    public static void close(ResultSet rs, Statement stmt, Connection conn){
        if(rs!=null) {
            try {
                rs.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        if(stmt!=null) {
            try {
                stmt.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            if (conn!=null){
                try {
                    System.out.println(conn);
                    conn.close();
                    System.out.println("conn close");
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}

}
