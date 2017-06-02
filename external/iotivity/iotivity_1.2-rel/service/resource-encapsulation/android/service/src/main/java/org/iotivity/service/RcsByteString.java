package org.iotivity.service;

/**
 * This Class represents byte string value for RcsResourceAttributes.
 */
public class RcsByteString {

    private byte[] mData;
    private long mSize;

    public RcsByteString(byte[] data, long size) {
        this.mData = data;
        this.mSize =  size;
    }

    public byte[] getValue() {
        return this.mData;
    }

    public long getSize() {
        return this.mSize;
    }
}