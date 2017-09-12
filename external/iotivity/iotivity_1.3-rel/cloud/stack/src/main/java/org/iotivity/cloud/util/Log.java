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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.net.URI;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import org.apache.log4j.Logger;

import io.netty.channel.Channel;

public class Log {

    public static final int         VERBOSE        = 0;
    public static final int         DEBUG          = 1;
    public static final int         INFO           = 2;
    public static final int         WARNING        = 3;
    public static final int         ERROR          = 4;

    private static int              logLevel       = VERBOSE;

    private static FileOutputStream fos            = null;
    private static PrintStream      ps             = null;

    private final static Logger     logger         = Logger
            .getLogger(Log.class);
    private static WebsocketLog     websocketpoint = null;
    private static String           mServerName    = null;

    public static void Init() throws FileNotFoundException {
        System.setOut(Log.createLoggingProxy(System.out));
        createfile();
    }

    public static void InitWebLog(String webLogHost, String serverName) {
        mServerName = serverName;
        if (webLogHost != null) {
            try {
                websocketpoint = new WebsocketLog(new URI("ws://" + webLogHost));
                websocketpoint.start();
            } catch (Exception e) {
                System.out.println(e.getMessage());
                websocketpoint = null;
            }
        }
    }

    public static PrintStream createLoggingProxy(
            final PrintStream realPrintStream) {
        // TODO Auto-generated method stub
        return new PrintStream(realPrintStream) {
            public void print(final String string) {
                realPrintStream.print(string);
                logger.fatal(string);
            }
        };
    }

    public static void createfile() throws FileNotFoundException {
        File dir = new File("..//errLog//");
        if (!dir.isDirectory()) {
            dir.mkdirs();
        }
        fos = new FileOutputStream("../errLog/[" + getDate() + "] error.log",
                true);
        ps = new PrintStream(fos);
    }

    static public void f(Channel channel, Throwable t) {
        String log = channel.id().asLongText().substring(26) + " "
                + t.getMessage();
        ps.println(getTime() + " " + log);
        t.printStackTrace(ps);
        Log.v(log);
    }

    public static void setLogLevel(int level) {
        logLevel = level;
    }

    public static void v(String log) {
        printLog(VERBOSE, log);
    }

    public static void d(String log) {
        printLog(DEBUG, log);
    }

    public static void i(String log) {
        printLog(INFO, log);
    }

    public static void w(String log) {
        printLog(WARNING, log);
    }

    public static void e(String log) {
        printLog(ERROR, log);
    }

    public static void w(String log, Exception ex) {
        printLog(WARNING, log);
        ex.printStackTrace();
    }

    public static void w(String log, Throwable th) {
        printLog(WARNING, log);
        th.printStackTrace();
    }

    public static void e(String log, Exception ex) {
        printLog(ERROR, log);
        ex.printStackTrace();
    }

    public static void e(String log, Throwable th) {
        printLog(ERROR, log);
        th.printStackTrace();
    }

    private static void printLog(int level, String log) {

        if (logLevel > level)
            return;

        String format = "";
        format += getTime();
        format += " [" + getLogLevelString(level) + "]";

        if (level >= ERROR) {
            format += " [" + getDetailInfo() + "]";
        }

        format += " " + log;

        System.out.println(format);

        if (websocketpoint != null && websocketpoint.getUserSession() != null) {
            websocketpoint.sendMessage("[" + mServerName + "]" + format);
        }
    }

    private static String getDetailInfo() {

        String res = "";

        StackTraceElement ste = Thread.currentThread().getStackTrace()[4];
        String className = ste.getClassName()
                .substring(ste.getClassName().lastIndexOf(".") + 1);

        res += ste.getFileName() + ", " + className + "." + ste.getMethodName()
                + "(), line:" + ste.getLineNumber();

        return res;
    }

    protected static String getTime() {
        Calendar calendar = Calendar.getInstance();
        SimpleDateFormat dateFormat = new SimpleDateFormat(
                "yyyy-MM-dd HH:mm:ss:SSS");

        return dateFormat.format(calendar.getTime());
    }

    protected static String getDate() {
        Calendar calendar = Calendar.getInstance();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
        return dateFormat.format(calendar.getTime());
    }

    private static String getLogLevelString(int level) {

        String res = "";

        if (level == VERBOSE) {
            res = "V";
        } else if (level == DEBUG) {
            res = "D";
        } else if (level == INFO) {
            res = "I";
        } else if (level == WARNING) {
            res = "W";
        } else if (level == ERROR) {
            res = "E";
        }

        return res;
    }
}
