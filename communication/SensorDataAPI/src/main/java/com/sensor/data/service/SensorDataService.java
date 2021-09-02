package com.sensor.data.service;

import java.util.concurrent.ExecutionException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.google.api.core.ApiFuture;
import com.google.cloud.firestore.DocumentSnapshot;
import com.google.cloud.firestore.Firestore;
import com.google.cloud.firestore.WriteResult;
import com.sensor.data.model.SensorData;

@Service
public class SensorDataService {
	
	private final static Logger logger = LoggerFactory.getLogger(SensorDataService.class);
	
	@Autowired
    private Firestore firestore;

	public SensorData getSensorDataById(String documentId) throws InterruptedException, ExecutionException {
	    ApiFuture<DocumentSnapshot> apiFuture = this.firestore.document("sensorData/data" + documentId).get();
	    DocumentSnapshot documentSnapshot = apiFuture.get();
	    return documentSnapshot.toObject(SensorData.class);
	}

	public void addData(SensorData sensorData) throws InterruptedException, ExecutionException {
	    ApiFuture<WriteResult> apiFuture = this.firestore.document("sensorData/data" + sensorData.getId()).set(sensorData);
	    WriteResult writeResult = apiFuture.get();
	    logger.info("Update time: {}", writeResult.getUpdateTime());
	}

}
