package com.example.blebandwidth;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ExpandableListView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    enum UserReq {
        START_SCAN_REQ,
        STOP_SCAN_REQ,
        CONNECT_REQ,
        DISCONNECT_REQ,
        UNDEFINED_REQ,
    }

    private BluetoothManager mblueManager;
    private BluetoothAdapter mblueAdapter;
    private BluetoothLeScanner mblueLeScanner;

    private boolean m_isScanning = false;
    private static final String TAG = "bbw_BleMainActivity";
    private String m_device_name;
    private String m_device_address;
    private boolean m_is_device_found = false;
    private BluetoothGatt m_blueGatt;
    private boolean m_is_connected = false;
    private UserReq m_user_req = UserReq.UNDEFINED_REQ;
    private Button mConnectButton;
    private Button mScanButton;
    private EditText mDeviceNameText;
    private TextView mScanResultTextview;
    private TextView mDeviceStatus;
    private Button mSpeedTestButton;
    private TextView mSpeedTextView;
    private ExpandableListView mServiceListExpandView;
    private EditText mPacketCountText;
    private EditText mPacketSizeText;
    private Handler m_mainHandler = new Handler(Looper.getMainLooper());
    private byte mcount = (byte)0;
    private int m_default_packet_count = 100;
    private int m_default_packet_size = 10;
    private String m_default_packet_config = "";
    private String m_packet_config = "";
    private int m_packet_count = 100;
    private int m_packet_size = 10;
    private int m_byte_sent = 0;
    private int m_curr_packet_count = 0;
    private long mStartTime = 0;
    private long mEndTime = 0;
    private String mResult = "";
    String m_svc_uuid = "";
    String m_char_uuid = "";
    private final int MAX_MTU_REQ = 517;
    private int mCurrentMTU = 0;
    private Context m_context;
    private HashMap<String, List<ChildInfo> >  m_ServiceGroupsHash;
    private ServiceListAdapter m_serviceListAdapter;
    private List<String> m_serviceGroupList;
    private final int FIRST_PACKET = 11;
    private final int NEXT_PACKET = 55;
    private final int LAST_PACKET = 99;

    String[] PERMISSIONS = {
            android.Manifest.permission.ACCESS_FINE_LOCATION,
            android.Manifest.permission.BLUETOOTH_SCAN,
            android.Manifest.permission.BLUETOOTH_CONNECT,
    };

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == 1) {
            if(grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                if(ContextCompat.checkSelfPermission(this,
                        android.Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
                        Context context = getApplicationContext();
                        int duration = Toast.LENGTH_SHORT;
                        Toast toast = Toast.makeText(context, "Location Permission Granted", duration);
                        toast.show();
                } else {
                    Context context = getApplicationContext();
                    int duration = Toast.LENGTH_SHORT;
                    Toast toast = Toast.makeText(context, "Location Permission denied", duration);
                    toast.show();
                }

                if(ContextCompat.checkSelfPermission(this,
                        android.Manifest.permission.BLUETOOTH_SCAN) == PackageManager.PERMISSION_GRANTED) {
                        Context context = getApplicationContext();
                        int duration = Toast.LENGTH_SHORT;
                        Toast toast = Toast.makeText(context, "SCAN Permission Granted", duration);
                        toast.show();
                } else {
                    Context context = getApplicationContext();
                    int duration = Toast.LENGTH_SHORT;
                    Toast toast = Toast.makeText(context, "SCAN Permission denied", duration);
                    toast.show();
                }

                if(ContextCompat.checkSelfPermission(this,
                        android.Manifest.permission.BLUETOOTH_CONNECT) == PackageManager.PERMISSION_GRANTED) {
                        Context context = getApplicationContext();
                        int duration = Toast.LENGTH_SHORT;
                        Toast toast = Toast.makeText(context, "CONNECT Permission Granted", duration);
                        toast.show();
                } else {
                    Context context = getApplicationContext();
                    int duration = Toast.LENGTH_SHORT;
                    Toast toast = Toast.makeText(context, "CONNECT Permission denied", duration);
                    toast.show();
                }
            }
        }
    }

    private boolean hasPermissions( String[] permissions) {
         for (String permission: permissions) {
            if (ContextCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                return false;
             }
        }
         return true;
    }

    private void request_permission_all() {
        if (!hasPermissions(PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, PERMISSIONS, 1);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mConnectButton = (Button) findViewById(R.id.connectButton);
        mScanButton = (Button) findViewById(R.id.ScanButton);
        mDeviceNameText = (EditText) findViewById(R.id.deviceName);
        mScanResultTextview = (TextView)findViewById(R.id.scanResult);
        mDeviceStatus = (TextView)findViewById(R.id.deviceStatus);
        mSpeedTestButton = (Button) findViewById(R.id.speedTestButton);
        mSpeedTextView = (TextView) findViewById(R.id.bandWidthResult);
        mServiceListExpandView = (ExpandableListView) findViewById(R.id.expandServiceListView);
        mPacketCountText = (EditText) findViewById(R.id.packetCount);
        mPacketSizeText = (EditText) findViewById(R.id.packetSize);

        request_permission_all();

       m_context = getApplicationContext();
    }

    private void update_device_status(String device_status) {
        mDeviceStatus.setVisibility(View.VISIBLE);
        mDeviceStatus.setText(device_status);
    }

    private void hide_scan_view() {
        mScanButton.setVisibility(View.INVISIBLE); //invisible
        mDeviceNameText.setVisibility(View.INVISIBLE); //invisible
    }

    private void display_scan_view() {
        mScanButton.setVisibility(View.VISIBLE); //invisible
        mDeviceNameText.setVisibility(View.VISIBLE); //visible
        mDeviceStatus.setVisibility(View.INVISIBLE);
    }

    private void hide_connected_view() {
        mScanResultTextview.setVisibility(View.INVISIBLE);
        mConnectButton.setVisibility(View.INVISIBLE);
        mSpeedTestButton.setVisibility(View.INVISIBLE);
        mServiceListExpandView.setVisibility(View.INVISIBLE);
        mSpeedTextView.setVisibility(View.INVISIBLE);
        mPacketCountText.setVisibility(View.INVISIBLE);
        mPacketSizeText.setVisibility(View.INVISIBLE);
    }

    private void hide_speed_test_button()
    {
        m_mainHandler.post(new Runnable() {
            @Override
            public void run() {
                mSpeedTestButton.setVisibility(View.INVISIBLE);
            }
        });
    }

    private void enable_speed_test_button()
    {
        m_mainHandler.post(new Runnable() {
            @Override
            public void run() {
                mSpeedTestButton.setVisibility(View.VISIBLE);
            }
        });
    }

    private void display_scan_result(String scan_result) {
        //Display Scan result
        mScanResultTextview.setVisibility(View.VISIBLE);
        mScanResultTextview.setText(scan_result);

        //show connect button
        mConnectButton.setVisibility(View.VISIBLE); //visible

         //hide scan screen
        hide_scan_view();

        update_device_status("Device Found");
    }

    private ScanCallback mLeScanCallback = new ScanCallback() {
        private static final String TAG = "bbw_onScanResult";
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);
            m_is_device_found = false;

            Log.d(TAG, "onScanResult :" + result.getDevice().getName() + " ~ "  +result.getDevice().getAddress());
            String dev_name = result.getDevice().getName();
            if (m_device_name.equals(dev_name)) {
                m_is_device_found = true;
                m_device_address = result.getDevice().getAddress();
                Log.d(TAG, "onScanResult : device found");
                stop_ble_scan();

                String scan_result_str = m_device_name + "\n" + m_device_address;
                display_scan_result(scan_result_str);
            }
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results) {
            super.onBatchScanResults(results);
            Log.d(TAG, "onBatchScanResults ");
        }

        @Override
        public void onScanFailed(int errorCode) {
            super.onScanFailed(errorCode);
            Log.d(TAG, "onScanFailed errorCode = " + errorCode);
        }
    };

    private void stop_ble_scan() {
        if(m_isScanning) {
            m_isScanning = false;
            mblueLeScanner.stopScan(mLeScanCallback);

            //Change to start scan button
            Button button = (Button) findViewById(R.id.ScanButton);
            button.setText("Start Scan");
        }
    }

    private void start_ble_scan () {
        Log.d(TAG, "start ble scan");

        if (mblueLeScanner == null) {
            Log.d(TAG, "LE Scanner is null");
            return;
        }

        if (!m_isScanning) {
            m_isScanning = true;
            m_user_req = UserReq.START_SCAN_REQ;

            //get the device name from user
            m_device_name = mDeviceNameText.getText().toString();
            if(m_device_name.isEmpty()) {
                update_device_status("Empty Device Name");
                return;
            }

            List<ScanFilter> filter_list = null;
            if(m_device_name != null) {
                filter_list = new ArrayList<>();
                ScanFilter filter = new ScanFilter.Builder()
                        .setDeviceName(m_device_name)
                        .build();

                filter_list.add(filter);
            }

            ScanSettings scan_settings = new ScanSettings.Builder()
                            .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
                            .build();

            mblueLeScanner.startScan(filter_list, scan_settings, mLeScanCallback);


            //Change to stop scan button
            mScanButton.setText("Stop Scan");
        } else {
            m_isScanning = false;
            m_user_req = UserReq.STOP_SCAN_REQ;

            mblueLeScanner.stopScan(mLeScanCallback);

            //Change to start scan button
            mScanButton.setText("Start Scan");
        }
    }

     public void StartBleScanOnClick(View view) {
        Log.d(TAG, "starting scan");
         start_ble_scan();
     }

     private void request_mtu_ble(int mtu) {
        String BLE_TAG = "bbw_request_mtu";
        boolean ret = false;
         if (!m_is_connected) {
             Log.d(BLE_TAG, "Device " + m_device_address + "not connected");
             return;
         }

        ret = m_blueGatt.requestMtu(mtu);
        if(ret) {
            Log.d(BLE_TAG, "MTU Request: " + mtu + " is successful");
        } else {
            Log.d(BLE_TAG, "MTU Request: " + mtu + " failed");
        }
     }

     private void handle_ble_failure(BluetoothGatt gatt) {
        String BLE_TAG = "bbw_handle_ble_failure";

         BluetoothDevice ble_device = gatt.getDevice();
         String ble_dev_addr = ble_device.getAddress();
         //int connectionState = gatt.getConnectionState(ble_device);
         int connectionState = mblueManager.getConnectionState(ble_device, BluetoothProfile.GATT);

         Log.d(BLE_TAG, "GATT Fail " + ble_dev_addr + " connection state: " + connectionState);

         if (m_blueGatt == null) {
             Log.d(BLE_TAG, "m_blueGatt is null");
             return;
         }
         m_blueGatt.close();
     }

     private void reset_ble_data()
     {
         m_svc_uuid = "";
         m_char_uuid = "";
     }

     private void handle_ble_disconnect(BluetoothGatt gatt) {
         BluetoothDevice ble_device = gatt.getDevice();
         String ble_dev_addr = ble_device.getAddress();

        if (m_user_req == UserReq.DISCONNECT_REQ && m_is_connected == true ) {
            Log.d(TAG, "disconnected " + ble_dev_addr + " on user request");
        } else {
            Log.d(TAG, "disconnected " + ble_dev_addr);
        }
        m_is_connected = false;

        if (m_blueGatt == null) {
             Log.d(TAG, "m_blueGatt is null");
             return;
        }
         Log.d(TAG, "closing gatt connection");
         m_blueGatt.close();
         m_blueGatt = null;

         //change to connect Button from main thread
         mConnectButton.post(new Runnable() {
             @Override
             public void run() {
                 //Change from disconnect button to connect button
                 mConnectButton.setText("connect");

                 hide_connected_view();
                 display_scan_view();
                 reset_ble_data();
                 update_device_status("Device Disconnected");
             }
         });
     }

     private void handle_discover_services() {
         m_mainHandler.post(new Runnable() {
             private static final String TAG =  "bbw_DiscoveryService";
             @SuppressLint("MissingPermission")
             @Override
             public void run() {
                 if (m_blueGatt != null) {
                     if (m_blueGatt.discoverServices() ) {
                         Log.d(TAG, "GATT Service discovery started");
                     } else {
                         Log.d(TAG, "GATT Service discovery failed");
                     }
                 } else {
                     Log.d(TAG, "m_blueGatt is null");
                 }
             }
         });
     }

     private void display_connected_device_view() {
         mConnectButton.setText("disconnect");
         mSpeedTestButton.setVisibility(View.VISIBLE);
         mPacketCountText.setVisibility(View.VISIBLE);
         mPacketSizeText.setVisibility(View.VISIBLE);
     }

     private void handle_ble_connect(BluetoothGatt gatt) {
         BluetoothDevice ble_device = gatt.getDevice();
         String ble_dev_addr = ble_device.getAddress();

         if (m_user_req == UserReq.CONNECT_REQ) {
             Log.d(TAG, "Connected " + ble_dev_addr + " on user request");
         } else {
             Log.d(TAG, "Connected " + ble_dev_addr );
         }
         m_is_connected = true;

         //change to disconnect Button from main thread
         mConnectButton.post(new Runnable() {
             @Override
             public void run() {
                 display_connected_device_view();
                 update_device_status("Device Connected");
             }
         });

         //discover services
         handle_discover_services();
     }

    private void setListViewHeight(ExpandableListView listView,
                                   int group) {
        ServiceListAdapter listAdapter = (ServiceListAdapter) listView.getExpandableListAdapter();
        int totalHeight = 0;
        int desiredWidth = View.MeasureSpec.makeMeasureSpec(listView.getWidth(),
                View.MeasureSpec.EXACTLY);
        for (int i = 0; i < listAdapter.getGroupCount(); i++) {
            View groupItem = listAdapter.getGroupView(i, false, null, listView);
            groupItem.measure(desiredWidth, View.MeasureSpec.UNSPECIFIED);

            totalHeight += groupItem.getMeasuredHeight();

            if (((listView.isGroupExpanded(i)) && (i != group))
                    || ((!listView.isGroupExpanded(i)) && (i == group))) {
                for (int j = 0; j < listAdapter.getChildrenCount(i); j++) {
                    View listItem = listAdapter.getChildView(i, j, false, null,
                            listView);
                    listItem.measure(desiredWidth, View.MeasureSpec.UNSPECIFIED);

                    totalHeight += listItem.getMeasuredHeight();

                }
            }
        }

        ViewGroup.LayoutParams params = listView.getLayoutParams();
        int height = totalHeight
                + (listView.getDividerHeight() * (listAdapter.getGroupCount() - 1));
        if (height < 10)
            height = 400;
        params.height = height;
        listView.setLayoutParams(params);
        listView.requestLayout();
    }

    private void render_service_group_view() {
        m_mainHandler.post(new Runnable () {
            private static final String TAG =  "bbw_render_service";
           @Override
            public void run() {
               m_serviceGroupList = new ArrayList<String>(m_ServiceGroupsHash.keySet());
               m_serviceListAdapter = new ServiceListAdapter(m_context, m_serviceGroupList, m_ServiceGroupsHash);
               mServiceListExpandView.setAdapter(m_serviceListAdapter);
               mServiceListExpandView.setOnGroupClickListener(new ExpandableListView.OnGroupClickListener() {
                   @Override
                   public boolean onGroupClick(ExpandableListView expandableListView, View view, int groupPosition, long id) {
                       setListViewHeight(expandableListView, groupPosition);
                       String service_str = m_serviceGroupList.get(groupPosition);
                       //Log.d(TAG, "onGroupClick: service uuid: " + service_str);
                       return false;
                   }
               });

               mServiceListExpandView.setOnChildClickListener(new ExpandableListView.OnChildClickListener() {
                   @Override
                   public boolean onChildClick(ExpandableListView expandableListView, View view,
                                               int groupPosition, int childPosition, long id) {
                       String service_uuid = m_serviceGroupList.get(groupPosition);
                       m_svc_uuid = service_uuid;
                       String char_str = m_ServiceGroupsHash.get(service_uuid).get(childPosition).get_char_uuid();
                       m_char_uuid = char_str;

                       //update speed text view
                       mResult = "Selected Ble attributes: \n";
                       mResult += " service: " + m_svc_uuid + "\n";
                       mResult += " char: " + m_char_uuid + "\n";
                       String result = mResult + m_default_packet_config;
                       display_bandwidth_result(result);
                       Log.d(TAG, "onChildClick: \n" + mResult);

                       return true;
                   }
               });
               mServiceListExpandView.setVisibility(View.VISIBLE);
           }
        });
    }

    private BluetoothGattCallback m_blueGattCallback = new  BluetoothGattCallback() {
        private String BLE_TAG = "bbw_Bluetooth_Callback";
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            BLE_TAG= "bbw_onConnectionStateChange";
            if (status == BluetoothGatt.GATT_SUCCESS) {
                switch (newState) {
                    case BluetoothProfile.STATE_CONNECTED:
                        Log.d(BLE_TAG, "Device Connected");
                        handle_ble_connect(gatt);
                        break;
                    case BluetoothProfile.STATE_DISCONNECTED:
                        Log.d(BLE_TAG, "Device disConnected");
                        handle_ble_disconnect(gatt);
                        break;
                    case BluetoothProfile.STATE_CONNECTING:
                        Log.d(BLE_TAG, "Device Connecting");
                        break;
                    case BluetoothProfile.STATE_DISCONNECTING:
                        Log.d(BLE_TAG, "Device disConnecting");
                        break;
                    default:
                        Log.d(BLE_TAG, "Unknown state received");
                }
            } else {
                Log.d(BLE_TAG, "GATT Connection error: " + status);
                handle_ble_failure(gatt);
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            BLE_TAG= "bbw_onServicesDiscovered";
            BluetoothDevice ble_device = gatt.getDevice();
            String ble_dev_addr = ble_device.getAddress();

            if (status != BluetoothGatt.GATT_SUCCESS) {
                Log.d(BLE_TAG, "ServiceDiscovered failed GATT error: " + status);

                //disconnect device
                m_mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        if (m_blueGatt == null) {
                            Log.d(BLE_TAG, "m_blueGatt is null: ");
                            return;
                        }
                        m_blueGatt.disconnect();
                    }
                });
                return;
            }

            Log.d(BLE_TAG, "ServiceDiscovered Successfully for " + ble_dev_addr);

            //check gatt services
            List<BluetoothGattService> services  = gatt.getServices();
            Log.d(BLE_TAG, "Discovered  " + services.size() + " Services");

            m_ServiceGroupsHash =  new HashMap<String, List<ChildInfo>> ();

            String services_str = " " ;
            for(BluetoothGattService service: services) {
                Log.d(BLE_TAG, " service: " + service.getUuid() );
                services_str = services_str + service.getUuid().toString() + "\n";
                retrieve_characteristic_all(service);
            }

            render_service_group_view();
            request_mtu_ble(MAX_MTU_REQ);
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            BLE_TAG= "bbw_onCharacteristicWrite";

            if (status != BluetoothGatt.GATT_SUCCESS) {
                Log.d(BLE_TAG, "onCharacteristicWrite failed GATT error: " + status);
                switch (status) {
                    case BluetoothGatt.GATT_WRITE_NOT_PERMITTED:
                        Log.d(BLE_TAG, "GATT Write error: GATT_WRITE_NOT_PERMITTED");
                        break;
                    case BluetoothGatt.GATT_INVALID_ATTRIBUTE_LENGTH:
                        Log.d(BLE_TAG, "GATT Write error: GATT_INVALID_ATTRIBUTE_LENGTH");
                    default:
                        Log.d(BLE_TAG, "GATT Write error: other");
                }
                return;
            }

            String char_str = characteristic.getUuid().toString();
            //Log.d(BLE_TAG, "Write Success " + "Characteristic UUID: " + char_str);

            //send packets
            if(m_curr_packet_count < m_packet_count ) {
                if (m_curr_packet_count == m_packet_count -1 ) {
                    write_char_data(characteristic, LAST_PACKET);
                } else {
                    write_char_data(characteristic, NEXT_PACKET);
                }
            } else if(m_curr_packet_count == m_packet_count) {
                //result time
                mEndTime = System.currentTimeMillis();
                long duration = mEndTime - mStartTime;
                int total_byte_sent = m_packet_count * m_packet_size;
                long speed = (total_byte_sent * 8)/ duration;  //bits/ms
                speed = speed * 1000;  // bits/s (bps)
                speed = speed / 1024; //

                Log.d(BLE_TAG, "Total byte sent: " + total_byte_sent + " duration: " + duration);
                String result = mResult + m_packet_config;
                result += "\nCurrent speed is: \n";
                result += " Data sent: " + total_byte_sent + " bytes\n";
                result += " Duration: " + duration + " ms \n";
                result += " Speed: " + speed + " kbps \n";

                display_bandwidth_result(result);

                enable_speed_test_button();
            }
        }

        @Override
        public void onMtuChanged(BluetoothGatt gatt, int mtu, int status) {
            BLE_TAG= "bbw_onMtuChanged";

            if (status != BluetoothGatt.GATT_SUCCESS) {
                Log.d(BLE_TAG, "MTU changed failed GATT error: " + status);
                return;
            }
            mCurrentMTU = mtu;
            Log.d(BLE_TAG, "Current MTU: " + mtu);

            //packet configuration
            m_default_packet_size = mCurrentMTU - 3;
            m_default_packet_config = "MTU:" + mCurrentMTU + " packet count: " +
                            m_default_packet_count +  " packet size: " + m_default_packet_size + "\n";
        }
    };

    public void ConnectDeviceOnClick(View view) {
        Log.d(TAG, "Connecting Device");
        BluetoothDevice ble_device = mblueAdapter.getRemoteDevice(m_device_address);
        if (!m_is_connected) {
            Log.d(TAG, "Connect Req Device " + m_device_address);
            m_user_req = UserReq.CONNECT_REQ;
            m_blueGatt = ble_device.connectGatt(this, false,
                                    m_blueGattCallback, BluetoothDevice.TRANSPORT_LE);
        } else {
            m_user_req = UserReq.DISCONNECT_REQ;
            if (m_blueGatt == null) {
                Log.d(TAG, "m_blueGatt is null");
                return;
            }
            Log.d(TAG, "Disconnect Req Device " + m_device_address);
            m_blueGatt.disconnect();
        }
    }

    private BluetoothGattCharacteristic get_characteristic(BluetoothGattService service, String char_uuid) {
        String BLE_TAG = "bbwget_characteristic";

        //list of characteristics
        List<BluetoothGattCharacteristic> char_list = service.getCharacteristics();
        for (BluetoothGattCharacteristic characteristic : char_list) {
            String char_str = characteristic.getUuid().toString();

            if (char_str.equals(char_uuid)) {
                Log.d(BLE_TAG, "Found characteristic uuid: " + char_uuid);
                return characteristic;
            }
        }
        return null;
    }

    private String retrieve_char_property(BluetoothGattCharacteristic characteristic) {
        String BLE_TAG = "bbw_display_property";

        int char_property = 0;
        String property_str = "Properties: ";

        char_property = characteristic.getProperties();

        if ((char_property & BluetoothGattCharacteristic.PROPERTY_WRITE) != 0) {
            property_str += "WRITE ";
        }

        if ((char_property & BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE) != 0) {
            property_str += "WRITE_NO_RESPONSE ";
        }

        if ((char_property & BluetoothGattCharacteristic.PROPERTY_READ) != 0) {
            property_str += "READ ";
        }
        if ((char_property & BluetoothGattCharacteristic.PROPERTY_NOTIFY) != 0) {
            property_str += "NOTIFY ";
        }
        if ((char_property & BluetoothGattCharacteristic.PROPERTY_INDICATE) != 0) {
            property_str += "INDICATE";
        }

        Log.d(BLE_TAG, "Properties: " + property_str);
        return property_str;
    }

    private void retrieve_characteristic_all(BluetoothGattService service) {
        String BLE_TAG = "bbw_display_char";

        //list of characteristics
        List<BluetoothGattCharacteristic> char_list = service.getCharacteristics();
        List<ChildInfo> charList = new ArrayList<ChildInfo>();

        for (BluetoothGattCharacteristic characteristic : char_list) {
            String char_str = characteristic.getUuid().toString();
            Log.d(BLE_TAG, "characteristic uuid: " + char_str);
            String char_property = retrieve_char_property(characteristic);

            charList.add(new ChildInfo(char_str, char_property));
        }

        String svc_uuid = service.getUuid().toString();
        m_ServiceGroupsHash.put(svc_uuid, charList);
    }

    private BluetoothGattService get_service(String svc_uuid) {
        String BLE_TAG = "bbw_get_service";

        List<BluetoothGattService> service_list  = m_blueGatt.getServices();
        for(BluetoothGattService service : service_list) {
            String svc_str = service.getUuid().toString();

            if (svc_str.equals(svc_uuid)) {
                Log.d(BLE_TAG, "Found service uuid: " + svc_uuid);
                return service;
            }
        }
        return null;
    }

    private void display_bandwidth_result(String result) {
        m_mainHandler.post(new Runnable() {
            @Override
            public void run() {
                mSpeedTextView.setVisibility(View.VISIBLE);
                mSpeedTextView.setText(result);
            }
        });
    }

    private void write_char_data(BluetoothGattCharacteristic ble_char, int packet_sequence) {
        String BLE_TAG = "bbw_write_char_data";

        if ((ble_char.getProperties() & BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE) != 0) {
            ble_char.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE);
        } else if ((ble_char.getProperties() & BluetoothGattCharacteristic.PROPERTY_WRITE) != 0)  {
            ble_char.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
        }

        //prepare data
        byte[] value = new byte[m_packet_size];
        for(int i = 0; i < m_packet_size; i++) {
            value[i] = (byte)i;
        }
       // value[0] = (byte)(++mcount % 120);//index
        value[0] = (byte) packet_sequence;
        value[m_packet_size -1] = value[0];

        //set data
        ble_char.setValue(value);

        if (m_blueGatt.writeCharacteristic(ble_char)) {
            //Log.d(BLE_TAG, "Write successful");
            ++m_curr_packet_count;
        } else {
            Log.d(BLE_TAG, "Write failed");
        }
    }

    private void write_characteristic_ble() {
        String BLE_TAG = "bbw_WriteChar";
        m_byte_sent = 0;
        m_curr_packet_count = 0;
        mStartTime = 0;
        mEndTime = 0;
        mcount = 0;

        if (!m_is_connected) {
            Log.d(BLE_TAG, "Device " + m_device_address + "not connected");
            return;
        }

        BluetoothGattService ble_service = get_service(m_svc_uuid);
        if (ble_service == null) {
            Log.d(BLE_TAG, "Not Found service: " + m_svc_uuid);
            return;
        }

        BluetoothGattCharacteristic ble_char = get_characteristic(ble_service, m_char_uuid);
        if (ble_char == null) {
            Log.d(BLE_TAG, "Not Found Characteristic : " + m_char_uuid);
            return;
        }

        // check write property
        if ((ble_char.getProperties() & BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE) == 0
                && (ble_char.getProperties() & BluetoothGattCharacteristic.PROPERTY_WRITE) == 0) {
            Log.d(BLE_TAG, m_char_uuid + " WRITE operation not supported");
            String result = mResult + "WRITE operation not supported \n";
            display_bandwidth_result(result);
            return;
        }

        //hide Speed Test button
        hide_speed_test_button();

        mStartTime = System.currentTimeMillis();
        write_char_data(ble_char, FIRST_PACKET);
    }

    public void SpeedTestOnClick(View view) {
        Log.d(TAG, "Testing BLE Speed");

        if (m_char_uuid.isEmpty() || m_svc_uuid.isEmpty()) {
            mResult = "BLE Characteristic is not selected";
            display_bandwidth_result(mResult);
            return;
        }

        m_packet_size = m_default_packet_size;
        m_packet_count = m_default_packet_count;

        //retrieve the packet count and packet size
        int packet_count = 0;
        int packet_size = 0;

        String packet_count_str = mPacketCountText.getText().toString();
        if (!packet_count_str.isEmpty()) {
            packet_count = Integer.parseInt(packet_count_str);
            m_packet_count = packet_count;
        }

        String packet_size_str = mPacketSizeText.getText().toString();
        if(!packet_size_str.isEmpty()) {
            packet_size = Integer.parseInt(packet_size_str);
            m_packet_size = packet_size;
        }

        //update the speed text view
        m_packet_config = "MTU:" + mCurrentMTU + " packet count: " +
                m_packet_count + " packet size: " + m_packet_size + "\n";

        String result = mResult + m_packet_config + " Analysing the speed... \n";
        display_bandwidth_result(result);

        // write characteristic
        write_characteristic_ble();
    }

     @Override
     protected void onResume() {
        super.onResume();
         mblueManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
         mblueAdapter = mblueManager.getAdapter();
         if(mblueAdapter == null) {
             Log.d(TAG, "mblueAdapter is null");
         }

         // Ensures Bluetooth is available on the device and it is enabled. If not,
         // displays a dialog requesting user permission to enable Bluetooth.
         if(!mblueAdapter.isEnabled() ) {
             Context context = getApplicationContext();
             int duration = Toast.LENGTH_SHORT;
             Toast toast = Toast.makeText(context, "Bluetooth Disabled", duration);
             toast.show();

             Log.d(TAG, "Bluetooth Disabled");
             // Intent enableBTIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
             // startActivityForResult(enableBTIntent, REQUEST_ENABLE_BT);

             return;
         }

         mblueLeScanner = mblueAdapter.getBluetoothLeScanner();
     }
}

