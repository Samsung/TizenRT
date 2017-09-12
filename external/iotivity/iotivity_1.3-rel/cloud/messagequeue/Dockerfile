FROM openjdk:8-jre

ADD ./target/CloudMessageQueue-0.0.1-SNAPSHOT.jar iotivity/MessageQueue.jar
ADD ./target/lib/* iotivity/lib/

WORKDIR iotivity/

ENV TLS_MODE=0
ENV ZOOKEEPER_ADDRESS kafka-zookeeper
ENV ZOOKEEPER_PORT 2181
ENV KAFKA_ADDRESS kafka-zookeeper
ENV KAFKA_PORT 9092

EXPOSE 5686
ENTRYPOINT ["java", "-jar", "MessageQueue.jar"]