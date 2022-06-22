FROM openjdk:11

RUN mkdir /opt/API
COPY ./SensorDataAPI /opt/API/SensorDataAPI

# Downloading and installing Maven
ARG MAVEN_VERSION=3.6.3
ARG USER_HOME_DIR="/root"
ARG BASE_URL=https://apache.osuosl.org/maven/maven-3/${MAVEN_VERSION}/binaries

RUN mkdir -p /usr/share/maven /usr/share/maven/ref \
  && echo "Downlaoding maven" \
  && curl -fsSL -o /tmp/apache-maven.tar.gz ${BASE_URL}/apache-maven-${MAVEN_VERSION}-bin.tar.gz \
  \
  && echo "Unziping maven" \
  && tar -xzf /tmp/apache-maven.tar.gz -C /usr/share/maven --strip-components=1 \
  \
  && echo "Cleaning and setting links" \
  && rm -f /tmp/apache-maven.tar.gz \
  && ln -s /usr/share/maven/bin/mvn /usr/bin/mvn

ENV MAVEN_HOME /usr/share/maven
ENV MAVEN_CONFIG "$USER_HOME_DIR/.m2"

RUN ls /opt/API/SensorDataAPI && cd /opt/API/SensorDataAPI && mvn clean install -DskipTests=true
WORKDIR /opt/API/SensorDataAPI/target
EXPOSE $PORT
CMD ["java", "-jar", "-Dserver.port=${PORT}", \
  "-Dspring.datasource.url=${DATABASE_APP_URL}", \
  "-Dspring.datasource.username=${DATABASE_USERNAME}", \
  "-Dspring.datasource.password=${DATABASE_PASSWORD}", \
  "-Xmx256m", "SensorDataAPI-1.0.0-SNAPSHOT.jar"]