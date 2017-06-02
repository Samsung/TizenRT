package org.iotivity.cloud.base.exception;

public class ClientException extends Exception {
    private static final long serialVersionUID = -1054509324241250664L;

    ClientException(String msg) {
        super(msg);
    }

    public static class RequesterGoneException extends ClientException {
        private static final long serialVersionUID = 6205209551209108501L;

        public RequesterGoneException(String msg) {
            super(msg);
        }
    }

    public static class BadResponseException extends ClientException {
        private static final long serialVersionUID = 6205209551209108501L;

        public BadResponseException(String msg) {
            super(msg);
        }
    }
}
