package com.sensor.data.service;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.sensor.data.model.SensorDataEntity;
import com.sensor.data.repository.SensorDataRepository;

@Service
public class SensorDataService {

	@Autowired
	private SensorDataRepository sensorDataRepo;

	public List<SensorDataEntity> getAllSensorData() {
		List<SensorDataEntity> sensorDataArray = new ArrayList<SensorDataEntity>();
		sensorDataRepo.findAll().forEach(sensorDataArray::add);
		return sensorDataArray;
	}

	public void addSensorData(SensorDataEntity sensorData) {
		sensorDataRepo.save(sensorData);
	}

}
