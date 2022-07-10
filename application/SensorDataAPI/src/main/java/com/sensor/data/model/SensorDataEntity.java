package com.sensor.data.model;

import lombok.Data;

import javax.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "sensor_data")
@Data
public class SensorDataEntity {
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Integer id;
	
	private Integer rpm;

	private Double voltage;

	@Column(name = "a1_acc")
	private Double a1Acc;

	@Column(name = "a2_acc")
	private Double a2Acc;

	@Column(name = "a3_acc")
	private Double a3Acc;

	@Column(name = "temperature")
	private Double temperature;

	@Column(name = "timestamp_microcontroler")
	private long timestampMicrocontroler;

	@Column(name = "timestamp")
	private long timestamp;

	private LocalDateTime date;

}
