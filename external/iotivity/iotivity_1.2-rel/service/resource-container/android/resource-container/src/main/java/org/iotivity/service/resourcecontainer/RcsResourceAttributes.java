//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


package org.iotivity.service.resourcecontainer;

import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 *
 * This class represents the attributes for a resource.
 *
 * @see RcsValue
 */
public final class RcsResourceAttributes 
{

    private final Map<String, RcsValue> mCache = new HashMap<>();

    public RcsResourceAttributes() {
    }

    public RcsResourceAttributes(RcsResourceAttributes attrs){
        for (final String key : attrs.keySet()) {
            mCache.put(key, attrs.get(key));
        }
    }

    /**
     * Returns a unmodifiable Set view of the keys contained in this attributes.
     *
     * @return an unmodifiable set view of the keys in this attributes
     */
    public Set<String> keySet() {
        return Collections.unmodifiableSet(mCache.keySet());
    }

    /**
     * Returns the value to which the specified key is mapped, or null if this
     * contains no mapping for the key.
     *
     * @param key
     *            the key whose associated value is to be returned
     *
     * @return the value to which the specified key is mapped, or null if this
     *         contains no mapping for the key
     *
     * @throws NullPointerException
     *             if key is null
     */
    public RcsValue get(String key) {
        if (key == null) throw new NullPointerException("key is null");
        
        return mCache.get(key);
    }

    /**
     * Sets the specified value with the specified key.
     * If the object previously contained a mapping for the key, the old value
     * is replaced by the specified value.
     *
     * @param key
     *            key with which the specified value is to be associated
     *
     * @param value
     *            value to be associated with the specified key
     *
     * @throws NullPointerException
     *             if key or value is null
     *
     */
    public void put(String key, RcsValue value) {
        if (key == null) throw new NullPointerException("key is null");
        if (value == null) throw new NullPointerException("value is null");

        mCache.put(key, value);
    }

    /**
     * Sets the specified value with the specified key.
     * If the object previously contained a mapping for the key, the old value
     * is replaced by the specified value.
     *
     * @param key
     *            key with which the specified value is to be associated
     *
     * @param value
     *            value to be associated with the specified key
     *
     * @throws NullPointerException
     *             if key or value is null
     * @throws IllegalArgumentException
     *             if object is not supported type by {@link RcsValue}
     */
    public void put(String key, Object object) {
        if (key == null) throw new NullPointerException("key is null");

        put(key, new RcsValue(object));
    }

    /**
     * Returns true if this contains no key-value mappings.
     *
     * @return true if this contains no key-value mappings
     */
    public boolean isEmpty() {
        return mCache.isEmpty();
    }

    /**
     * Returns the number of key-value mappings.
     *
     * @return the number of key-value mappings
     */
    public int size() {
        return mCache.size();
    }

    /**
     * Removes the mapping for a key from this attributes if it is present.
     *
     * @param key
     *            key whose mapping is to be removed
     *
     * @return true if the key is present and the the value mapped is removed.
     */
    public boolean remove(String key) {
        if (key == null) throw new NullPointerException("key is null");
      
        final boolean cacheRemove = mCache.remove(key) != null;      

        return cacheRemove;
    }

    /**
     * Removes all of the mappings.
     */
    public void clear(){
        mCache.clear();
    }

    /**
     * Returns true if this contains a mapping for the specified key.
     *
     * @param key
     *            key whose presence is to be tested
     *
     * @return true if this contains a mapping for the specified key.
     *
     * @throws NullPointerException
     *             if key is null
     */
    public boolean contains(String key) {
        if (key == null) throw new NullPointerException("key is null");

        return mCache.containsKey(key);
    }
    

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof RcsResourceAttributes)) return false;        
        RcsResourceAttributes rhs = (RcsResourceAttributes) o;
        return mCache.equals(rhs.mCache);
    }

    @Override
    public int hashCode() {
        return mCache.hashCode();
    }
    
    /**
     * Updates all properties provided as parameter.
     */
    public void put(RcsResourceAttributes attrs){
        for (final String key : attrs.keySet()) {
            mCache.put(key, attrs.get(key));
        }
    }
}
