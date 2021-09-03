package com.sensor.data.repository;

import org.springframework.data.jpa.repository.JpaRepository;

import com.sensor.data.model.SensorDataEntity;

public interface SensorDataRepository extends JpaRepository<SensorDataEntity, Integer> {

}
