/* message port list should be filled except the last field like below.
(the last field should be set to 0 by default, then TizenRT automatically set again to proper value.)
(TizenRT_broadcast_value should be in framework/include/task_manager/task_manager_broadcast_list.h)

{message_port, TizenRT_broadcast_value, message port id(automatically set)},
ex : {"test_msg_port", TEST_MSG_PORT, 0}, */
{"testapp", TEST_MSG_PORT, 0},
