package org.iotivity.cloud.base.exception;

import org.iotivity.cloud.base.protocols.enums.ResponseStatus;

public class ServerException extends RuntimeException {
    private static final long serialVersionUID = -3650388853103279698L;
    private ResponseStatus    mErrorResponse   = ResponseStatus.INTERNAL_SERVER_ERROR;

    public ServerException(ResponseStatus errorResponse) {
        super(errorResponse.toString());
        mErrorResponse = errorResponse;
    }

    public ServerException(ResponseStatus errorResponse, String msg) {
        super(errorResponse.toString() + ", " + msg);
        mErrorResponse = errorResponse;
    }

    public ResponseStatus getErrorResponse() {
        return mErrorResponse;
    }

    public static class BadRequestException extends ServerException {
        private static final long serialVersionUID = -7903463045263420748L;

        public BadRequestException() {
            super(ResponseStatus.BAD_REQUEST);
        }

        public BadRequestException(String msg) {
            super(ResponseStatus.BAD_REQUEST, msg);
        }
    }

    public static class BadOptionException extends ServerException {
        private static final long serialVersionUID = -1133293352997486233L;

        public BadOptionException() {
            super(ResponseStatus.BAD_OPTION);
        }

        public BadOptionException(String msg) {
            super(ResponseStatus.BAD_OPTION, msg);
        }
    }

    public static class NotFoundException extends ServerException {
        private static final long serialVersionUID = 775328915430229701L;

        public NotFoundException() {
            super(ResponseStatus.NOT_FOUND);
        }

        public NotFoundException(String msg) {
            super(ResponseStatus.NOT_FOUND, msg);
        }
    }

    public static class UnAuthorizedException extends ServerException {
        private static final long serialVersionUID = -1413182067769946759L;

        public UnAuthorizedException() {
            super(ResponseStatus.UNAUTHORIZED);
        }

        public UnAuthorizedException(String msg) {
            super(ResponseStatus.UNAUTHORIZED, msg);
        }
    }

    public static class InternalServerErrorException extends ServerException {
        private static final long serialVersionUID = -4873009823652617325L;

        public InternalServerErrorException() {
            super(ResponseStatus.INTERNAL_SERVER_ERROR);
        }

        public InternalServerErrorException(String msg) {
            super(ResponseStatus.INTERNAL_SERVER_ERROR, msg);
        }
    }

    public static class MethodNotAllowedException extends ServerException {
        private static final long serialVersionUID = -3491771205478082847L;

        public MethodNotAllowedException() {
            super(ResponseStatus.METHOD_NOT_ALLOWED);
        }

        public MethodNotAllowedException(String msg) {
            super(ResponseStatus.METHOD_NOT_ALLOWED, msg);
        }
    }

    public static class PreconditionFailedException extends ServerException {
        private static final long serialVersionUID = -4139595328143251734L;

        public PreconditionFailedException() {
            super(ResponseStatus.PRECONDITION_FAILED);
        }

        public PreconditionFailedException(String msg) {
            super(ResponseStatus.PRECONDITION_FAILED, msg);
        }
    }

    public static class ForbiddenException extends ServerException {
        private static final long serialVersionUID = -2217447793288515253L;

        public ForbiddenException() {
            super(ResponseStatus.FORBIDDEN);
        }

        public ForbiddenException(String msg) {
            super(ResponseStatus.FORBIDDEN, msg);
        }
    }

    public static class NotImplementedException extends ServerException {
        private static final long serialVersionUID = 5699911106834702025L;

        public NotImplementedException() {
            super(ResponseStatus.NOT_IMPLEMENTED);
        }

        public NotImplementedException(String msg) {
            super(ResponseStatus.NOT_IMPLEMENTED, msg);
        }
    }
}
