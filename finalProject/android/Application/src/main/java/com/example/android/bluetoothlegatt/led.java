package com.example.android.bluetoothlegatt;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;


public class led extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.led);




    }

    @Override
    public void onBackPressed() {
        Intent intentMain = new Intent(led.this,
                DeviceControlActivity.class);
        led.this.startActivity(intentMain);
        finish();
    }

    private void refreshData() {

    }


}
