package com.sensor.data.service.impl;

import com.sensor.data.api.model.SensorDataList;
import com.sensor.data.model.SensorDataEntitiesDTO;
import com.sensor.data.model.SensorDataEntity;
import com.sensor.data.repository.SensorDataRepository;
import com.sensor.data.service.SensorDataService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

@Service
public class SensorDataServiceImpl implements SensorDataService {

	@Autowired
	private SensorDataRepository sensorDataRepo;

	private Integer key = -1;

	private SensorDataEntitiesDTO sensorDataEntities = new SensorDataEntitiesDTO();

	private static final String GMT_BR = "-03:00";
	private static final String REGION = "America/Sao_Paulo";

	public List<SensorDataEntity> getAllSensorData() {
		List<SensorDataEntity> sensorDataArray = new ArrayList<>();
		sensorDataRepo.findAll().forEach(sensorDataArray::add);
		return sensorDataArray;
	}

	@Override
	public void addSensorDataList(SensorDataList sensorDataList) {
		if(sensorDataList.getId().intValue() != key.intValue()) {
			if(sensorDataEntities.isEmpty()) {
				sensorDataEntities.buildSensorDataEntities(sensorDataList);
			} else {
				persistSensorData();
				sensorDataEntities.clear();
				sensorDataEntities.buildSensorDataEntities(sensorDataList);
			}
			key = sensorDataList.getId();
		} else {
			sensorDataEntities.updateSensorDataEntities(sensorDataList);
		}
	}

	private void persistSensorData() {
		for(SensorDataEntity sensorDataEntity : sensorDataEntities) {
			sensorDataEntity.setTimestamp(LocalDateTime.now().toInstant(ZoneOffset.of(GMT_BR)).toEpochMilli());

			long timestamp = sensorDataEntity.getTimestampMicrocontroler() == 0 ?
					sensorDataEntity.getTimestamp() : sensorDataEntity.getTimestampMicrocontroler() * 1000;

			LocalDateTime dataMedicao = new Date(timestamp).toInstant().
					atZone(ZoneId.of(REGION)).
					toLocalDateTime();

			sensorDataEntity.setDate(dataMedicao);
			sensorDataRepo.save(sensorDataEntity);
		}
	}

}
