FROM openjdk:11

RUN mkdir /opt/API
COPY ./* /opt/API/
RUN cd /opt/API/ && mvn clean install
WORKDIR /opt/API/target
EXPOSE $PORT
CMD ["java", "-jar", "-Dserver.port=${PORT}", "-Xmx256m", "./target/SensorDataAPI-0.0.1-SNAPSHOT.jar"]