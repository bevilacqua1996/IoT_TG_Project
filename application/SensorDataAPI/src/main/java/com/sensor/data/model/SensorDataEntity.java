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

	@Column(name = "acc_a1")
	private Double a1Acc;

	@Column(name = "acc_a2")
	private Double a2Acc;

	@Column(name = "acc_a3")
	private Double a3Acc;

	@Column(name = "temperature")
	private Double temperature;

	@Column(name = "timestamp_microcontroler")
	private long timestampMicrocontroler;

	@Column(name = "timestamp")
	private long timestamp;

	private LocalDateTime date;

}
