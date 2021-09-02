package com.sensor.data.controller;

import java.util.concurrent.ExecutionException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.sensor.data.model.SensorData;
import com.sensor.data.service.SensorDataService;

@RestController
public class SensorController {

	@Autowired
	private SensorDataService sensorDataService;

	@GetMapping("/sensorData")
	public SensorData getSensorData(@RequestParam String documentId) throws InterruptedException, ExecutionException {
		return sensorDataService.getSensorDataById(documentId);
	}

	@PostMapping("/sensorData")
	public void addSensorData(@RequestBody SensorData sensorData) throws InterruptedException, ExecutionException {
		sensorDataService.addData(sensorData);
	}

}
