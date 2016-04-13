package com.example.android.bluetoothlegatt;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;


public class temperature extends AppCompatActivity {

    int[] timeTemp = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    int[] pointsTemp = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    GraphView graph_temp;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.temperature);

        graph_temp = (GraphView) findViewById(R.id.graph_temp);

        graph_temp.removeAllSeries();
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

    private void updatePointsTemp(int newPoint) {
        
        //shift the data
        int i;
        for(i = 0; i< 19; i++) {
            pointsTemp[i] = pointsTemp[i+1];
        }
        pointsTemp[19] = newPoint;

        //increment the time
        for(i = 0; i< 20; i++){
            timeTemp[i] += 1;
        }

        LineGraphSeries<DataPoint> series_temp = new LineGraphSeries<DataPoint>(new DataPoint[] {
                new DataPoint(timeTemp[0], pointsTemp[0]),
                new DataPoint(timeTemp[1], pointsTemp[1]),
                new DataPoint(timeTemp[2], pointsTemp[2]),
                new DataPoint(timeTemp[3], pointsTemp[3]),
                new DataPoint(timeTemp[4], pointsTemp[4]),
                new DataPoint(timeTemp[5], pointsTemp[5]),
                new DataPoint(timeTemp[6], pointsTemp[6]),
                new DataPoint(timeTemp[7], pointsTemp[7]),
                new DataPoint(timeTemp[8], pointsTemp[8]),
                new DataPoint(timeTemp[9], pointsTemp[9]),
                new DataPoint(timeTemp[10], pointsTemp[10]),
                new DataPoint(timeTemp[11], pointsTemp[11]),
                new DataPoint(timeTemp[12], pointsTemp[12]),
                new DataPoint(timeTemp[13], pointsTemp[13]),
                new DataPoint(timeTemp[14], pointsTemp[14]),
                new DataPoint(timeTemp[15], pointsTemp[15]),
                new DataPoint(timeTemp[16], pointsTemp[16]),
                new DataPoint(timeTemp[17], pointsTemp[17]),
                new DataPoint(timeTemp[18], pointsTemp[18]),
                new DataPoint(timeTemp[19], pointsTemp[19])
        });
        graph_temp.removeAllSeries();
        graph_temp.addSeries(series_temp);


    }


}
