# How to manage SmartThings Things device model.  

## Importing and Exporting Device Models
To import a device model:
1. Click **Import** on the **Device/Platform** toolbar.  
![Import the JSON file](media/rt_model_import_model.png)
2. Select the JSON file containing the device model to be imported and click **OK**.  
![Select the JSON file](media/rt_model_import_model_window.png)
3. The imported device shows up in the device list with the **Custom** type.  

#### Managing Resource  
You can select, add, and restore resources:  
- To select all resources, click the checkbox on the **Resource/Property** toolbar.  
![Select all resources](media/rt_model_select_all.png)  
- To add a new resource:  
   1. Click **Add Resource**.  
   ![Add Resource](media/rt_model_add_resource.png)  
   2. In the Add Resource window, select the type of resource to be added, edit its information, and click **OK**.  
   ![Add Resource](media/rt_model_add_resource_window.png)  
   The new resource appears in the list in the **Resource/Property** section of the Model Manager main view.  
- To restore the resources, click **Restore default**. This restores the initial resource state of the device, deleting all changes that have been made.  
![The imported device shows up](media/rt_model_import_model_finished.png)  

To export a device model:  
1. Select the model to be exported and click **Export** on the **Device/Platform** toolbar.  
![Export the model](media/rt_model_export_model.png)  
2. The device model is exported as a JSON file. Enter a file name for the exported model and click **OK**.  
![Exported as a JSON file](media/rt_model_export_model_window.png)  
