package com.sensor.data.endpoints.test;

import io.cucumber.junit.Cucumber;
import io.cucumber.junit.CucumberOptions;
import org.junit.runner.RunWith;

@RunWith(Cucumber.class)
@CucumberOptions(features = "src/test/resources/features/addSensorData.feature", plugin = {"pretty",
        "json:target/reports/json/addSensorData.json",
        "html:target/reports/addSensorData.html",}, glue = {"com.sensor.data"})
public class AddSensorDataApiTest {
}
