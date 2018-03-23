/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>
#include <tinyara/init.h>
#include <stdio.h>
#include <apps/platform/cxxinitialize.h>

#include "gtest/gtest.h"

// The following lines pull in the test files.
#include "stubs/bytestream_unittest.cc"
#include "stubs/common_unittest.cc"
#include "stubs/int128_unittest.cc"
#include "stubs/once_unittest.cc"
#include "stubs/statusor_test.cc"
#include "stubs/status_test.cc"
#include "stubs/stringpiece_unittest.cc"
#include "stubs/stringprintf_unittest.cc"
#include "stubs/structurally_valid_unittest.cc"
#include "stubs/strutil_unittest.cc"
#include "stubs/template_util_unittest.cc"
#include "stubs/time_test.cc"
#include "stubs/type_traits_unittest.cc"
#include "any_test.cc"
#include "arenastring_unittest.cc"
#include "arena_unittest.cc"
#include "descriptor_database_unittest.cc"
#include "descriptor_unittest.cc"
#include "drop_unknown_fields_test.cc"
#include "dynamic_message_unittest.cc"
#include "extension_set_unittest.cc"
#include "generated_message_reflection_unittest.cc"
#include "map_field_test.cc"
#include "map_test.cc"
#include "message_unittest.cc"
#include "no_field_presence_test.cc"
#include "preserve_unknown_enum_test.cc"
#include "reflection_ops_unittest.cc"
#include "repeated_field_reflection_unittest.cc"
#include "repeated_field_unittest.cc"
#include "unknown_field_set_unittest.cc"
#include "well_known_types_unittest.cc"
#include "wire_format_unittest.cc"
#include "io/coded_stream_unittest.cc"
#include "io/printer_unittest.cc"
#include "io/tokenizer_unittest.cc"
#include "io/zero_copy_stream_unittest.cc"
#include "util/delimited_message_util_test.cc"
#include "util/field_comparator_test.cc"
#include "util/field_mask_util_test.cc"
#include "util/internal/default_value_objectwriter_test.cc"
#include "util/internal/json_objectwriter_test.cc"
#include "util/internal/json_stream_parser_test.cc"
#include "util/internal/protostream_objectsource_test.cc"
//#include "util/internal/protostream_objectwriter_test.cc"
#include "util/json_util_test.cc"
#include "util/time_util_test.cc"

/****************************************************************************
 * Name: gtest_main
 ****************************************************************************/

extern "C"
{
        void gtest_proto3_arena_lite_unittest_main(void);
        void gtest_proto3_arena_unittest_main(void);
        void gtest_proto3_lite_unittest_main(void);
        void gtest_util_tests_others_main(void);

        GTEST_API_ int gtest_main(int argc, char **argv)
        {
                printf("gtest_main()\n");
                gtest_proto3_arena_lite_unittest_main();
                gtest_proto3_arena_unittest_main();
                gtest_proto3_lite_unittest_main();
                gtest_util_tests_others_main();

                // C++ initialization should be supported to run this unittest.
                // All the tests are registered executing this function.
                up_cxxinitialize();

                testing::InitGoogleTest(&argc, argv);
                return RUN_ALL_TESTS();
        }
}
