Feature: API that tests failed data on SensorData API

  Scenario Outline: client calls POST /sensorData/addInfo
    Given client has access to the /sensorData API
    When client calls POST /sensorData with id <id>, code <code>, factor <factor> and data <data>
    Then client sensorData receives status code of <status>

    Examples:
      | id                 | code    | factor | data                                        | status |
      | 3456789            | 0       | 1      | 1658103128;1658103129;1658103130;1658103131 | 200    |
      | 3456789            | 1       | 1000   | 3500000;3500000;3600000;3550000             | 200    |
      | 3456789            | 2       | 100    | 12700;12500;12600;12700                     | 200    |
      | 3456789            | 3       | 1000   | 300;300;300;300                             | 200    |
      | 3456789            | 4       | 1000   | 200;200;200;200                             | 200    |
      | 3456789            | 5       | 1000   | 100;100;100;100                             | 200    |
      | 3456789            | 6       | 1000   | 24000;53000;54000;60000                     | 200    |
      | 3456790            | 1       | 1      | 1658093972;1658093973;1658093974;1658093975 | 200    |