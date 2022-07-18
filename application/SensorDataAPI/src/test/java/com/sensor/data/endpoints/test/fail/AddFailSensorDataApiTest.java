package com.sensor.data.endpoints.test.fail;

import io.cucumber.junit.Cucumber;
import io.cucumber.junit.CucumberOptions;
import org.junit.runner.RunWith;

@RunWith(Cucumber.class)
@CucumberOptions(features = "src/test/resources/features/addFailSensorData.feature", plugin = {"pretty",
        "json:target/reports/json/addFailSensorData.json",
        "html:target/reports/addFailSensorData.html",}, glue = {"com.sensor.data.endpoints.test.fail"})
public class AddFailSensorDataApiTest {
}
