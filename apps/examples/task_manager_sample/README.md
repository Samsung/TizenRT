# Task Manager Sample

## Sample Scenario:
1. The ```task_manager_sample_main()``` in the task\_manager\_sample_main.c is the entry point of the Task Manager sample.
2. First, the task\_manager\_sample\_main registers an action\_manager task and a user\_app task by using ```task_manager_register()```.
3. The task\_manager\_sample\_main requests to start the action\_manager to the Task Manager by using ```task_manager_start()```.
4. The action\_manager registers an alarm action, a LED on action, and a LED off action by using ```task_manager_register()```.
5. The action\_manager requests to start the LED on action and the LED off action sequentially by using ```task_manager_start()```.
6. The action\_manager sets a unicast handler to receive unicast messages from other applications by using ```task_manager_set_handler()```.
7. The action\_manager sets a broadcast handler to receive broadcast messages form other applications by using ```task_manager_set_broadcast_handler()```, and the msg_mask is set to ```TM_BROADCAST_WIFI_ON | TM_BROADCAST_WIFI_OFF``` (e.g., This handler processes ```TM_BROADCAST_WIFI_ON``` and ```TM_BROADCAST_WIFI_OFF``` messages)
8. The task\_manager\_sample\_main requests to start the user\_app to the Task Manager by using ```task_manager_start()```.
9. The user\_app requests the handle of action\_manager to the Task Manager in order to send a unicast message to the action\_manager by using ```task_manager_getinfo_with_name()```.
10. The user\_app sends a unicast message to the action\_manager through the Task Manager to start the alarm action by using ```task_manager_unicast()```.
11. The action\_manager requests to start the alarm action through the Task Manager by using ```task_manager_start()```.
12. Three seconds after sending the alarm action start request message, the user\_app sends a unicast message to the action\_manager through the Task Manager to pause the alarm action by using ```task_manager_unicast()```.
13. The action\_manager requests to pause the alarm action through the Task Manager by using ```task_manager_pause()```.
14. Three seconds after sending the alarm action pause request message, the user\_app sends a unicast message to the action\_manager through the Task Manager to resume the alarm action by using ```task_manager_unicast()```.
15. The action\_manager requests to resume the alarm action through the Task Manager by using ```task_manager_resume()```.
16. Three seconds after sending the alarm action resume request message, the user\_app sends a unicast message to the action\_manager through the Task Manager to restart the alarm action by using ```task_manager_unicast()```.
17. The action\_manager requests to restart the alarm action through the Task Manager by using ```task_manager_restart()```.
18. Two seconds after sending the alarm action resume request message, the user\_app sends a unicast message to the action\_manager through the Task Manager to terminate the alarm action by using ```task_manager_unicast()```.
19. The action\_manager requests to terminate the alarm action through the Task Manager by using ```task_manager_terminate()```.
20. One seconds after sending the alarm action terminate request message, the user\_app broadcasts a ```TM_BROADCAST_WIFI_ON``` message.
21. One seconds after broadcasting the ```TM_BROADCAST_WIFI_ON``` message, the user\_app broadcasts a ```TM_BROADCAST_WIFI_OFF``` message.
22. The action\_manager requests to unregister the alarm action, the LED on action, and the LED off action to the Task Manager by using ```task_manager_unregister()```.
23. The task\_manager\_sample\_main requests to unregister the action\_manager and the user\_app to the Task Manager by using ```task_manager_unregister()```.
