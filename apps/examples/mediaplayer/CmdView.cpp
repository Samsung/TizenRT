/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include "CmdView.h"

#include <iostream>

using namespace std;

namespace MediaPlayerApp
{
int CmdView::getUserInput(int min, int max)
{
	if (min > max) {
		cout << "min > max is impossible. Return -1." << endl;
		return -1;
	}

	int input;

	while (true) {
		cin >> input;
		cout << endl;

		if (!cin.fail()) {
			if (min <= input && input <= max) {
				cout << "return input " << input << endl;
				return input;
			}
		}

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid Input, please try again" << endl;
	}
}

int CmdView::selectSource(vector<string> sourceList)
{
	cout << "====================" << endl;
	for (size_t i = 0; i < sourceList.size(); i++) {
		cout << ' ' << i << ". " << sourceList[i] << endl;
	}
	cout << "====================" << endl;

	return getUserInput(0, sourceList.size());
}

int CmdView::selectPlayer(void)
{
	cout << "====================" << endl;
	cout << " 0. Select Source   " << endl;
	cout << " 1. Select PLAYER A " << endl;
	cout << " 2. Select PLAYER B " << endl;
	cout << "====================" << endl;

	int player = getUserInput(0, 2) - 1;
	if (player != -1) {
		cout << "PLAYER " << (char)('A' + player) << " is selected" << endl;
	}
	return player;
}

int CmdView::selectCommand(void)
{
	cout << "====================" << endl;
	cout << " 0. Do Nothing      " << endl;
	cout << " 1. PLAYER_START    " << endl;
	cout << " 2. PLAYER_PAUSE    " << endl;
	cout << " 3. PLAYER_RESUME   " << endl;
	cout << " 4. PLAYER_STOP     " << endl;
	cout << " 5. GET_MAX_VOLUME  " << endl;
	cout << " 6. VOLUME_UP       " << endl;
	cout << " 7. VOLUME_DOWN     " << endl;
	cout << "====================" << endl;
	return getUserInput(0, 7);
}
}
