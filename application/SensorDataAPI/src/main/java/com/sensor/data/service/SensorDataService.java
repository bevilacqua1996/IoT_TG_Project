package com.sensor.data.service;

import com.sensor.data.model.SensorDataEntity;
import com.sensor.data.repository.SensorDataRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.ArrayList;
import java.util.List;

@Service
public class SensorDataService {

	@Autowired
	private SensorDataRepository sensorDataRepo;

	private static final String GMT_BR = "-03:00";
	private static final String REGION = "America/Sao_Paulo";

	public List<SensorDataEntity> getAllSensorData() {
		List<SensorDataEntity> sensorDataArray = new ArrayList<SensorDataEntity>();
		sensorDataRepo.findAll().forEach(sensorDataArray::add);
		return sensorDataArray;
	}

	public void addSensorData(SensorDataEntity sensorData) {

		sensorData.setTimestamp(LocalDateTime.now().toInstant(ZoneOffset.of(GMT_BR)).toEpochMilli());
		sensorData.setDate(LocalDateTime.now(ZoneId.of(REGION)));

		sensorDataRepo.save(sensorData);
	}

}
