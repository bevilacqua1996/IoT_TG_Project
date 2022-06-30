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

	@Column(name = "x_acc")
	private Double xAcc;

	@Column(name = "y_acc")
	private Double yAcc;

	@Column(name = "z_acc")
	private Double zAcc;

	@Column(name = "temperature")
	private Double temperature;

	@Column(name = "timestamp_microcontroler")
	private long timestampMicrocontroler;

	@Column(name = "timestamp")
	private long timestamp;

	private LocalDateTime date;

}
