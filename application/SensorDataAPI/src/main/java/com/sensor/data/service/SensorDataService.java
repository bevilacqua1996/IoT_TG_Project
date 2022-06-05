package com.sensor.data.service;

import com.sensor.data.api.model.SensorDataList;
import com.sensor.data.api.model.SensorDataModel;
import com.sensor.data.model.SensorDataEntity;

import java.util.List;

public interface SensorDataService {

    List<SensorDataEntity> getAllSensorData();
    void addSensorDataList(SensorDataList sensorDataList);
    void addSensorData(SensorDataModel sensorData);

}
