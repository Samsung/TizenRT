/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include <string.h>

#include "octypes.h"
#include "ocstack.h"
#include "psinterface.h"

#include "credresource.h"

#include "svrdbeditorcommon.h"
#include "svrdbeditoracl.h"
#include "svrdbeditorcred.h"
#include "svrdbeditordoxm.h"
#include "svrdbeditorpstat.h"
#include "pstatresource.h"

static bool g_allowedEditMenu[SVR_EDIT_IDX_SIZE] = {false/*unused*/, false, false, false, false};
static char g_svrDbPath[SVR_DB_PATH_LENGTH];

typedef enum OperationType
{
    SVR_PRINT_ALL = 1,
    SVR_EDIT_CRED,
    SVR_EDIT_ACL,
    SVR_EDIT_DOXM,
    SVR_EDIT_PSTAT = 5,
    EXIT = 99
} OperationType_t;

static int MainOperation(const char *svrpath);
static void PrintMainMenu(void);
static void PrintEditMenu(const char *resourceName, bool print, bool add, bool remove,
                          bool modify);
static void PrintHelp(void);
static FILE *SVRDBFopen(const char *path, const char *mode);

int main(int argc, char *argv[])
{
    if (2 == argc)
    {
        PRINT_NORMAL("SVR DB File Path: %s\n", argv[1]);
        return MainOperation(argv[1]);
    }
    else
    {
        PrintHelp();
        return 0;
    }
}

static int MainOperation(const char *svrpath)
{
    OperationType_t menu = EXIT;
    SubOperationType_t editMenu = EXIT;
    OCStackResult ocResult = OC_STACK_ERROR;
    bool run = true;

    // initialize persistent storage for SVR DB
    static OCPersistentStorage psInst =
    {
        .open = SVRDBFopen,
        .read = fread,
        .write = fwrite,
        .close = fclose,
        .unlink = unlink
    };

    if (!svrpath)
    {
        PRINT_ERR("Incorrect file path");
        return -1;
    }

    strncpy(g_svrDbPath, svrpath, sizeof(g_svrDbPath) - 1);
    g_svrDbPath[sizeof(g_svrDbPath) - 1] = '\0';

    ocResult = OCRegisterPersistentStorageHandler(&psInst);
    if (OC_STACK_OK != ocResult)
    {
        PRINT_ERR("OCRegisterPersistentStorageHandler : %d", ocResult);
        return -1;
    }

    InitPstatResourceToDefault();

    RefreshACL();
    RefreshCred();
    RefreshDoxm();
    RefreshPstat();
    while (run)
    {
        PrintMainMenu();
        menu = (OperationType_t)InputNumber("\tSelect the menu : ");
        switch (menu)
        {
            case SVR_PRINT_ALL:
                PrintDoxm();
                PrintPstat();
                PrintAcl();
                PrintCredList(GetCredList());
                break;
            case SVR_EDIT_CRED:
                for (;;)
                {
                    if (NULL == GetCredList())
                    {
                        PRINT_WARN("Credential resource is empty.");
                        PrintEditMenu("Credential Resource", false, true, false, false);
                    }
                    else
                    {
                        PrintEditMenu("Credential Resource", true, true, true, true);
                    }
                    editMenu = (SubOperationType_t)InputNumber("Select the menu : ");
                    if (0 < editMenu && editMenu < SVR_EDIT_IDX_SIZE)
                    {
                        if (!g_allowedEditMenu[editMenu])
                        {
                            PRINT_ERR("Disabled menu");
                            continue;
                        }
                    }
                    else if (BACK == editMenu)
                    {
                        PRINT_INFO("Back to the previous menu.");
                        break;
                    }
                    else
                    {
                        PRINT_ERR("Invalid menu");
                        continue;
                    }
                    HandleCredOperation(editMenu);
                    RefreshCred();
                }
                break;
            case SVR_EDIT_ACL:
                for (;;)
                {
                    PrintEditMenu("ACL Resource", true, true, true, true);
                    editMenu = (SubOperationType_t)InputNumber("Select the menu : ");
                    if (0 < editMenu && editMenu < SVR_EDIT_IDX_SIZE)
                    {
                        if (!g_allowedEditMenu[editMenu])
                        {
                            PRINT_ERR("Disabled menu");
                            continue;
                        }
                    }
                    else if (BACK == editMenu)
                    {
                        PRINT_INFO("Back to the previous menu.");
                        break;
                    }
                    else
                    {
                        PRINT_ERR("Invalid menu");
                        continue;
                    }
                    HandleAclOperation(editMenu);
                    RefreshACL();
                }
                break;
            case SVR_EDIT_DOXM:
                for (;;)
                {
                    PrintEditMenu("Doxm Resource", true, true, true, true);
                    editMenu = (SubOperationType_t)InputNumber("Select the menu : ");
                    if (0 < editMenu && editMenu < SVR_EDIT_IDX_SIZE)
                    {
                        if (!g_allowedEditMenu[editMenu])
                        {
                            PRINT_ERR("Disabled menu");
                            continue;
                        }
                    }
                    else if (BACK == editMenu)
                    {
                        PRINT_INFO("Back to the previous menu.");
                        break;
                    }
                    else
                    {
                        PRINT_ERR("Invalid menu");
                        continue;
                    }
                    HandleDoxmOperation(editMenu);
                    RefreshDoxm();
                }
                break;
            case SVR_EDIT_PSTAT:
                for (;;)
                {
                    PrintEditMenu("Pstat Resource", true, true, true, true);
                    editMenu = (SubOperationType_t)InputNumber("Select the menu : ");
                    if (0 < editMenu && editMenu < SVR_EDIT_IDX_SIZE)
                    {
                        if (!g_allowedEditMenu[editMenu])
                        {
                            PRINT_ERR("Disabled menu");
                            continue;
                        }
                    }
                    else if (BACK == editMenu)
                    {
                        PRINT_INFO("Back to the previous menu.");
                        break;
                    }
                    else
                    {
                        PRINT_ERR("Invalid menu");
                        continue;
                    }
                    HandlePstatOperation(editMenu);
                    RefreshPstat();
                }
                break;
            case EXIT:
                run = false;
                break;
            default:
                PRINT_ERR("Unknown operation");
                PRINT_ERR("Please make sure the menu.");
                break;
        }
    }

    DeInitCredResource();
    DeInitACL();
    DeInitDoxm();
    DeInitPstat();
    return 0;
}

static FILE *SVRDBFopen(const char *path, const char *mode)
{
    (void)path;  // unused |path| parameter
    return fopen(g_svrDbPath, mode);
}

static void PrintHelp(void)
{
    PRINT_ERR("<This program requires one input>");
    PRINT_INFO("./svrdbeditor <svr_db_file_path>");
}

static void PrintEditMenu(const char *resourceName, bool print, bool add, bool remove,
                          bool modify)
{
    PRINT_PROG("\n\nYou can perform the "
               CYAN_BEGIN "cyan color opertions " COLOR_END
               BOLD_BEGIN "for" COLOR_END
               YELLOW_BEGIN " %s" COLOR_END_NL, resourceName);

    for (int i = 0; i < SVR_EDIT_IDX_SIZE; i++)
    {
        g_allowedEditMenu[i] = false;
    }
    if (print)
    {
        g_allowedEditMenu[SVR_PRINT] = true;
        PRINT_DATA("\t%2d. Print all entities\n", SVR_PRINT);
    }
    else
    {
        PRINT_NORMAL("\t%2d. Print all entities\n", SVR_PRINT);
    }

    if (add)
    {
        g_allowedEditMenu[SVR_ADD] = true;
        PRINT_DATA("\t%2d. Add entity\n", SVR_ADD);
    }
    else
    {
        PRINT_NORMAL("\t%2d. Add entity\n", SVR_ADD);
    }
    if (remove)
    {
        g_allowedEditMenu[SVR_REMOVE] = true;
        PRINT_DATA("\t%2d. Remove entity\n", SVR_REMOVE);
    }
    else
    {
        PRINT_NORMAL("\t%2d. Remove entity\n", SVR_REMOVE);
    }

    if (modify)
    {
        g_allowedEditMenu[SVR_MODIFY] = true;
        PRINT_DATA("\t%2d. Modify entity\n", SVR_MODIFY);
    }
    else
    {
        PRINT_NORMAL("\t%2d. Modify entity\n", SVR_MODIFY);
    }
    PRINT_DATA("\t%2d. Back to the main menu\n", BACK);
}

static void PrintMainMenu(void)
{
    PRINT_PROG("\n\nYou can perform the "
               CYAN_BEGIN "cyan color opertions : " COLOR_END_NL);

    PRINT_DATA("\t%2d. Print All Security Resource.\n", SVR_PRINT_ALL);
    PRINT_DATA("\t%2d. Edit Credential Resource.\n", SVR_EDIT_CRED);
    PRINT_DATA("\t%2d. Edit ACL Resource.\n", SVR_EDIT_ACL);
    PRINT_DATA("\t%2d. Edit Doxm Resource.\n", SVR_EDIT_DOXM);
    PRINT_DATA("\t%2d. Edit Pstat Resource.\n", SVR_EDIT_PSTAT);
    PRINT_DATA("\t%2d. Exit.\n", EXIT);
}
