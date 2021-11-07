FROM openjdk:11

RUN mkdir /opt/API
COPY ./target/SensorDataAPI-0.0.1-SNAPSHOT.jar /opt/API/
WORKDIR /opt/API/
EXPOSE $PORT
CMD ["java", "-jar", "-Dserver.port=${PORT}", "-Xmx256m", "SensorDataAPI-0.0.1-SNAPSHOT.jar"]