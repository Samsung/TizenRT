/*
 * Copyright 2017-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <tinyara/config.h>
#include <tinyara/init.h>
#include <apps/platform/cxxinitialize.h>

#include <iostream>
#include <cstdlib>
#include <string>

#include <avs-device-sdk/avs-device-sdk.h>

#include "SampleApp/SampleApplication.h"

/**
 * This serves as the starting point for the application. This code instantiates the @c UserInputManager and processes
 * user input until the @c run() function returns.
 *
 * @param argc The number of elements in the @c argv array.
 * @param argv An array of @argc elements, containing the program name and all command-line arguments.
 * @return @c EXIT_FAILURE if the program failed to initialize correctly, else @c EXIT_SUCCESS.
 */
int avsSampleAppEntry(void) {
    std::vector<std::string> configFiles;
    std::string pathToKWDInputFolder = "none";
    std::string logLevel;

    configFiles.push_back("/rom/AlexaClientSDKConfig.json");
    logLevel = "DEBUG9";

    printf("Hello1\n");

    alexaClientSDK::sampleApp::ConsolePrinter::simplePrint("Gangster!");

    auto sampleApplication =
        alexaClientSDK::sampleApp::SampleApplication::create(configFiles, pathToKWDInputFolder, logLevel);
        printf("Hello2\n");

    if (!sampleApplication) {
        alexaClientSDK::sampleApp::ConsolePrinter::simplePrint("Failed to create to SampleApplication!");
        return EXIT_FAILURE;
    }

    // This will run until the user specifies the "quit" command.
    //sampleApplication->run();

    std::cout << "Done!!" << std::endl;

    return EXIT_SUCCESS;
}