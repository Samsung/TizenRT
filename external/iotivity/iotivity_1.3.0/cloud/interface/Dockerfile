FROM openjdk:8-jre

ADD ./target/CloudInterface-0.0.1-SNAPSHOT.jar iotivity/CloudInterface.jar
ADD ./target/lib/* iotivity/lib/

WORKDIR iotivity/

ENV TLS_MODE=0
ENV KEEPALIVE_CLOUD=1
ENV HC_PROXY_MODE=0
ENV WEBSOCKET_MODE=0
ENV RESOURCE_DIRECTORY_ADDRESS iotivity-resourcedirectory
ENV ACCOUNT_SERVER_ADDRESS iotivity-accountserver
ENV MESSAGE_QUEUE_ADDRESS iotivity-messagequeue

EXPOSE 5683
EXPOSE 80
ENTRYPOINT ["java", "-jar", "CloudInterface.jar"]
