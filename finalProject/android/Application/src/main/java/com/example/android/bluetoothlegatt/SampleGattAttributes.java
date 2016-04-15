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

import java.util.HashMap;

/**
 * This class includes a small subset of standard GATT attributes for demonstration purposes.
 */
public class SampleGattAttributes {
    private static HashMap<String, String> attributes = new HashMap();
    public static String ACC_VALUE =    "00000000-0000-0000-0000-000000000002";
    public static String ROLL_VALUE =   "00000000-0000-0000-0000-000000000004";
    public static String PITCH_VALUE =  "00000000-0000-0000-0000-000000000005";
    public static String TEMP_VALUE =   "00000000-0000-0000-0000-000000000007";
    public static String TAP_VALUE =    "00000000-0000-0000-0000-000000000009";
    public static String DIR_VALUE =    "00000000-0000-0000-0000-000000000011";
    public static String ON_VALUE =     "00000000-0000-0000-0000-000000000012";
    public static String ACC_SERV =     "00000000-0000-0000-0000-000000000003";
    public static String TEMP_SERV =    "00000000-0000-0000-0000-000000000006";
    public static String LED_SERV =     "00000000-0000-0000-0000-000000000010";
    public static String BRI_VALUE =    "00000000-0000-0000-0000-000000000013";
    public static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";
    public static String TAP_SERV =      "00000000-0000-0000-0000-000000000008";

    static {

        // acc
        attributes.put("00000000-0000-0000-0000-000000000001", "Acc Services Test");
        attributes.put(ACC_VALUE, "Acc Values");

        // ACCELERPMETER
        attributes.put("00000000-0000-0000-0000-000000000003", "Accelerometer Services");
        attributes.put(ROLL_VALUE, "Roll Values");
        attributes.put(PITCH_VALUE, "Pitch Values");

        // TEMPERATURE
        attributes.put("00000000-0000-0000-0000-000000000006", "Temperature Service");
        attributes.put(TEMP_VALUE, "Temperature Value");

        // TAP
        attributes.put("00000000-0000-0000-0000-000000000008", "TapTap Service");
        attributes.put(TAP_VALUE, "TapTap Value");

        // LED
        attributes.put("00000000-0000-0000-0000-000000000010", "LED Services");
        attributes.put(DIR_VALUE, "Direction Values");
        attributes.put(ON_VALUE, "On or Off Values");
        attributes.put(BRI_VALUE, "Brightness Values");
    }

    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        return name == null ? defaultName : name;
    }
}
