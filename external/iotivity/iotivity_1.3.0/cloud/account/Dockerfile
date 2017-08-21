FROM openjdk:8-jre

ADD ./target/CloudAccount-0.0.1-SNAPSHOT.jar iotivity/AccountServer.jar
ADD ./target/lib/* iotivity/lib/
ADD ./properties/* iotivity/properties/

WORKDIR iotivity/

ENV TLS_MODE=0
ENV MONGODB_ADDRESS=mongodb
ENV MONGODB_PORT=27017

EXPOSE 5685
ENTRYPOINT ["java", "-jar", "AccountServer.jar"]
