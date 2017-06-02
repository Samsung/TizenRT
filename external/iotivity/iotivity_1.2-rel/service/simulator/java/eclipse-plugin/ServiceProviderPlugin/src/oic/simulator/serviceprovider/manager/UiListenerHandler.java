/*
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
 */

package oic.simulator.serviceprovider.manager;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import oic.simulator.serviceprovider.listener.IAutomationListener;
import oic.simulator.serviceprovider.listener.IDataChangeListener;
import oic.simulator.serviceprovider.listener.IDeviceInfoListener;
import oic.simulator.serviceprovider.listener.IObserverListChangedListener;
import oic.simulator.serviceprovider.listener.IPropertiesChangedListener;
import oic.simulator.serviceprovider.listener.IResourceListChangedListener;
import oic.simulator.serviceprovider.listener.ISelectionChangedListener;
import oic.simulator.serviceprovider.model.AttributeElement;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.ResourceType;
import oic.simulator.serviceprovider.model.SingleResource;

public class UiListenerHandler {

    private static UiListenerHandler           uiHandler;

    private List<IResourceListChangedListener> resourceListChangedUIListeners;

    private List<ISelectionChangedListener>    selectionChangedUIListeners;

    private List<IAutomationListener>          automationUIListeners;

    private List<IObserverListChangedListener> observerUIListeners;

    private List<IPropertiesChangedListener>   propertiesChangedUIListeners;

    private List<IDeviceInfoListener>          deviceInfoUIListeners;

    private List<IDataChangeListener>          dataChangeListeners;

    private UiListenerHandler() {
        resourceListChangedUIListeners = new ArrayList<IResourceListChangedListener>();
        selectionChangedUIListeners = new ArrayList<ISelectionChangedListener>();
        automationUIListeners = new ArrayList<IAutomationListener>();
        observerUIListeners = new ArrayList<IObserverListChangedListener>();
        propertiesChangedUIListeners = new ArrayList<IPropertiesChangedListener>();
        deviceInfoUIListeners = new ArrayList<IDeviceInfoListener>();
        dataChangeListeners = new ArrayList<IDataChangeListener>();
    }

    public static UiListenerHandler getInstance() {
        if (null == uiHandler) {
            uiHandler = new UiListenerHandler();
        }
        return uiHandler;
    }

    public void addResourceListChangedUIListener(
            IResourceListChangedListener resourceListChangedUIListener) {
        synchronized (resourceListChangedUIListeners) {
            resourceListChangedUIListeners.add(resourceListChangedUIListener);
        }
    }

    public void addResourceSelectionChangedUIListener(
            ISelectionChangedListener resourceSelectionChangedUIListener) {
        synchronized (selectionChangedUIListeners) {
            selectionChangedUIListeners.add(resourceSelectionChangedUIListener);
        }
    }

    public void addAutomationUIListener(IAutomationListener automationUIListener) {
        synchronized (automationUIListeners) {
            automationUIListeners.add(automationUIListener);
        }
    }

    public void addObserverListChangedUIListener(
            IObserverListChangedListener observerListChangedUIListener) {
        synchronized (observerUIListeners) {
            observerUIListeners.add(observerListChangedUIListener);
        }
    }

    public void addResourcePropertiesChangedUIListener(
            IPropertiesChangedListener resourcePropertiesChangedUIListener) {
        synchronized (propertiesChangedUIListeners) {
            propertiesChangedUIListeners
                    .add(resourcePropertiesChangedUIListener);
        }
    }

    public void addDataChangeListener(IDataChangeListener dataChangeListener) {
        synchronized (dataChangeListeners) {
            dataChangeListeners.add(dataChangeListener);
        }
    }

    public void removeResourceListChangedUIListener(
            IResourceListChangedListener listener) {
        synchronized (resourceListChangedUIListeners) {
            if (null != listener && resourceListChangedUIListeners.size() > 0) {
                resourceListChangedUIListeners.remove(listener);
            }
        }
    }

    public void removeResourceSelectionChangedUIListener(
            ISelectionChangedListener listener) {
        synchronized (selectionChangedUIListeners) {
            if (null != listener && selectionChangedUIListeners.size() > 0) {
                selectionChangedUIListeners.remove(listener);
            }
        }
    }

    public void removeAutomationUIListener(IAutomationListener listener) {
        synchronized (automationUIListeners) {
            if (null != listener && automationUIListeners.size() > 0) {
                automationUIListeners.remove(listener);
            }
        }
    }

    public void removeObserverListChangedUIListener(
            IObserverListChangedListener listener) {
        synchronized (observerUIListeners) {
            if (null != listener && observerUIListeners.size() > 0) {
                observerUIListeners.remove(listener);
            }
        }
    }

    public void removeResourcePropertiesChangedUIListener(
            IPropertiesChangedListener resourcePropertiesChangedUIListener) {
        synchronized (propertiesChangedUIListeners) {
            propertiesChangedUIListeners
                    .remove(resourcePropertiesChangedUIListener);
        }
    }

    public void removeDataChangeListener(IDataChangeListener dataChangeListener) {
        synchronized (dataChangeListeners) {
            dataChangeListeners.remove(dataChangeListener);
        }
    }

    public void addDeviceInfoUIListener(IDeviceInfoListener deviceUIListener) {
        synchronized (deviceInfoUIListeners) {
            deviceInfoUIListeners.add(deviceUIListener);
        }
    }

    public void removeDeviceInfoUIListener(
            IDeviceInfoListener platformUIListener) {
        synchronized (deviceInfoUIListeners) {
            deviceInfoUIListeners.remove(platformUIListener);
        }
    }

    public void resourceCreatedUINotification(ResourceType type) {
        synchronized (resourceListChangedUIListeners) {
            if (resourceListChangedUIListeners.size() > 0) {
                IResourceListChangedListener listener;
                Iterator<IResourceListChangedListener> listenerItr = resourceListChangedUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onResourceCreation(type);
                    }
                }
            }
        }
    }

    public void resourceDeletedUINotification(ResourceType type) {
        synchronized (resourceListChangedUIListeners) {
            if (resourceListChangedUIListeners.size() > 0) {
                IResourceListChangedListener listener;
                Iterator<IResourceListChangedListener> listenerItr = resourceListChangedUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onResourceDeletion(type);
                    }
                }
            }
        }
    }

    public void resourceListUpdateUINotification(ResourceType type) {
        synchronized (resourceListChangedUIListeners) {
            if (resourceListChangedUIListeners.size() > 0) {
                IResourceListChangedListener listener;
                Iterator<IResourceListChangedListener> listenerItr = resourceListChangedUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onResourceListUpdate(type);
                    }
                }
            }
        }
    }

    public void resourceSelectionChangedUINotification(Resource resource) {
        synchronized (selectionChangedUIListeners) {
            if (selectionChangedUIListeners.size() > 0) {
                ISelectionChangedListener listener;
                Iterator<ISelectionChangedListener> listenerItr = selectionChangedUIListeners
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

    public void attributeUpdatedUINotification(AttributeElement attribute) {
        synchronized (dataChangeListeners) {
            if (dataChangeListeners.size() > 0 && null != attribute) {
                IDataChangeListener listener;
                Iterator<IDataChangeListener> listenerItr = dataChangeListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.update(attribute);
                    }
                }
            }
        }
    }

    public void attributeAddedUINotification(AttributeElement attribute) {
        synchronized (dataChangeListeners) {
            if (dataChangeListeners.size() > 0 && null != attribute) {
                IDataChangeListener listener;
                Iterator<IDataChangeListener> listenerItr = dataChangeListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.add(attribute);
                    }
                }
            }
        }
    }

    public void attributeRemovedUINotification(AttributeElement attribute) {
        synchronized (dataChangeListeners) {
            if (dataChangeListeners.size() > 0 && null != attribute) {
                IDataChangeListener listener;
                Iterator<IDataChangeListener> listenerItr = dataChangeListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.remove(attribute);
                    }
                }
            }
        }
    }

    public void resourceAutomationStartedUINotification(SingleResource resource) {
        synchronized (automationUIListeners) {
            if (automationUIListeners.size() > 0 && null != resource) {
                IAutomationListener listener;
                Iterator<IAutomationListener> listenerItr = automationUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onResourceAutomationStart(resource);
                    }
                }
            }
        }
    }

    public void automationCompleteUINotification(SingleResource resource,
            String attName) {
        synchronized (automationUIListeners) {
            if (automationUIListeners.size() > 0 && null != resource) {
                IAutomationListener listener;
                Iterator<IAutomationListener> listenerItr = automationUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onAutomationComplete(resource, attName);
                    }
                }
            }
        }
    }

    public void observerListChangedUINotification(Resource resource) {
        synchronized (observerUIListeners) {
            if (observerUIListeners.size() > 0 && null != resource) {
                IObserverListChangedListener listener;
                Iterator<IObserverListChangedListener> listenerItr = observerUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        listener.onObserverListChanged(resource);
                    }
                }
            }
        }
    }

    public void propertiesChangedUINotification(Class<?> targetClass) {
        synchronized (propertiesChangedUIListeners) {
            if (propertiesChangedUIListeners.size() > 0) {
                IPropertiesChangedListener listener;
                Iterator<IPropertiesChangedListener> listenerItr = propertiesChangedUIListeners
                        .iterator();
                while (listenerItr.hasNext()) {
                    listener = listenerItr.next();
                    if (null != listener) {
                        if (targetClass == Resource.class) {
                            listener.onResourcePropertyChange();
                        }
                    }
                }
            }
        }
    }

    public void deviceInfoReceivedNotification() {
        synchronized (deviceInfoUIListeners) {
            if (deviceInfoUIListeners.size() > 0) {
                IDeviceInfoListener listener;
                Iterator<IDeviceInfoListener> listenerItr = deviceInfoUIListeners
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
}
