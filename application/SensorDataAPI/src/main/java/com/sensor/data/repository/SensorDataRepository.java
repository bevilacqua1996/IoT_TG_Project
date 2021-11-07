package com.sensor.data.repository;

import com.sensor.data.model.SensorDataEntity;
import org.springframework.data.jpa.repository.JpaRepository;

public interface SensorDataRepository extends JpaRepository<SensorDataEntity, Integer> {

}
