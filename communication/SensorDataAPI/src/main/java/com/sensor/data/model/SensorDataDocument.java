package com.sensor.data.model;

public class SensorDataDocument {
	
	private long id;
	private int rpm;
	
	public SensorDataDocument() {
	}
	
	public SensorDataDocument(long id, int rpm) {
		this.id = id;
		this.rpm = rpm;
	}

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public int getRpm() {
		return rpm;
	}

	public void setRpm(int rpm) {
		this.rpm = rpm;
	}
	
}
