package com.sensor.data.model;

public enum DataCodeEnum {

    TIMESTAMP(0),
    RPM(1),
    VOLTAGE(2),
    ACC_X(3),
    ACC_Y(4),
    ACC_Z(5),
    TEMPERATURE(6);

    private int code;

    DataCodeEnum(int code) {
        this.code = code;
    }

    public int getCode() {
        return code;
    }

    public static DataCodeEnum ofCodeNumber(int code) {
        for(DataCodeEnum codeEnum : DataCodeEnum.values()) {
            if(codeEnum.getCode() == code) {
                return codeEnum;
            }
        }
        throw new IllegalArgumentException( "Código não encontrado: " + String.valueOf(code) );
    }
}
