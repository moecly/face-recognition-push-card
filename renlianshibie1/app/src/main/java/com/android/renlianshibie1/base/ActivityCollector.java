package com.android.renlianshibie1.base;

import android.app.Activity;

import java.util.ArrayList;
import java.util.List;

public class ActivityCollector {

  /*  Create a activity list  */
  private static List<Activity> mActivities = new ArrayList<>();

  /*  Add activity  */
  public static void addActivity(Activity activity) {
    mActivities.add(activity);
  }

  /*  Remove activity  */
  public static void removeActivity(Activity activity) {
    mActivities.remove(activity);
  }

  /*  Finish all activity  */
  public static void finishALL() {

    for (Activity activity : mActivities) {
      if (!activity.isFinishing()) {
        activity.finish();
      }
    }
    mActivities.clear();

  }

}
