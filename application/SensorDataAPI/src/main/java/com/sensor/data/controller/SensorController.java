package com.sensor.data.controller;

import java.util.List;
import java.util.concurrent.ExecutionException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.sensor.data.model.SensorDataDocument;
import com.sensor.data.model.SensorDataEntity;
import com.sensor.data.service.SensorDataService;

@RestController
public class SensorController {

	@Autowired
	private SensorDataService sensorDataService;

	@GetMapping("/firestore/sensorData")
	public SensorDataDocument getSensorData(@RequestParam String documentId) throws InterruptedException, ExecutionException {
		return sensorDataService.getFirestoreSensorDataById(documentId);
	}

	@PostMapping("/firestore/sensorData")
	public void addSensorData(@RequestBody SensorDataDocument sensorData) throws InterruptedException, ExecutionException {
		sensorDataService.addFirestoreData(sensorData);
	}
	
	@GetMapping("/sensorData")
	public List<SensorDataEntity> getSensorData() {
		return sensorDataService.getAllSensorData();
	}
	
	@PostMapping("/sensorData")
	public void addSensorData(@RequestBody SensorDataEntity sensorData) {
		sensorDataService.addSensorData(sensorData);
	}

}
