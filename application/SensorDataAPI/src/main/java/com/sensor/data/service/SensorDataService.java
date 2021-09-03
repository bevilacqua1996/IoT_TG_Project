package com.sensor.data.service;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.google.api.core.ApiFuture;
import com.google.cloud.firestore.DocumentSnapshot;
import com.google.cloud.firestore.Firestore;
import com.google.cloud.firestore.WriteResult;
import com.sensor.data.model.SensorDataDocument;
import com.sensor.data.model.SensorDataEntity;
import com.sensor.data.repository.SensorDataRepository;

@Service
public class SensorDataService {

	private final static Logger logger = LoggerFactory.getLogger(SensorDataService.class);

	@Autowired
	private SensorDataRepository sensorDataRepo;

	@Autowired
	private Firestore firestore;

	public SensorDataDocument getFirestoreSensorDataById(String documentId)
			throws InterruptedException, ExecutionException {
		ApiFuture<DocumentSnapshot> apiFuture = this.firestore.document("sensorData/data" + documentId).get();
		DocumentSnapshot documentSnapshot = apiFuture.get();
		return documentSnapshot.toObject(SensorDataDocument.class);
	}

	public void addFirestoreData(SensorDataDocument sensorData) throws InterruptedException, ExecutionException {
		ApiFuture<WriteResult> apiFuture = this.firestore.document("sensorData/data" + sensorData.getId())
				.set(sensorData);
		WriteResult writeResult = apiFuture.get();
		logger.info("Update time: {}", writeResult.getUpdateTime());
	}

	public List<SensorDataEntity> getAllSensorData() {
		List<SensorDataEntity> sensorDataArray = new ArrayList<SensorDataEntity>();
		sensorDataRepo.findAll().forEach(sensorDataArray::add);
		return sensorDataArray;
	}

	public void addSensorData(SensorDataEntity sensorData) {
		sensorDataRepo.save(sensorData);
	}

}
