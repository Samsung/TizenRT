/* *****************************************************************
 *
 * Copyright 2016 Microsoft
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include <windows.h>
#include <gtest/gtest.h>
#include "vs12_snprintf.h"

void OutputBufferLargerThanNeeded(size_t outputBufferSize, size_t inputBufferSize, int callerLineNumber)
{
    ASSERT_GT(outputBufferSize, inputBufferSize);
    ASSERT_GT(inputBufferSize, 1U);

    char *inputBuffer = new char[inputBufferSize];
    memset(inputBuffer, 'i', inputBufferSize - 1);
    inputBuffer[inputBufferSize - 1] = '\0';

    char *outputBuffer = new char[outputBufferSize];
    memset(outputBuffer, 'o', outputBufferSize);

    EXPECT_EQ((inputBufferSize - 1),
              vs12_snprintf(outputBuffer, outputBufferSize, "%s", inputBuffer))
              << "caller line number: " << callerLineNumber;
    EXPECT_STREQ(inputBuffer, outputBuffer) << "caller line number: " << callerLineNumber;

    delete[] inputBuffer;
    delete[] outputBuffer;
}

void OutputBufferSmallerThanNeeded(size_t outputBufferSize, size_t inputBufferSize, int callerLineNumber)
{
    ASSERT_GT(inputBufferSize, outputBufferSize);
    ASSERT_GT(outputBufferSize, 1U);

    char *inputBuffer = new char[inputBufferSize];
    memset(inputBuffer, 'i', inputBufferSize - 1);
    inputBuffer[inputBufferSize - 1] = '\0';

    char *outputBuffer = new char[outputBufferSize];
    memset(outputBuffer, 'o', outputBufferSize);

    if (outputBufferSize == (inputBufferSize - 1))
    {
        // When the output buffer size is just off by 1, the return value of
        // vs12_snprintf is C99-compliant.
        EXPECT_EQ((inputBufferSize - 1),
                   vs12_snprintf(outputBuffer, outputBufferSize, "%s", inputBuffer))
                   << "caller line number: " << callerLineNumber;
    }
    else
    {
        // vs12_snprintf returns -1 in this case (a non C99-compliant value).
        EXPECT_EQ(-1,
                  vs12_snprintf(outputBuffer, outputBufferSize, "%s", inputBuffer))
                  << "caller line number: " << callerLineNumber;
    }

    // snprintf output should be zero-terminated, even when the output buffer was too small.
    EXPECT_EQ('\0', outputBuffer[outputBufferSize - 1]) << "caller line number: " << callerLineNumber;

    // Truncate the input buffer, then compare it with the output buffer.
    inputBuffer[outputBufferSize - 1] = '\0';
    EXPECT_STREQ(inputBuffer, outputBuffer) << "caller line number: " << callerLineNumber;

    delete[] inputBuffer;
    delete[] outputBuffer;
}

TEST(vs12_snprintfTest, OutputBufferLargerThanNeeded1)
{
    OutputBufferLargerThanNeeded(10, 10 - 1, __LINE__);
}

TEST(vs12_snprintfTest, OutputBufferLargerThanNeeded2)
{
    OutputBufferLargerThanNeeded(123, 123 - 2, __LINE__);
}

TEST(vs12_snprintfTest, OutputBufferAsLargeAsNeeded)
{
    char outputBuffer[54];
    char inputBuffer[sizeof(outputBuffer)];

    memset(inputBuffer, 'i', sizeof(inputBuffer) - 1);
    inputBuffer[sizeof(inputBuffer) - 1] = '\0';

    memset(outputBuffer, 'o', sizeof(outputBuffer));

    EXPECT_EQ((sizeof(inputBuffer) - 1),
               vs12_snprintf(outputBuffer, sizeof(outputBuffer), "%s", inputBuffer));
    EXPECT_STREQ(inputBuffer, outputBuffer);
}

TEST(vs12_snprintfTest, OutputBufferSmallerThanNeeded1)
{
    OutputBufferSmallerThanNeeded(100, 100 + 1, __LINE__);
}

TEST(vs12_snprintfTest, OutputBufferSmallerThanNeeded2)
{
    OutputBufferSmallerThanNeeded(123, 123 + 2, __LINE__);
}

TEST(vs12_snprintfTest, OutputBufferSmallerThanNeeded3)
{
    OutputBufferSmallerThanNeeded(432, 2 * 432, __LINE__);
}
