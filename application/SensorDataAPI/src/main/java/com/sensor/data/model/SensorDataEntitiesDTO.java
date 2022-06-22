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
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_X.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setXAcc(data / sensorDataList.getFactor().doubleValue());
                add(sensorDataEntity);
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_Y.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setYAcc(data / sensorDataList.getFactor().doubleValue());
                add(sensorDataEntity);
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_Z.getCode()) {
            for (Integer data : sensorDataList.getData()) {
                SensorDataEntity sensorDataEntity = new SensorDataEntity();
                sensorDataEntity.setZAcc(data / sensorDataList.getFactor().doubleValue());
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
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_X.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setXAcc(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_Y.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setYAcc(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.ACC_Z.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setZAcc(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        } else if(sensorDataList.getCode() == DataCodeEnum.TEMPERATURE.getCode()) {
            for (int i=0; i<sensorDataList.getData().size(); i++) {
                get(i).setTemperature(sensorDataList.getData().get(i) / sensorDataList.getFactor().doubleValue());
            }
        }
    }

}
