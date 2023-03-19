/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include <vector>
#include <stack>
using namespace std;
vector<vector<int>>visit;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isbfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	hPopup = menu->AddPopupMenu(hMenu, "DFS");
	menu->AddPopupItem(hPopup, "DFS", false, false); // No auto check

	hPopup = menu->AddPopupMenu(hMenu, "BFS");
	menu->AddPopupItem(hPopup, "BFS", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			visit.assign(10001, vector<int>(10001, 0));
			isdfs = false, isbfs = false;
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		if (isOpen)
		{
			visit.assign(10001, vector<int>(10001, 0));
			isdfs = false, isbfs = false;
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

	if (title == "DFS") {
		if (isOpen)
		{
			visit.assign(10001, vector<int>(10001, 0));
			isdfs = false, isbfs = false;
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if (title == "BFS") {
		if (isOpen)
		{
			visit.assign(10001, vector<int>(10001, 0));
			isdfs = false, isbfs = false;
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	if (isOpen)
	{
		ofDrawLine(0, 0, WIDTH * 40, 0);
		ofDrawLine(0, 0, 0, HEIGHT * 40);
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				if (arr[i][j].rwall)
					ofDrawLine((j + 1) * 40, i * 40, (j + 1) * 40, (i + 1) * 40);
				if (arr[i][j].dwall)
					ofDrawLine(j * 40, (i + 1) * 40, (j + 1) * 40, (i + 1) * 40);
			}
		}
	}

	if (isdfs)
	{
		ofSetColor(255, 0, 0);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	
	if (isbfs)
	{
		ofSetColor(255, 0, 0);
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(100);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}
} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		//printf("file name is %s\n", fileName);
		cout << "file name is " << fileName << "\n";
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;
			
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			isdfs = false, isbfs = false;
			freeMemory();
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it, cnt++)
			{
				string line = *it;
				if (cnt == 0)
					continue;
				else if (cnt % 2 == 1)
				{
					vector<Room>R;
					for (int i = 2; i < line.length(); i += 2)
					{
						Room r;
						if (line[i] == '|')
							r.rwall = 1, r.dwall = 0;
						else
							r.rwall = 0, r.dwall = 0;
						R.push_back(r);
					}
					arr.push_back(R);
				}
				else
				{
					for (int i = 1; i < line.length(); i += 2)
					{
						if (line[i] == '-')
							arr[idx][i / 2].dwall = 1;
					}
					idx++;
				}
			}
			WIDTH = arr[0].size();
			HEIGHT = arr.size();
			return true;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수
	for (int i = 0; i < arr.size(); i++)
		arr[i].clear();

	arr.clear();
	visit.assign(10001, vector<int>(10001, 0));
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	stack<pair<int, int>>s;
	int dir[4][2] = { {0,1},{1,0},{-1,0},{0,-1} };
	visit[0][0] = 1;
	s.push({ 0,0 });
	while (!s.empty())
	{
		pair<int, int>top = s.top();
		s.pop();
		if (top.first == HEIGHT - 1 && top.second == WIDTH - 1)
			break;
		for (int i = 0; i < 4; i++)
		{
			int nx = top.first + dir[i][0];
			int ny = top.second + dir[i][1];
			if (0 <= nx && nx < HEIGHT && 0 <= ny && ny < WIDTH && visit[nx][ny] == 0)
			{
				if (i == 0 && arr[top.first][top.second].rwall == 0)
				{
					visit[nx][ny] = visit[top.first][top.second] + 1;
					s.push({ top.first,top.second });
					s.push({ nx,ny });
					break;
				}
				else if (i == 1 && arr[top.first][top.second].dwall == 0)
				{
					visit[nx][ny] = visit[top.first][top.second] + 1;
					s.push({ top.first,top.second });
					s.push({ nx,ny });
					break;
				}
				else if (i == 2 && arr[nx][ny].dwall == 0)
				{
					visit[nx][ny] = visit[top.first][top.second] + 1;
					s.push({ top.first,top.second });
					s.push({ nx,ny });
					break;
				}
				else if (i == 3 && arr[nx][ny].rwall == 0)
				{
					visit[nx][ny] = visit[top.first][top.second] + 1;
					s.push({ top.first,top.second });
					s.push({ nx,ny });
					break;
				}
			}
		}
	}
	isdfs = true;
	return true;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (visit[i][j] != 0)
			{
				if (arr[i][j].rwall == 0 && visit[i][j + 1] != 0)
					ofDrawLine(j * 40 + 20, i * 40 + 20, (j + 1) * 40 + 20, i * 40 + 20);
				if (arr[i][j].dwall == 0 && visit[i + 1][j] != 0)
					ofDrawLine(j * 40 + 20, i * 40 + 20, j * 40 + 20, (i + 1) * 40 + 20);
			}
		}
	}

	ofSetColor(0, 255, 0);
	int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
	int x = HEIGHT - 1, y = WIDTH - 1;
	int flag = 0;
	while (1)
	{
		if (x == 0 && y == 0)
			break;
		for (int i = 0; i < 4; i++)
		{
			int nx = x + dir[i][0];
			int ny = y + dir[i][1];
			if (0 <= nx && nx < HEIGHT && 0 <= ny && ny < WIDTH && visit[nx][ny] == visit[x][y] - 1)
			{
				if (i == 0 && arr[nx][ny].dwall == 0)
					flag = 1;
				else if (i == 1 && arr[x][y].dwall == 0)
					flag = 1;
				else if (i == 2 && arr[nx][ny].rwall == 0)
					flag = 1;
				else if (i == 3 && arr[x][y].rwall == 0)
					flag = 1;
				if (flag == 1)
				{
					ofDrawLine(ny * 40 + 20, nx * 40 + 20, y * 40 + 20, x * 40 + 20);
					x = nx, y = ny;
					flag = 0;
					break;
				}
			}
		}
	}
}

bool ofApp::BFS()//BFS탐색을 하는 함수
{
	queue<pair<int, int>>q;
	int dir[4][2] = { {0,1},{1,0},{-1,0},{0,-1} };
	visit[0][0] = 1;
	q.push({ 0,0 });
	while (!q.empty())
	{
		pair<int, int>front = q.front();
		q.pop();
		if (front.first == HEIGHT - 1 && front.second == WIDTH - 1)
			break;
		for (int i = 0; i < 4; i++)
		{
			int nx = front.first + dir[i][0];
			int ny = front.second + dir[i][1];
			if (0 <= nx && nx < HEIGHT && 0 <= ny && ny < WIDTH && visit[nx][ny] == 0)
			{
				if (i == 0 && arr[front.first][front.second].rwall == 0)
				{
					visit[nx][ny] = visit[front.first][front.second] + 1;
					q.push({ nx,ny });
				}
				else if (i == 1 && arr[front.first][front.second].dwall == 0)
				{
					visit[nx][ny] = visit[front.first][front.second] + 1;
					q.push({ nx,ny });
				}
				else if (i == 2 && arr[nx][ny].dwall == 0)
				{
					visit[nx][ny] = visit[front.first][front.second] + 1;
					q.push({ nx,ny });
				}
				else if (i == 3 && arr[nx][ny].rwall == 0)
				{
					visit[nx][ny] = visit[front.first][front.second] + 1;
					q.push({ nx,ny });
				}
			}
		}
	}
	isbfs = true;
	return true;
}

void ofApp::bfsdraw()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (visit[i][j] != 0)
			{
				if (arr[i][j].rwall == 0 && visit[i][j + 1] != 0)
					ofDrawLine(j * 40 + 20, i * 40 + 20, (j + 1) * 40 + 20, i * 40 + 20);
				if (arr[i][j].dwall == 0 && visit[i + 1][j] != 0)
					ofDrawLine(j * 40 + 20, i * 40 + 20, j * 40 + 20, (i + 1) * 40 + 20);
			}
		}
	}

	ofSetColor(0, 255, 0);
	int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
	int x = HEIGHT - 1, y = WIDTH - 1;
	int flag = 0;
	while (1)
	{
		if (x == 0 && y == 0)
			break;
		for (int i = 0; i < 4; i++)
		{
			int nx = x + dir[i][0];
			int ny = y + dir[i][1];
			if (0 <= nx && nx < HEIGHT && 0 <= ny && ny < WIDTH && visit[nx][ny] == visit[x][y] - 1)
			{
				if (i == 0 && arr[nx][ny].dwall == 0)
					flag = 1;
				else if (i == 1 && arr[x][y].dwall == 0)
					flag = 1;
				else if (i == 2 && arr[nx][ny].rwall == 0)
					flag = 1;
				else if (i == 3 && arr[x][y].rwall == 0)
					flag = 1;
				if (flag == 1)
				{
					ofDrawLine(ny * 40 + 20, nx * 40 + 20, y * 40 + 20, x * 40 + 20);
					x = nx, y = ny;
					flag = 0;
					break;
				}
			}
		}
	}
}