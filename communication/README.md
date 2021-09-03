# Communication

Camada que tem como objetivo a implementação do firmware responsável pela coleta dos dados dos sensores e envio para API.

## Optical Sensor Reading

Este módulo se refere à implementação da captação de dados do sensor óptico e envio de pacote de dados por *LoRa* visando a leitura de dados referente ao RPM de um motor.

Para montagem apropriada do circuito, sugere-se que se siga as seguintes pinagens para uma integração do *sensor TCRT5000* com *LoRa32*.

```
LoRa  TCRT5000
3v3     VCC
GND     GND
23      D0
0       A0
```

Não se faz necessário o uso do pino A0 do sensor óptico, pois neste caso iremos apenas utilizar o OUTPUT digital do sensor que nos permite inferir se foi detectado movimento ou não.

Para mais informações a respeito do sensor TCRT5000 e sua integração com ESP32, pode-se acessar o [link](https://diyi0t.com/tcrt5000-line-tracking-module-arduino-esp8266-esp32/).

Além disso, para um maior detalhamento a respeito da pinagem do *LoRa32*, abaixo pode-se observar o diagrama da pinagem do dispositivo.

![LoRa32PinoutDiagram](https://user-images.githubusercontent.com/18063196/130385861-da05a071-3046-4306-8e74-78c8c67cb129.png)
