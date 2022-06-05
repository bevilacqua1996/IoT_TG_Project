package com.sensor.data.service.impl;

import com.sensor.data.api.model.SensorDataList;
import com.sensor.data.api.model.SensorDataModel;
import com.sensor.data.model.SensorDataEntity;
import com.sensor.data.repository.SensorDataRepository;
import com.sensor.data.service.SensorDataService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.ArrayList;
import java.util.List;

@Service
public class SensorDataServiceImpl implements SensorDataService {

	@Autowired
	private SensorDataRepository sensorDataRepo;

	@Value("${sensor.data.buffer:5}")
	private int sensorDataBuffer;

	private static final String GMT_BR = "-03:00";
	private static final String REGION = "America/Sao_Paulo";

	public List<SensorDataEntity> getAllSensorData() {
		List<SensorDataEntity> sensorDataArray = new ArrayList<>();
		sensorDataRepo.findAll().forEach(sensorDataArray::add);
		return sensorDataArray;
	}

	@Override
	public void addSensorDataList(SensorDataList sensorDataList) {
		for(int i=0; i<sensorDataBuffer; i++) {
			SensorDataModel sensorDataModel = new SensorDataModel();

			sensorDataModel.setRpm(sensorDataList.getRpm().get(i));
			sensorDataModel.setTimestampMicrocontroler(sensorDataList.getTimestampMicrocontroler().get(i));
			sensorDataModel.setVoltage(sensorDataList.getVoltage().get(i));
			sensorDataModel.setXAcc(sensorDataList.getXAcc().get(i));
			sensorDataModel.setYAcc(sensorDataList.getYAcc().get(i));
			sensorDataModel.setZAcc(sensorDataList.getZAcc().get(i));
			sensorDataModel.setTemperature(sensorDataList.getTemperature().get(i));

			addSensorData(sensorDataModel);
		}

	}

	public void addSensorData(SensorDataModel sensorData) {

		SensorDataEntity sensorDataEntity = new SensorDataEntity();

		sensorDataEntity.setTimestampMicrocontroler(sensorData.getTimestampMicrocontroler());
		sensorDataEntity.setRpm(sensorData.getRpm());
		sensorDataEntity.setVoltage(sensorData.getVoltage());
		sensorDataEntity.setxAcc(sensorData.getXAcc());
		sensorDataEntity.setyAcc(sensorData.getYAcc());
		sensorDataEntity.setzAcc(sensorData.getZAcc());
		sensorDataEntity.setTemperature(sensorData.getTemperature());

		sensorDataEntity.setTimestamp(LocalDateTime.now().toInstant(ZoneOffset.of(GMT_BR)).toEpochMilli());
		sensorDataEntity.setDate(LocalDateTime.now(ZoneId.of(REGION)));

		sensorDataRepo.save(sensorDataEntity);

	}



}
