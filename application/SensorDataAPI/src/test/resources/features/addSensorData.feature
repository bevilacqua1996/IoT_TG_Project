Feature: API that tests addSensorData API

  Scenario Outline: client calls POST /sensorData/addInfo
    Given client has access to the /sensorData API
    When client calls POST /sensorData with id <id>, code <code>, factor <factor> and data <data>
    Then client sensorData receives status code of <status>

    Examples:
      | id            | code    | factor | data                                        | status |
      | 27            | 0       | 1      | 1655164571;1655164572;1655164573;1655164574 | 200    |
      | 27            | 1       | 1      | 1655164571;1655164572;1655164573;1655164574 | 200    |
      | 28            | 2       | 100    | 1655164571;1655164572;1655164573;1655164574 | 200    |