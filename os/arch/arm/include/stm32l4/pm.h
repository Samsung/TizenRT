/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Name: pm_sleep
 *
 * Description:
 *   Cause immediate transit from NORMAL to SLEEP
 *    and from SLEEP to NORMAL upon wakeup
 *
 * Returned Value:
 *   0 (OK) means that the transitions were successful.
 *   Non-zero means that the transition failed maybe because
 *   one of the drivers refused the state change.  In this case,
 *   the system will revert to the preceding state.
 *
 ****************************************************************************/
int pm_sleep(void);
