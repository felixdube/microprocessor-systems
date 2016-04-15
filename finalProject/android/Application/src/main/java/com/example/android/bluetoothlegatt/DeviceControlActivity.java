/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.bluetoothlegatt;

import android.app.Activity;
import android.app.NotificationManager;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.NotificationCompat;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.SeekBar;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */


//TODO create an object ScheduledExecutorService
    //TODO create a function that call mBluetoothLeService.readCharacteristic(characteristic) on all the characteristic that are read
    // using    for (BluetoothGattService gattService : gattServices) {
        //TODO update value depending on the characteristic in  broadcastUpdate()
            //TODO create function that updates each specific graph

//TODO create a listener on the slider to calls mBluetoothLeService.writeCharacteristic(characteristic, data);


public class DeviceControlActivity extends Activity implements View.OnClickListener {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    private TextView mConnectionState;
    private TextView mDataField;
    private String mDeviceName;
    private String mDeviceAddress;
    private ExpandableListView mGattServicesList;
    private BluetoothLeService mBluetoothLeService;
    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
    private boolean mConnected = false;
    private BluetoothGattCharacteristic mNotifyCharacteristic;

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";

    static float[] timeTemp = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    static float[] pointsTemp = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    GraphView graph_temp;
    public static LineGraphSeries<DataPoint> series_temp;

    static float[] timePitch = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    static float[] pointsPitch = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    GraphView graph_pitch;
    public static LineGraphSeries<DataPoint> series_pitch;

    static float[] timeRoll = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    static float[] pointsRoll = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    GraphView graph_roll;
    public static LineGraphSeries<DataPoint> series_roll;


    boolean up1 = true;
    boolean up2 = true;
    boolean up3 = true;
    public int dummy1;
    public int dummy2;
    public int dummy3;
    int selector = 1;
    int update = 0;
    boolean on = false;

    private SeekBar brightness;
    private SeekBar speed;
    private Button onOff;

    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
                updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                clearUI();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
                //displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };

    // If a given GATT characteristic is selected, check for supported features.  This sample
    // demonstrates 'Read' and 'Notify' features.  See
    // http://d.android.com/reference/android/bluetooth/BluetoothGatt.html for the complete
    // list of supported characteristic features.
    private final ExpandableListView.OnChildClickListener servicesListClickListner =
            new ExpandableListView.OnChildClickListener() {
                @Override
                public boolean onChildClick(ExpandableListView parent, View v, int groupPosition,
                                            int childPosition, long id) {
                    if (mGattCharacteristics != null) {
                        final BluetoothGattCharacteristic characteristic =
                                mGattCharacteristics.get(groupPosition).get(childPosition);
                        final int charaProp = characteristic.getProperties();
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_READ) > 0) {
                            // If there is an active notification on a characteristic, clear
                            // it first so it doesn't update the data field on the user interface.
                            if (mNotifyCharacteristic != null) {
                                mBluetoothLeService.setCharacteristicNotification(
                                        mNotifyCharacteristic, false);
                                mNotifyCharacteristic = null;
                            }
                            mBluetoothLeService.readCharacteristic(characteristic);
                        }
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0) {
                            mNotifyCharacteristic = characteristic;
                            mBluetoothLeService.setCharacteristicNotification(
                                    characteristic, true);
                        }
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_WRITE) > 0) {
                            byte[] data = {1};
                            mBluetoothLeService.writeCharacteristic(characteristic, data);
                        }
                        return true;
                    }
                    return false;
                }
    };

    private void clearUI() {
        //mGattServicesList.setAdapter((SimpleExpandableListAdapter) null);
        //mDataField.setText(R.string.no_data);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.gatt_connected);


        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // Sets up UI references.
        ((TextView) findViewById(R.id.device_address)).setText(mDeviceAddress);
        //mGattServicesList = (ExpandableListView) findViewById(R.id.gatt_services_list);
        //mGattServicesList.setOnChildClickListener(servicesListClickListner);
        mConnectionState = (TextView) findViewById(R.id.connection_state);
        mDataField = (TextView) findViewById(R.id.data_value);

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);

//        List<BluetoothGattService> gattServices = mBluetoothLeService.getSupportedGattServices();
//
//        for (BluetoothGattService gattService : gattServices) {
//            for (final BluetoothGattCharacteristic gattCharacteristic : gattService.getCharacteristics()) {
//                if (BluetoothLeService.UUID_TAP_VALUE.equals(gattCharacteristic.getUuid())) {
//                        //TODO set notify
//                }
//                if (BluetoothLeService.UUID_DIR_VALUE.equals(gattCharacteristic.getUuid())) {
//
//                }
//            }
//
//        }



        graph_temp = (GraphView) findViewById(R.id.graph_temp);

        graph_temp.removeAllSeries();
        graph_temp.setTitle("Temperature Over Time");
        graph_temp.getViewport().setYAxisBoundsManual(true);
        graph_temp.getViewport().setMinY(25);
        graph_temp.getViewport().setMaxY(65);


        graph_roll = (GraphView) findViewById(R.id.graph_roll);
        graph_roll.removeAllSeries();
        graph_roll.setTitle("Roll Angle Over Time");
        graph_roll.getViewport().setYAxisBoundsManual(true);
        graph_roll.getViewport().setMinY(0);
        graph_roll.getViewport().setMaxY(180);


        graph_pitch = (GraphView) findViewById(R.id.graph_pitch);
        graph_pitch.removeAllSeries();
        graph_pitch.setTitle("Pitch Angle Over Time");
        graph_pitch.getViewport().setYAxisBoundsManual(true);
        graph_pitch.getViewport().setMinY(0);
        graph_pitch.getViewport().setMaxY(180);

        this.brightness = (SeekBar) findViewById(R.id.brightness_seekBar);
        brightness.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            int progress = 0;

            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                updateBrightness(progresValue);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });

        this.speed = (SeekBar) findViewById(R.id.speed_seekBar);
        speed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            int progress = 0;

            @Override
            public void onProgressChanged(SeekBar seekBar, int progresValue, boolean fromUser) {
                updateSpeed(progresValue);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                //updateBrightness(seekBar.getProgress());
            }

        });

        ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(10);

        Runnable ReadTask = new ReadRollCharacteristicTask();


        scheduledExecutorService.scheduleAtFixedRate(ReadTask,
                5000,
                20,
                TimeUnit.MILLISECONDS);

        Runnable ReadPitchTask = new ReadPitchCharacteristicTask();


        scheduledExecutorService.scheduleAtFixedRate(ReadPitchTask,
                5005,
                20,
                TimeUnit.MILLISECONDS);

        Runnable ReadTempTask = new ReadTempCharacteristicTask();


        scheduledExecutorService.scheduleAtFixedRate(ReadTempTask,
                5010,
                20,
                TimeUnit.MILLISECONDS);
    }

    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBluetoothLeService = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {

        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mConnectionState.setText(resourceId);
            }
        });
    }

    private void displayData(String data) {
        if (data != null) {
            mDataField.setText(data);
        }
    }



    // Demonstrates how to iterate through the supported GATT Services/Characteristics.
    // In this sample, we populate the data structure that is bound to the ExpandableListView
    // on the UI.
//    private void displayGattServices(List<BluetoothGattService> gattServices) {
//        if (gattServices == null) return;
//        String uuid = null;
//        String unknownServiceString = getResources().getString(R.string.unknown_service);
//        String unknownCharaString = getResources().getString(R.string.unknown_characteristic);
//        ArrayList<HashMap<String, String>> gattServiceData = new ArrayList<HashMap<String, String>>();
//        ArrayList<ArrayList<HashMap<String, String>>> gattCharacteristicData
//                = new ArrayList<ArrayList<HashMap<String, String>>>();
//        mGattCharacteristics = new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
//
//        // Loops through available GATT Services.
//        for (BluetoothGattService gattService : gattServices) {
//            HashMap<String, String> currentServiceData = new HashMap<String, String>();
//            uuid = gattService.getUuid().toString();
//            currentServiceData.put(
//                    LIST_NAME, SampleGattAttributes.lookup(uuid, unknownServiceString));
//            currentServiceData.put(LIST_UUID, uuid);
//            gattServiceData.add(currentServiceData);
//
//            ArrayList<HashMap<String, String>> gattCharacteristicGroupData =
//                    new ArrayList<HashMap<String, String>>();
//            List<BluetoothGattCharacteristic> gattCharacteristics =
//                    gattService.getCharacteristics();
//            ArrayList<BluetoothGattCharacteristic> charas =
//                    new ArrayList<BluetoothGattCharacteristic>();
//
//            // Loops through available Characteristics.
//            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
//                charas.add(gattCharacteristic);
//                HashMap<String, String> currentCharaData = new HashMap<String, String>();
//                uuid = gattCharacteristic.getUuid().toString();
//                currentCharaData.put(
//                        LIST_NAME, SampleGattAttributes.lookup(uuid, unknownCharaString));
//                currentCharaData.put(LIST_UUID, uuid);
//                gattCharacteristicGroupData.add(currentCharaData);
//            }
//            mGattCharacteristics.add(charas);
//            gattCharacteristicData.add(gattCharacteristicGroupData);
//        }
//
//        SimpleExpandableListAdapter gattServiceAdapter = new SimpleExpandableListAdapter(
//                this,
//                gattServiceData,
//                android.R.layout.simple_expandable_list_item_2,
//                new String[] {LIST_NAME, LIST_UUID},
//                new int[] { android.R.id.text1, android.R.id.text2 },
//                gattCharacteristicData,
//                android.R.layout.simple_expandable_list_item_2,
//                new String[] {LIST_NAME, LIST_UUID},
//                new int[] { android.R.id.text1, android.R.id.text2 }
//        );
//        mGattServicesList.setAdapter(gattServiceAdapter);
//    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }


    private final class ReadRollCharacteristicTask implements Runnable {
        @Override public void run() {

            List<BluetoothGattService> gattServices = mBluetoothLeService.getSupportedGattServices();

            for (BluetoothGattService gattService : gattServices) {
                if(BluetoothLeService.UUID_ACC_SERV.equals(gattService.getUuid())) {
                    for (final BluetoothGattCharacteristic gattCharacteristic : gattService.getCharacteristics()) {
                        if (BluetoothLeService.UUID_ROLL_VALUE.equals(gattCharacteristic.getUuid())) {
                            mBluetoothLeService.readCharacteristic(gattCharacteristic);
                        }
                    }
                }
            }

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (series_roll != null) {
                        graph_roll.removeAllSeries();
                        graph_roll.addSeries(series_roll);
                    }
                }
            });
        }
    }

    private final class ReadPitchCharacteristicTask implements Runnable {
        @Override public void run() {

            List<BluetoothGattService> gattServices = mBluetoothLeService.getSupportedGattServices();

            for (BluetoothGattService gattService : gattServices) {
                if(BluetoothLeService.UUID_ACC_SERV.equals(gattService.getUuid())) {
                    for (final BluetoothGattCharacteristic gattCharacteristic : gattService.getCharacteristics()) {
                        if (BluetoothLeService.UUID_PITCH_VALUE.equals(gattCharacteristic.getUuid())) {
                            mBluetoothLeService.readCharacteristic(gattCharacteristic);
                        }
                    }
                }
            }

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (series_pitch != null) {
                        graph_pitch.removeAllSeries();
                        graph_pitch.addSeries(series_pitch);
                    }
                }
            });
        }
    }

    private final class ReadTempCharacteristicTask implements Runnable {
        @Override public void run() {

            List<BluetoothGattService> gattServices = mBluetoothLeService.getSupportedGattServices();

            for (BluetoothGattService gattService : gattServices) {
                if(BluetoothLeService.UUID_TEMP_SERV.equals(gattService.getUuid())) {
                    for (final BluetoothGattCharacteristic gattCharacteristic : gattService.getCharacteristics()) {
                        if (BluetoothLeService.UUID_TEMP_VALUE.equals(gattCharacteristic.getUuid())) {
                            mBluetoothLeService.readCharacteristic(gattCharacteristic);
                        }
                    }
                }
            }

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (series_temp != null) {
                        graph_temp.removeAllSeries();
                        graph_temp.addSeries(series_temp);
                    }
                }
            });
        }
    }



    public static void updatePointsRoll(float newPoint) {
        //System.out.println("ROLL:  "+newPoint);

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

        series_roll = new LineGraphSeries<DataPoint>(new DataPoint[] {
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
    }

    public static void updatePointsPitch(float newPoint) {
        //System.out.println("PITCH:  "+newPoint);

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

        series_pitch = new LineGraphSeries<DataPoint>(new DataPoint[] {
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

    }

    public static void updatePointsTemp(float newPoint) {
        //System.out.println("TEMP:  "+newPoint);
        int i;
        for(i = 0; i< 19; i++) {
            pointsTemp[i] = pointsTemp[i+1];
        }
        pointsTemp[19] = newPoint;

        //increment the time
        for(i = 0; i< 20; i++){
            timeTemp[i] += 1;
        }

        series_temp = new LineGraphSeries<DataPoint>(new DataPoint[] {
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
    }

    public void onOff(View v){

        this.onOff = (Button) findViewById(R.id.onOff_button);
        byte[] data = {2};

        if(on){
            data[0] = 0;
            on = false;
            onOff.setText("OFF");
        }
        else{
            data[0] = 1;
            on = true;
            onOff.setText("ON");
        }

        List<BluetoothGattService> gattServices = mBluetoothLeService.getSupportedGattServices();

        for (BluetoothGattService gattService : gattServices) {
            if(BluetoothLeService.UUID_LED_SERV.equals(gattService.getUuid())) {
                for (final BluetoothGattCharacteristic gattCharacteristic : gattService.getCharacteristics()) {
                    if (BluetoothLeService.UUID_ON_VALUE.equals(gattCharacteristic.getUuid())) {
                        mBluetoothLeService.writeCharacteristic(gattCharacteristic, data);
                    }
                }
            }
        }
    }

    public void updateBrightness(int value) {

        System.out.println("new brightness");

        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();

        List<BluetoothGattService> gattServices = mBluetoothLeService.getSupportedGattServices();

        for (BluetoothGattService gattService : gattServices) {
            if(BluetoothLeService.UUID_LED_SERV.equals(gattService.getUuid())) {
                for (final BluetoothGattCharacteristic gattCharacteristic : gattService.getCharacteristics()) {
                    if (BluetoothLeService.UUID_BRI_VALUE.equals(gattCharacteristic.getUuid())) {
                        mBluetoothLeService.writeCharacteristic(gattCharacteristic, bytes);
                    }
                }
            }
        }
    }

    public void updateSpeed(int value) {

        System.out.println("new speed");

        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();

        List<BluetoothGattService> gattServices = mBluetoothLeService.getSupportedGattServices();

        for (BluetoothGattService gattService : gattServices) {
            if(BluetoothLeService.UUID_LED_SERV.equals(gattService.getUuid())) {
                for (final BluetoothGattCharacteristic gattCharacteristic : gattService.getCharacteristics()) {
                    if (BluetoothLeService.UUID_DIR_VALUE.equals(gattCharacteristic.getUuid())) {
                        mBluetoothLeService.writeCharacteristic(gattCharacteristic, bytes);
                    }
                }
            }
        }
    }

    public void notifyTap(View V) {
        android.support.v4.app.NotificationCompat.Builder mBuilder =
                new NotificationCompat.Builder(this)
                        .setSmallIcon(R.drawable.tile)
                        .setContentTitle("My notification")
                        .setContentText("Hello World!");

        // Sets an ID for the notification
        int mNotificationId = 001;
        // Gets an instance of the NotificationManager service
        NotificationManager mNotifyMgr =
                (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        // Builds the notification and issues it.
        mNotifyMgr.notify(mNotificationId, mBuilder.build());
    }

}
