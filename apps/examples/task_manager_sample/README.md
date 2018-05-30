# Task Manager Sample

## Sample Scenario:
1. The ```task_manager_sample_main()``` in the task\_manager\_sample_main.c is the entry point of the task manager sample.
2. First, the task\_manager\_sample\_main registers an action\_manager task and a user\_app task by using ```task_manager_register()```.
3. The task\_manager\_sample\_main requests to start the action\_manager to the task manager by using ```task_manager_start()```.
4. The action\_manager registers an alarm action, a LED on action, and a LED off action by using ```task_manager_register()```.
5. The action\_manager requests to start the LED on action and the LED off action sequentially by using ```task_manager_start()```.
6. The action\_manager sets a handler to receive unicast messages from other applications by using ```task_manager_set_handler()```.
7. The task\_manager\_sample\_main requests to start the user\_app to the task manager by using ```task_manager_start()```.
8. The user\_app requests the handle of action\_manager to the task manager in order to send a unicast message to the action\_manager by using ```task_manager_getinfo_with_name()```.
9. The user\_app sends a unicast message to the action\_manager through the task manager to start the alarm action by using ```task_manager_unicast()```.
10. The action\_manager requests to start the alarm action through the task manager by using ```task_manager_start()```.
11. Three seconds after sending the alarm action start request message, the user\_app sends a unicast message to the action\_manager through the task manager to pause the alarm action by using ```task_manager_unicast()```.
12. The action\_manager requests to pause the alarm action through the task manager by using ```task_manager_pause()```.
13. Three seconds after sending the alarm action pause request message, the user\_app sends a unicast message to the action\_manager through the task manager to resume the alarm action by using ```task_manager_unicast()```.
14. The action\_manager requests to resume the alarm action through the task manager by using ```task_manager_resume()```.
15. Two seconds after sending the alarm action resume request message, the user\_app sends a unicast message to the action\_manager through the task manager to terminate the alarm action by using ```task_manager_unicast()```.
16. The action\_manager requests to terminate the alarm action through the task manager by using ```task_manager_terminate()```.
17. The action\_manager requests to unregister the alarm action, the LED on action, and the LED off action to the task manager by using ```task_manager_unregister()```.
18. The task\_manager\_sample\_main requests to unregister the action\_manager and the user\_app to the task manager by using ```task_manager_unregister()```.
