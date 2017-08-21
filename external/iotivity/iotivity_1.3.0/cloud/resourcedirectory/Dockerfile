FROM openjdk:8-jre

ADD ./target/CloudResourceDirectory-0.0.1-SNAPSHOT.jar iotivity/ResourceDirectory.jar
ADD ./target/lib/* iotivity/lib/

WORKDIR iotivity/

ENV TLS_MODE=0
ENV MONGODB_ADDRESS mongodb
ENV MONGODB_PORT 27017

EXPOSE 5684
ENTRYPOINT ["java", "-jar", "ResourceDirectory.jar"]