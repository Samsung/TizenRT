/* app list should be filled except the last field like below.
(the last field should be set to 0 by default, then TizenRT automatically set again to proper value.)

{app_id, entry_point, priority, stack_size, is_on_boot, 0},
ex : {"com.example.test", test_sample_main, 1024, 100, false, 0}, */
{"com.example.testapp1", testapp, 4096, 100, false, 0},
