package com.sensor.data.model;

import com.sensor.data.api.model.SensorDataList;

import java.util.ArrayList;

public class SensorDataEntitiesDTO extends ArrayList<SensorDataEntity> {

    public void buildSensorDataEntities(SensorDataList sensorDataList) {
        if(sensorDataList.getCode() == DataCodeEnum.TIMESTAMP.getCode()) {
            for(Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setTimestampMicrocontroler(data / sensorDataList.getFactor());
                add(sensorDataEntity);
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.RPM.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setRpm(data / sensorDataList.getFactor());
                add(sensorDataEntity);
            }
        }else if(sensorDataList.getCode() == DataCodeEnum.VOLTAGE.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setVoltage(data / sensorDataList.getFactor().doubleValue());
                add(sensorDataEntity);
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_A1.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setA1Acc(data / sensorDataList.getFactor().doubleValue());
                add(sensorDataEntity);
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_A2.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setA2Acc(data / sensorDataList.getFactor().doubleValue());
                add(sensorDataEntity);
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_A3.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setA3Acc(data / sensorDataList.getFactor().doubleValue());
                add(sensorDataEntity);
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.TEMPERATURE.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setTemperature(data / sensorDataList.getFactor().doubleValue());
                add(sensorDataEntity);
            }
        }
    }

    public void updateSensorDataEntities(SensorDataList sensorDataList) {
        if(sensorDataList.getCode() == DataCodeEnum.TIMESTAMP.getCode()) {
            for(int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setTimestampMicrocontroler(sensorDataList.getData().get(i) / sensorDataList.getFactor());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.RPM.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setRpm(sensorDataList.getData().get(i) / sensorDataList.getFactor());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.VOLTAGE.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setVoltage(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_A1.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setA1Acc(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_A2.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setA2Acc(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_A3.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setA3Acc(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.TEMPERATURE.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setTemperature(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        }
    }

}
