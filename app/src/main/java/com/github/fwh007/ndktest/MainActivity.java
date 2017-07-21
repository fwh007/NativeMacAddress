package com.github.fwh007.ndktest;

import android.content.Context;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });

        // Example of a call to a native method
        ((TextView) findViewById(R.id.sample_text1))
                .setText(getMacAddress1());

        ((TextView) findViewById(R.id.sample_text2))
                .setText(getMacAddress2());

        ((TextView) findViewById(R.id.sample_text3))
                .setText(getMacAddress3());

        //这个会崩溃
//        ((TextView) findViewById(R.id.sample_text4))
//                .setText(getMacAddress4());

        ((TextView) findViewById(R.id.sample_text5))
                .setText(getMacAddress5());

        ((TextView) findViewById(R.id.sample_text6))
                .setText(getMacAddress6());

        ((TextView) findViewById(R.id.sample_text7))
                .setText(getDeviceId());

        ((TextView) findViewById(R.id.sample_text8))
                .setText(getAndroidId(this));
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    public native String getMacAddress1();

    public native String getMacAddress2();

    public native String getMacAddress3();

    public native String getMacAddress4();

    public native String getMacAddress5();

    public native String getMacAddress6();

    public native String getDeviceId();

    public native String getAndroidId(Context context);

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
