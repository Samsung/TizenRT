package com.example.blebandwidth;

public class ChildInfo {
    private String m_CharUUID = "";
    private String m_property;

    ChildInfo(String char_uuid, String property) {
        m_CharUUID = char_uuid;
        m_property = property;
    }

    public String get_char_uuid() {
        return m_CharUUID;
    }

    public String get_char_property() {
        return m_property;
    }
    public void set_char_uuid(String char_uuid) {
        m_CharUUID = char_uuid;
    }
}