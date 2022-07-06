package com.example.blebandwidth;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.TextView;

import java.util.HashMap;
import java.util.List;

public class ServiceListAdapter extends BaseExpandableListAdapter {
    private Context m_context;
    private List<String> m_ServiceGroupList;
    private HashMap<String, List<ChildInfo>> m_ServiceGrpHash;

    ServiceListAdapter(Context context, List<String> ServiceGroupList,
                       HashMap<String, List<ChildInfo>> ServiceGrpHash) {

        m_context = context;
        m_ServiceGroupList = ServiceGroupList;
        m_ServiceGrpHash = ServiceGrpHash;
    }

    @Override
    public Object getChild(int groupPosition, int childPosition) {
        return m_ServiceGrpHash.get(m_ServiceGroupList.get(groupPosition)).get(childPosition);
    }

    @Override
    public long getChildId(int groupPosition, int childPosition) {
        return childPosition;
    }

    @Override
    public View getChildView(int groupPosition, int childPosition, boolean isLastChild,
                             View view, ViewGroup parent) {
        if (view == null) {
            LayoutInflater infalInflater = (LayoutInflater) m_context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = infalInflater.inflate(R.layout.list_child, null);
        }

        ChildInfo child_info = (ChildInfo)getChild(groupPosition, childPosition);
        String char_str = child_info.get_char_uuid() + "\n" + child_info.get_char_property();
        TextView childItem = (TextView) view.findViewById(R.id.childItem);
        childItem.setText(char_str);

        return view;
    }

    @Override
    public int getChildrenCount(int groupPosition) {
        return m_ServiceGrpHash.get(m_ServiceGroupList.get(groupPosition)).size();
    }

    @Override
    public Object getGroup(int groupPosition) {
        return m_ServiceGroupList.get(groupPosition);
    }

    @Override
    public int getGroupCount() {
        return m_ServiceGroupList.size();
    }

    @Override
    public long getGroupId(int groupPosition) {
        return groupPosition;
    }

    @Override
    public View getGroupView(int groupPosition, boolean isLastChild, View view,
                             ViewGroup parent) {
        if (view == null) {
            LayoutInflater infalInflater = (LayoutInflater) m_context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = infalInflater.inflate(R.layout.list_group, null);
        }

        String ServiceGroupItem = (String)getGroup(groupPosition);

        TextView ServiceGroup = (TextView) view.findViewById(R.id.ServiceGroup);
        ServiceGroup.setText(ServiceGroupItem);

        return view;
    }

    @Override
    public boolean hasStableIds() {
        return false;
    }

    @Override
    public boolean isChildSelectable(int groupPosition, int childPosition) {
        return true;
    }
}