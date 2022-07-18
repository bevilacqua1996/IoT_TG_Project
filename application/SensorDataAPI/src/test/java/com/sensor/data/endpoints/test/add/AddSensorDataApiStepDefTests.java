package com.sensor.data.endpoints.test.add;

import com.sensor.data.SensorDataApiApplication;
import com.sensor.data.api.model.SensorDataList;
import io.cucumber.java.Before;
import io.cucumber.java.en.Given;
import io.cucumber.java.en.Then;
import io.cucumber.java.en.When;
import io.cucumber.spring.CucumberContextConfiguration;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.boot.test.web.server.LocalServerPort;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.ResponseEntity;
import org.springframework.test.context.TestPropertySource;

import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.fail;

@CucumberContextConfiguration
@TestPropertySource(locations = "classpath:application-test.properties")
@SpringBootTest(classes = SensorDataApiApplication.class, webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class AddSensorDataApiStepDefTests {

    @LocalServerPort
    private int port;
    private URL base;

    @Autowired
    private TestRestTemplate template;

    private ResponseEntity<String> response;

    @Before
    public void startMockServer() {
        try {
            base = new URL("http://localhost:" + port + '/');

        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
    }

    @Given("client has access to the \\/sensorData API")
    public void client_has_access_to_the_sensor_data_api() {
        assertEquals(true, true);
    }

    private String getURL() {
        String endpoint = base.toString() +
                "sensorData/addInfo";
        return endpoint;
    }

    @When("client calls POST \\/sensorData with id {int}, code {int}, factor {int} and data {int};{int};{int};{int}")
    public void client_calls_post_sensor_data_with_id_code_factor_and_data(Integer id, Integer code, Integer factor, Integer data1, Integer data2, Integer data3, Integer data4) throws MalformedURLException, URISyntaxException {
        HttpHeaders headers = new HttpHeaders();
        headers.add("Content-Type", "application/json");

        SensorDataList sensorDataList = buildRequestObject(id, code, factor, data1, data2, data3, data4);

        HttpEntity httpEntity = new HttpEntity(sensorDataList, headers);
        response = template.postForEntity(new URL(getURL()).toURI(), httpEntity, String.class);
    }

    private SensorDataList buildRequestObject(Integer id, Integer code, Integer factor, Integer data1, Integer data2, Integer data3, Integer data4) {
        List<Integer> dataList = new ArrayList<>();
        dataList.add(data1);
        dataList.add(data2);
        dataList.add(data3);
        dataList.add(data4);

        SensorDataList sensorDataList = new SensorDataList();
        sensorDataList.setId(id);
        sensorDataList.setCode(code);
        sensorDataList.setFactor(factor);
        sensorDataList.setData(dataList);

        return sensorDataList;
    }

    @Then("client sensorData receives status code of {int}")
    public void client_sensor_data_receives_status_code_of(Integer statusCode) {
        if (response != null) {
            assertEquals(response.getStatusCode().value(), statusCode);
        } else {
            fail("Sem resposta");
        }
    }

}
