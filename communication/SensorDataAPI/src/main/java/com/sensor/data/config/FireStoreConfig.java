package com.sensor.data.config;

import java.io.IOException;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.google.auth.oauth2.GoogleCredentials;
import com.google.cloud.firestore.Firestore;
import com.google.cloud.firestore.FirestoreOptions;


@Configuration
public class FireStoreConfig {
	
	@Bean
	public Firestore getFireStore(@Value("${firebase.credential.path}") String credentialPath) throws IOException {
		var serviceAccount = getClass().getClassLoader().getResourceAsStream(credentialPath);
		var credentials = GoogleCredentials.fromStream(serviceAccount);

		var options = FirestoreOptions.newBuilder()
						.setCredentials(credentials).build();

		return options.getService();
	}

}
