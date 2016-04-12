package com.example.android.bluetoothlegatt;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;


public class temperature extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.temperature);

        GraphView graph_temp = (GraphView) findViewById(R.id.graph_temp);
        LineGraphSeries<DataPoint> series_temp = new LineGraphSeries<DataPoint>(new DataPoint[] {
                new DataPoint(0, 1),
                new DataPoint(1, 5),
                new DataPoint(2, 3),
                new DataPoint(3, 2),
                new DataPoint(4, 6),
                new DataPoint(5, 1),
                new DataPoint(6, 5),
                new DataPoint(7, 3),
                new DataPoint(8, 2),
                new DataPoint(9, 6)
        });
        graph_temp.removeAllSeries();
        graph_temp.addSeries(series_temp);
        graph_temp.setTitle("Temperature Over Time");




    }

    @Override
    public void onBackPressed() {
        Intent intentMain = new Intent(temperature.this,
                DeviceControlActivity.class);
        temperature.this.startActivity(intentMain);
        finish();
    }

    private void refreshData() {

    }


}
