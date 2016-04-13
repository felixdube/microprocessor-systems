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

    int[] timePitch = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    int[] pointsPitch = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    GraphView graph_pitch;

    int[] timeRoll = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    int[] pointsRoll = {0,0,0,0,0,0,0,0,0,0};
    GraphView graph_roll;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.accelerometer);

        graph_pitch = (GraphView) findViewById(R.id.graph_pitch);
        graph_roll = (GraphView) findViewById(R.id.graph_roll);

        graph_roll.removeAllSeries();
        graph_roll.setTitle("Roll Angle Over Time");

        graph_pitch.removeAllSeries();
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

    private void updatePointsPitch(int newPoint) {


        //shift the data
        int i;
        for(i = 0; i< 19; i++) {
            pointsPitch[i] = pointsPitch[i+1];
        }
        pointsPitch[19] = newPoint;

        //increment the time
        for(i = 0; i< 20; i++){
            timePitch[i] += 1;
        }

        LineGraphSeries<DataPoint> series_pitch = new LineGraphSeries<DataPoint>(new DataPoint[] {
                new DataPoint(timePitch[0], pointsPitch[0]),
                new DataPoint(timePitch[1], pointsPitch[1]),
                new DataPoint(timePitch[2], pointsPitch[2]),
                new DataPoint(timePitch[3], pointsPitch[3]),
                new DataPoint(timePitch[4], pointsPitch[4]),
                new DataPoint(timePitch[5], pointsPitch[5]),
                new DataPoint(timePitch[6], pointsPitch[6]),
                new DataPoint(timePitch[7], pointsPitch[7]),
                new DataPoint(timePitch[8], pointsPitch[8]),
                new DataPoint(timePitch[9], pointsPitch[9]),
                new DataPoint(timePitch[10], pointsPitch[10]),
                new DataPoint(timePitch[11], pointsPitch[11]),
                new DataPoint(timePitch[12], pointsPitch[12]),
                new DataPoint(timePitch[13], pointsPitch[13]),
                new DataPoint(timePitch[14], pointsPitch[14]),
                new DataPoint(timePitch[15], pointsPitch[15]),
                new DataPoint(timePitch[16], pointsPitch[16]),
                new DataPoint(timePitch[17], pointsPitch[17]),
                new DataPoint(timePitch[18], pointsPitch[18]),
                new DataPoint(timePitch[19], pointsPitch[19])
        });
        graph_pitch.removeAllSeries();
        graph_pitch.addSeries(series_pitch);


    }

    private void updatePointsRoll(int newPoint) {


        //shift the data
        int i;
        for(i = 0; i< 19; i++) {
            pointsRoll[i] = pointsRoll[i+1];
        }
        pointsRoll[19] = newPoint;

        //increment the time
        for(i = 0; i< 20; i++){
            timeRoll[i] += 1;
        }

        LineGraphSeries<DataPoint> series_roll = new LineGraphSeries<DataPoint>(new DataPoint[] {
                new DataPoint(timeRoll[0], pointsRoll[0]),
                new DataPoint(timeRoll[1], pointsRoll[1]),
                new DataPoint(timeRoll[2], pointsRoll[2]),
                new DataPoint(timeRoll[3], pointsRoll[3]),
                new DataPoint(timeRoll[4], pointsRoll[4]),
                new DataPoint(timeRoll[5], pointsRoll[5]),
                new DataPoint(timeRoll[6], pointsRoll[6]),
                new DataPoint(timeRoll[7], pointsRoll[7]),
                new DataPoint(timeRoll[8], pointsRoll[8]),
                new DataPoint(timeRoll[9], pointsRoll[9]),
                new DataPoint(timeRoll[10], pointsRoll[10]),
                new DataPoint(timeRoll[11], pointsRoll[11]),
                new DataPoint(timeRoll[12], pointsRoll[12]),
                new DataPoint(timeRoll[13], pointsRoll[13]),
                new DataPoint(timeRoll[14], pointsRoll[14]),
                new DataPoint(timeRoll[15], pointsRoll[15]),
                new DataPoint(timeRoll[16], pointsRoll[16]),
                new DataPoint(timeRoll[17], pointsRoll[17]),
                new DataPoint(timeRoll[18], pointsRoll[18]),
                new DataPoint(timeRoll[19], pointsRoll[19])
        });
        graph_roll.removeAllSeries();
        graph_roll.addSeries(series_roll);

    }

}
