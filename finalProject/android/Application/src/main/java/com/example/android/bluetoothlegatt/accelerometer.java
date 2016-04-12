package com.example.android.bluetoothlegatt;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;


public class accelerometer extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.accelerometer);

        GraphView graph_roll = (GraphView) findViewById(R.id.graph_roll);
        LineGraphSeries<DataPoint> series_roll = new LineGraphSeries<DataPoint>(new DataPoint[] {
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
        graph_roll.removeAllSeries();
        graph_roll.addSeries(series_roll);
        graph_roll.setTitle("Roll Angle Over Time");

        GraphView graph_pitch = (GraphView) findViewById(R.id.graph_pitch);
        LineGraphSeries<DataPoint> series_pitch = new LineGraphSeries<DataPoint>(new DataPoint[] {
                new DataPoint(0, 4),
                new DataPoint(1, 3),
                new DataPoint(2, 7),
                new DataPoint(3, 5),
                new DataPoint(4, 3),
                new DataPoint(5, 8),
                new DataPoint(6, 2),
                new DataPoint(7, 4),
                new DataPoint(8, 3),
                new DataPoint(9, 2)
        });
        graph_pitch.removeAllSeries();
        graph_pitch.addSeries(series_pitch);
        graph_pitch.setTitle("Pitch Angle Over Time");








    }

    @Override
    public void onBackPressed() {
        Intent intentMain = new Intent(accelerometer.this,
                DeviceControlActivity.class);
        accelerometer.this.startActivity(intentMain);
        finish();
    }

    private void refreshData() {

    }


}
