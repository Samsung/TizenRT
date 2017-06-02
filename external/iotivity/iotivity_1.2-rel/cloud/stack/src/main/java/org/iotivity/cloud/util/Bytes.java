/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
package org.iotivity.cloud.util;

import java.nio.ByteBuffer;

public class Bytes {
    public static byte[] longTo8Bytes(long l) {
        ByteBuffer buffer = ByteBuffer.allocate(8);
        buffer.putLong(l);
        return buffer.array();
    }

    public static long bytesToLong(byte[] bytes) {
        ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
        if (bytes.length < Long.BYTES) {
            buffer.put(new byte[Long.BYTES - bytes.length]);
        }
        buffer.put(bytes);
        buffer.flip();
        return buffer.getLong();
    }

    public static int bytesToInt(byte[] bytes) {
        ByteBuffer buffer = ByteBuffer.allocate(Integer.BYTES);
        if (bytes.length < Integer.BYTES) {
            buffer.put(new byte[Integer.BYTES - bytes.length]);
        }
        buffer.put(bytes);
        buffer.flip();
        return buffer.getInt();
    }

    public static byte[] intToMax4Bytes(int i) {
        ByteBuffer buffer = ByteBuffer.allocate(4);
        buffer.putInt(i);
        buffer.flip();
        if (buffer.get(0) != 0) {
            return buffer.array();
        }

        if (buffer.get(1) != 0) {
            byte b[] = new byte[3];
            buffer.get();
            buffer.get(b);
            return b;
        }

        if (buffer.get(2) != 0) {
            byte b[] = new byte[2];
            buffer.get();
            buffer.get();
            buffer.get(b);
            return b;
        }

        return new byte[] { buffer.get(3) };
    }
}