package com.sensor.data.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import com.sensor.data.model.SensorDataEntity;
import com.sensor.data.service.SensorDataService;

@RestController
public class SensorController {

	@Autowired
	private SensorDataService sensorDataService;

	@GetMapping("/sensorData")
	public List<SensorDataEntity> getSensorData() {
		return sensorDataService.getAllSensorData();
	}
	
	@PostMapping("/sensorData")
	public void addSensorData(@RequestBody SensorDataEntity sensorData) {
		sensorDataService.addSensorData(sensorData);
	}

}
