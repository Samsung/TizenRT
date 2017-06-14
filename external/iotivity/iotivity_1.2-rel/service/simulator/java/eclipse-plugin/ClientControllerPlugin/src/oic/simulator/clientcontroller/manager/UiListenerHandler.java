package oic.simulator.clientcontroller.manager;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.oic.simulator.client.SimulatorRemoteResource.RequestType;

import oic.simulator.clientcontroller.listener.IConfigurationUpload;
import oic.simulator.clientcontroller.listener.IDevicePlatformInfoUIListener;
import oic.simulator.clientcontroller.listener.IFindResourceUIListener;
import oic.simulator.clientcontroller.listener.IGetUIListener;
import oic.simulator.clientcontroller.listener.IObserveUIListener;
import oic.simulator.clientcontroller.listener.IPostUIListener;
import oic.simulator.clientcontroller.listener.IPutUIListener;
import oic.simulator.clientcontroller.listener.IResourceSelectionChangedUIListener;
import oic.simulator.clientcontroller.listener.IVerificationUIListener;
import oic.simulator.clientcontroller.remoteresource.RemoteResource;

public class UiListenerHandler {
    private static UiListenerHandler                  uiHandler;

    private List<IFindResourceUIListener>             findResourceUIListeners;
    private List<IResourceSelectionChangedUIListener> resourceSelectionChangedUIListeners;
    private List<IGetUIListener>                      getUIListeners;
    private List<IPutUIListener>                      putUIListeners;
    private List<IPostUIListener>                     postUIListeners;
    private List<IObserveUIListener>                  observeUIListeners;
    private List<IVerificationUIListener>             verificationUIListeners;
    private List<IConfigurationUpload>                configUploadUIListeners;
    private List<IDevicePlatformInfoUIListener>       devicePlatformInfoUIListeners;

    private UiListenerHandler() {
        findResourceUIListeners = new ArrayList<IFindResourceUIListener>();
        resourceSelectionChangedUIListeners = new ArrayList<IResourceSelectionChangedUIListener>();
        getUIListeners = new ArrayList<IGetUIListener>();
        putUIListeners = new ArrayList<IPutUIListener>();
        postUIListeners = new ArrayList<IPostUIListener>();
        observeUIListeners = new ArrayList<IObserveUIListener>();
        verificationUIListeners = new ArrayList<IVerificationUIListener>();
        configUploadUIListeners = new ArrayList<IConfigurationUpload>();
        devicePlatformInfoUIListeners = new ArrayList<IDevicePlatformInfoUIListener>();
    }

    public static UiListenerHandler getInstance() {
        if (null == uiHandler) {
            uiHandler = new UiListenerHandler();
        }
        return uiHandler;
    }

    public void addResourceSelectionChangedUIListener(
            IResourceSelectionChangedUIListener resourceSelectionChangedUIListener) {
        synchronized (resourceSelectionChangedUIListeners) {
            resourceSelectionChangedUIListeners
                    .add(resourceSelectionChangedUIListener);
        }
    }

    public void addGetUIListener(IGetUIListener getUIListener) {
        synchronized (getUIListeners) {
            getUIListeners.add(getUIListener);
        }
    }

    public void addPutUIListener(IPutUIListener putUIListener) {
        synchronized (putUIListeners) {
            putUIListeners.add(putUIListener);
        }
    }

    public void addPostUIListener(IPostUIListener postUIListener) {
        synchronized (postUIListeners) {
            postUIListeners.add(postUIListener);
        }
    }

    public void addObserveUIListener(IObserveUIListener observeUIListener) {
        synchronized (observeUIListeners) {
            observeUIListeners.add(observeUIListener);
        }
    }

    public void addVerificationUIListener(
            IVerificationUIListener verificationUIListener) {
        synchronized (verificationUIListeners) {
            verificationUIListeners.add(verificationUIListener);
        }
    }

    public void addConfigUploadUIListener(IConfigurationUpload configListener) {
        synchronized (configUploadUIListeners) {
            configUploadUIListeners.add(configListener);
        }
    }

    public void addDevicePlatformInfoUIListener(
            IDevicePlatformInfoUIListener deviceUIListener) {
        synchronized (devicePlatformInfoUIListeners) {
            devicePlatformInfoUIListeners.add(deviceUIListener);
        }
    }

    public void removeDevicePlatformInfoUIListener(
            IDevicePlatformInfoUIListener platformUIListener) {
        synchronized (devicePlatformInfoUIListeners) {
            devicePlatformInfoUIListeners.remove(platformUIListener);
        }
    }

    public void removeResourceSelectionChangedUIListener(
            IResourceSelectionChangedUIListener listener) {
        synchronized (resourceSelectionChangedUIListeners) {
            if (null != listener
                    && resourceSelectionChangedUIListeners.size() > 0) {
                resourceSelectionChangedUIListeners.remove(listener);
            }
        }
    }

    public void removeGetUIListener(IGetUIListener getUIListener) {
        synchronized (getUIListeners) {
            getUIListeners.remove(getUIListener);
        }
    }

    public void removePutUIListener(IPutUIListener putUIListener) {
        synchronized (putUIListeners) {
            putUIListeners.remove(putUIListener);
        }
    }

    public void removePostUIListener(IPostUIListener postUIListener) {
        synchronized (postUIListeners) {
            postUIListeners.remove(postUIListener);
        }
    }

    public void removeObserveUIListener(IObserveUIListener observeUIListener) {
        synchronized (observeUIListeners) {
            observeUIListeners.remove(observeUIListener);
        }
    }

    public void removeVerificationUIListener(
            IVerificationUIListener verificationUIListener) {
        synchronized (verificationUIListeners) {
            verificationUIListeners.remove(verificationUIListener);
        }
    }

    public void removeConfigUploadUIListener(IConfigurationUpload configListener) {
        synchronized (configUploadUIListeners) {
            configUploadUIListeners.remove(configListener);
        }
    }

    public void addFindresourceUIListener(IFindResourceUIListener listener) {
        if (null == listener) {
            return;
        }
        synchronized (findResourceUIListeners) {
            findResourceUIListeners.add(listener);
        }
    }

    public void removeFindresourceUIListener(IFindResourceUIListener listener) {
        if (null == listener) {
            return;
        }
        synchronized (findResourceUIListeners) {
            findResourceUIListeners.remove(listener);
        }
    }

    public void newResourceFoundNotification(RemoteResource resource) {
        synchronized (findResourceUIListeners) {
            if (findResourceUIListeners.size() > 0) {
                IFindResourceUIListener listener;
                Iterator<IFindResourceUIListener> listenerItr = findResourceUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onNewResourceFound(resource);
                    }
                }
            }
        }
    }

    public void resourceSelectionChangedUINotification(RemoteResource resource) {
        synchronized (resourceSelectionChangedUIListeners) {
            if (resourceSelectionChangedUIListeners.size() > 0) {
                IResourceSelectionChangedUIListener listener;
                Iterator<IResourceSelectionChangedUIListener> listenerItr = resourceSelectionChangedUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onResourceSelectionChange(resource);
                    }
                }
            }
        }
    }

    public void getCompleteNotification(RemoteResource resource) {
        synchronized (getUIListeners) {
            if (getUIListeners.size() > 0) {
                IGetUIListener listener;
                Iterator<IGetUIListener> listenerItr = getUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onGetCompleted(resource);
                    }
                }
            }
        }
    }

    public void putCompleteNotification(RemoteResource resource) {
        synchronized (putUIListeners) {
            if (putUIListeners.size() > 0) {
                IPutUIListener listener;
                Iterator<IPutUIListener> listenerItr = putUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onPutCompleted(resource);
                    }
                }
            }
        }
    }

    public void postCompleteNotification(RemoteResource resource) {
        synchronized (postUIListeners) {
            if (postUIListeners.size() > 0) {
                IPostUIListener listener;
                Iterator<IPostUIListener> listenerItr = postUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onPostCompleted(resource);
                    }
                }
            }
        }
    }

    public void observeCompleteNotification(RemoteResource resource) {
        synchronized (observeUIListeners) {
            if (observeUIListeners.size() > 0) {
                IObserveUIListener listener;
                Iterator<IObserveUIListener> listenerItr = observeUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onObserveCompleted(resource);
                    }
                }
            }
        }
    }

    public void verificationStartedNotification(RemoteResource resource,
            RequestType reqType) {
        synchronized (verificationUIListeners) {
            if (verificationUIListeners.size() > 0) {
                IVerificationUIListener listener;
                Iterator<IVerificationUIListener> listenerItr = verificationUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onVerificationStarted(resource, reqType);
                    }
                }
            }
        }
    }

    public void verificationAbortedNotification(RemoteResource resource,
            RequestType reqType) {
        synchronized (verificationUIListeners) {
            if (verificationUIListeners.size() > 0) {
                IVerificationUIListener listener;
                Iterator<IVerificationUIListener> listenerItr = verificationUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onVerificationAborted(resource, reqType);
                    }
                }
            }
        }
    }

    public void verificationCompletedNotification(RemoteResource resource,
            RequestType reqType) {
        synchronized (verificationUIListeners) {
            if (verificationUIListeners.size() > 0) {
                IVerificationUIListener listener;
                Iterator<IVerificationUIListener> listenerItr = verificationUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onVerificationCompleted(resource, reqType);
                    }
                }
            }
        }
    }

    public void configUploadedNotification(RemoteResource resource) {
        synchronized (configUploadUIListeners) {
            if (configUploadUIListeners.size() > 0) {
                IConfigurationUpload listener;
                Iterator<IConfigurationUpload> listenerItr = configUploadUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onConfigurationUploaded(resource);
                    }
                }
            }
        }
    }

    public void deviceInfoReceivedNotification() {
        synchronized (devicePlatformInfoUIListeners) {
            if (devicePlatformInfoUIListeners.size() > 0) {
                IDevicePlatformInfoUIListener listener;
                Iterator<IDevicePlatformInfoUIListener> listenerItr = devicePlatformInfoUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onDeviceInfoFound();
                    }
                }
            }
        }
    }

    public void platformInfoReceivedNotification() {
        synchronized (devicePlatformInfoUIListeners) {
            if (devicePlatformInfoUIListeners.size() > 0) {
                IDevicePlatformInfoUIListener listener;
                Iterator<IDevicePlatformInfoUIListener> listenerItr = devicePlatformInfoUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onPlatformInfoFound();
                    }
                }
            }
        }
    }
}
