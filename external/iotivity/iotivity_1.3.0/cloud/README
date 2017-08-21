This project contains series of modules for building cloud services.

Modules

1. stack - Java 1.8 based CoAP over TCP implementation.
2. interface - cloud interface server that accept incoming IoTivity clients connection.
3. resource_directory - resource directory server that store clients resource models.
4. account - account server that handle clients credential using OAuth2.
5. message_queue - message queue server that provides pub/sub feature.
6. samples - sample IoTivity clients that show how clients working with cloud.

Each cloud modules has README file how to install and run.
You can run each modules on single computer or PAAS.

Note. resource_directory should launched before interface and account.

Architecture
client--------interface_server--------resource_directory_server--------mongo_db
                      \---------------account_server-------------------mongo_db
					  \---------------message_queue_server-------------kafka