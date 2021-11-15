package com.sensor.data.model;

import javax.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "sensor_data")
public class SensorDataEntity {
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Integer id;
	
	private Integer rpm;

	private Double voltage;

	@Column(name = "timestamp_microcontroler")
	private long timestampMicrocontroler;

	@Column(name = "timestamp")
	private long timestamp;

	private LocalDateTime date;

	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public long getTimestampMicrocontroler() {
		return timestampMicrocontroler;
	}

	public void setTimestampMicrocontroler(long timestampMicrocontroler) {
		this.timestampMicrocontroler = timestampMicrocontroler;
	}

	public long getTimestamp() {
		return timestamp;
	}

	public void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}

	public LocalDateTime getDate() {
		return date;
	}

	public void setDate(LocalDateTime date) {
		this.date = date;
	}

	public Integer getRpm() {
		return rpm;
	}

	public void setRpm(Integer rpm) {
		this.rpm = rpm;
	}

	public Double getVoltage() {
		return voltage;
	}

	public void setVoltage(Double voltage) {
		this.voltage = voltage;
	}
}
