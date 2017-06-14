Eclipse plug-in
-----------------


Pre-requisites to launch Eclipse plug-in
------------------------------------------

1.Eclipse IDE with PDE(Plug-in Development Environment) support.

2.Execute the "scons SIMULATOR=1" command from the IoTivity home
directory in the terminal to build Simulator specific libraries along
with the iotivity native libraries.

3.Simulator application requires liboc.so, liboctbstack.so and
liboc_logger.so native libraries and libSimulatorManager.so,
libRamlParser.so simulator specific libraries, all of which are
generated in ~/iotivity/out/linux/<arch>/release directory.


Setting up and launching the Eclipse plug-in projects
--------------------------------------------------------

1. Import the below projects into Eclipse IDE using File -> Import ->
Select 'Existing projects into Workspace' under General category -> click
next -> Browse to the location as given below -> Click Finish.

i)   Service Provider Plugin:
~/iotivity/service/simulator/java/eclipse-plugin/ServiceProviderPlugin

ii)  Client Controller Plugin:
~/iotivity/service/simulator/java/eclipse-plugin/ClientControllerPlugin

iii) Simulator Java SDK: ~/iotivity/service/simulator/java/sdk


2. Right click the Simulator Java SDK project -> Export -> select
'JAR file' option under Java -> Next -> Select the export destination as
ClientControllerPlugin\libs\Simulator.jar and ServiceProviderPlugin\libs\Simulator.jar -> Finish.


3. Copy the libraries libSimulatorManager.so, libRamlParser.so, liboc.so,
liboctbstack.so, and liboc_logger.so generated previously in
~/iotivity/out/linux/<arch>/release directory into the libs folder of both the plug-in projects.


4. Set the LD_LIBRARY_PATH environment variable. Right click any plug-in project -> Properties ->
Run/Debug Settings. To configure the LD_LIBRARY_PATH, there needs to be a launch configuration.
We may choose to use the existing launch configuration or create a new launch configuration.
If no launch configurations exist, then a new one has to be created.
To create a new launch configuration, select 'New' from Run/Debug settings ->
select 'Eclipse Application' as the configuration type -> OK. It opens a new window for editing
the launch configuration -> Change the launch configuration name(optional) -> Apply -> OK to close
the Edit Configuration window.

From Run/Debug settings, Choose a launch configuration and press Edit -> select 'Environment' tab
-> Click on 'Select' -> check LD_LIBRARY_PATH option -> OK. Edit the LD_LIBRARY_PATH and add the
complete path up to the libs folder of the plug-in project
( ~/iotivity/service/simulator/java/eclipse-plugin/ClientControllerPlugin/libs)
-> Apply -> OK. Then Apply -> OK to close the properties window.


5. Right click the project -> Run as Eclipse Application -> If it prompts for a configuration,
then select the one which was configured in the LD_LIBRARY_PATH in the previous step.


Note: To switch the iotivity stack for conformance, need to build the latest iotivity code and
replace liboc.so, liboctbstack.so, and liboc_logger.so native libraries in libs folder of
respective plug-in applications.
