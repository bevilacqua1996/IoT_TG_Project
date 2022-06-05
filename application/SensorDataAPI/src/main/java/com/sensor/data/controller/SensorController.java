package com.sensor.data.controller;

import com.sensor.data.api.SensorDataApi;
import com.sensor.data.api.model.SensorDataList;
import com.sensor.data.api.model.SensorDataModel;
import com.sensor.data.service.SensorDataService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class SensorController implements SensorDataApi {

	@Autowired
	private SensorDataService sensorDataService;

	@Override
	public ResponseEntity<String> addSensorData(SensorDataList body) {
		sensorDataService.addSensorDataList(body);

		return new ResponseEntity("Requisição bem sucedida", HttpStatus.OK);
	}

	@Override
	public ResponseEntity<List<Object>> getAllSensorDataInfo() {
		return new ResponseEntity(sensorDataService.getAllSensorData(), HttpStatus.OK);
	}
}
