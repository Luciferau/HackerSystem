#include  <graphics.h> 	// 引用 EasyX 图形库
#include  <Windows.h>
#include <iostream>
#include <string>
#include <mmsystem.h> // mci库头文件
#include<conio.h>
#include "hacker.h"

#pragma comment(lib, "winmm.lib")
using namespace std;

char NAME[] =  "54hk";
char PASSWORD[] = "123456";

#define  CONNECT_FAIL "connect server failed"
#define  ATTACK_SUCCESS "攻击成功"
#define  ATTACK_FAIL "攻击失败"
#define  ATTACK_RECORD_SUCCESS "["

typedef struct {
	int x;
	int y;
	int w;
	int h;
}rect_t;

const  char* titles[] =
{
	"1-网站404攻击",
	"2-网站篡改攻击",
	"3-网站攻击修复",
	"4-查看攻击记录",
	"5-退出"
};

rect_t item1, item2, item3, item4, item5;

RECT rectButton = {258, 188, 446, 211};
RECT rectName = {317, 108, 443, 132};
RECT rectPwd = {317, 147, 443, 171};
IMAGE imgButton;
IMAGE imgName;
IMAGE imgPwd;

int lastIndex = 0;

IMAGE retButtonBg;
IMAGE loginButtonNormal;
char name[16] = "";
char password[16] = "";

void PRINT(char *msg) {
	TCHAR s[1024];
	_stprintf(s, _T("DEBUG:%s"), msg);

	settextcolor(RGB(255,255,0));   
	outtextxy(10, 10,"                                                                       ");
	outtextxy(10, 10,s); //在指定位置输出文本
	
    system("pause");
}

std::string UTF8ToGBK(const char* strUTF8)  
{  
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);  
	wchar_t* wszGBK = new wchar_t[len+1];  
	memset(wszGBK, 0, len*2+2);  
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);  
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);  
	char* szGBK = new char[len+1];  
	memset(szGBK, 0, len+1);  
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);  
	std::string strTemp(szGBK);  
	if(wszGBK) delete[] wszGBK;  
	if(szGBK) delete[] szGBK;  
	return strTemp;  
}  

void ConvertGBKToUtf8(string &strGBK)
{
    int len=MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL,0);
    wchar_t * wszUtf8 = new wchar_t [len];
    memset(wszUtf8, 0, len);
    MultiByteToWideChar(CP_ACP, 0,  strGBK.c_str(), -1, wszUtf8, len);
 
    len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);
 
    strGBK = szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;
}

void systemInit(void) {
	//initgraph(600, 340);	// 初始化图形界面, 画布大小为640x480
	
	rect_t tmp = {300, 40, 250, 40};
	item1 = tmp;
	item2 = item1;
	item2.y += 60;
	item3 = item2;
	item3.y += 60;
	item4 = item3;
	item4.y += 60;
	item5 = item4;
	item5.y += 60;
}

void drawUnSelected(int index) {
	if (index <= 0) {
		return;
	}

	rect_t items[] = {item1, item2, item3, item4, item5};
	rect_t item = items[index - 1];

	IMAGE img;
	loadimage(&img, "background1.jpg");
	putimage(item.x, item.y, &img);    
	setbkmode(TRANSPARENT);  //使得文本的背景色为透明效果
	settextcolor(RGB(240,240,240));   
	outtextxy(item.x+10, item.y+5, titles[index-1]); //在指定位置输出文本
}


void drawSelected(int index) {
	if (index <= 0) {
		return;
	}

	rect_t items[] = {item1, item2, item3, item4, item5};
	rect_t item = items[index - 1];

	IMAGE img;
	loadimage(&img, "background2.jpg");
	putimage(item.x, item.y, &img);    
	setbkmode(TRANSPARENT);  //使得文本的背景色为透明效果
	settextcolor(RGB(255,255,0));   
	outtextxy(item.x+10, item.y+5, titles[index-1]); //在指定位置输出文本
}


int inRect(int x, int y) {
	rect_t items[] = {item1, item2, item3, item4, item5};
	int n = sizeof(items) / sizeof(items[0]);
	for (int i=0; i<n; i++) {
		if (x >= items[i].x &&  x<=  items[i].x +  items[i].w &&
			y >=  items[i].y &&  y<=  items[i].y +  items[i].h) {
			return i+1;
		}
	}
	
	return 0;
}

void reDraw(int x, int y) {
	int n;
	n = inRect(x, y);
	if (n != lastIndex) {
		if (lastIndex != 0) {
			drawUnSelected(lastIndex);
		} 
		drawSelected(n);
		lastIndex = n;
	}
}

void getOldBack(IMAGE *button) {
	getimage(button, 436, 206, 142, 111);
}

void reDrawButton(int x, int y) {
	rect_t buttonItem = {436, 206, 142, 111};  

	if (x >=buttonItem.x &&  x<=  buttonItem.x +  buttonItem.w &&
		y >=  buttonItem.y &&  y<= buttonItem.y + buttonItem.h) {
			IMAGE img;
			loadimage(&img, "button3.jpg");
			putimage(buttonItem.x, buttonItem.y, &img);    
	} else {
			putimage(buttonItem.x, buttonItem.y, &retButtonBg);    
	}
}

bool clickButton(int x, int y) {
	int n;
	rect_t buttonItem = {436, 206, 142, 111};  

	if (x >=buttonItem.x &&  x<=  buttonItem.x +  buttonItem.w &&
		y >=  buttonItem.y &&  y<= buttonItem.y + buttonItem.h) {
			mciSendString("play click.wav", 0, 0, 0);
			FlushMouseMsgBuffer();
			return true;
	}
	return false;
}


void show_memu(void) {
	loadimage(0,  "bg.jpg");
	settextstyle(30, 0, "微软雅黑"); 

	for (int i=1; i<=5; i++) {
		drawUnSelected(i);
	}
}

bool inRect(int x, int y, rect_t rect) {
	if (x < rect.x || x > rect.x+rect.w || y < rect.y ||y > rect.y+rect.h) {
		return false;
	} else {
		return true;
	}
}


int getSelectedIndex(void) {
	MOUSEMSG m;		// 定义鼠标消息
	int index;

	FlushMouseMsgBuffer();

	while(true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();

		switch(m.uMsg)
		{
			case WM_MOUSEMOVE:
				reDraw(m.x, m.y);
				break;

			case WM_LBUTTONDOWN:
				index = inRect(m.x, m.y);
				if (index > 0) {
					mciSendString("play click.wav", 0, 0, 0);
					FlushMouseMsgBuffer();
					return index;
				}				
				break;
			case WM_RBUTTONUP:
				break;	// 按鼠标右键退出程序
		}
	}
}

void show_wait(void) {
	loadimage(0,  "wait.jpg");
}

void waitReturnMenu(void) {
	MOUSEMSG m;	
	
	getOldBack(&retButtonBg);

	FlushMouseMsgBuffer();
	while(true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		//436,206  w:142 H:111
		switch(m.uMsg)
		{
			case WM_MOUSEMOVE:
				reDrawButton(m.x, m.y);
				break;

			case WM_LBUTTONDOWN:
				if (clickButton(m.x, m.y)) {
					return;
				}
				break;
			case WM_RBUTTONUP:
				break;	
		}
	}
}

void show_success(void) {
	mciSendString("play complete.wav", 0, 0, 0);
	loadimage(0,  "success1.jpg");
	//PRINT("show_success");
	waitReturnMenu();
}

void showRecords(const char* response) {
	settextstyle(21, 0, "微软雅黑"); 
	settextcolor(RGB(0,0,0));   
	RECT r = {10, 114, 434, 338};
	drawtext(response, &r, DT_LEFT | DT_WORDBREAK);
}

void show_success(const char *response) {
	mciSendString("play complete.wav", 0, 0, 0);
	loadimage(0,  "success4.jpg");
	showRecords(response);
	waitReturnMenu();
}

bool check_response(const char *response) {
	if (!strncmp(response, ATTACK_SUCCESS, strlen(ATTACK_SUCCESS)) || 
		!strncmp(response, ATTACK_RECORD_SUCCESS, strlen(ATTACK_RECORD_SUCCESS))){
		return true;	
	} 

	return false;
}

void show_fail(void) {

}

void waitBySound(void) {
	mciSendString("play tel.wav", 0, 0, 0);
	Sleep(4000);
	/*
	for (int i=0; i<3; i++) {
		mciSendString("play wait.mp3", 0, 0, 0);
		Sleep(1000);
	}
	*/
}

void attack404(void) {
	char response[MAXSIZE] ;
	char id[64];

	for (int i=0; i<MAXSIZE; i++) {
		response[i] = 0;
	}

	InputBox(id, sizeof(id), "请输入准备攻击的网站ID：", "网站404攻击", 0, 250, 20); 

	show_wait();
	hk_404(id, response);
	waitBySound();
	//Sleep(3000); //增加延时,以便演示效果
	string retStr = UTF8ToGBK(response);

	if (check_response(retStr.c_str())) {
		show_success();
	} else {
		show_fail();
	}
}

void attackTamper(void) {
	char response[MAXSIZE];
	char id[16];
	std::string str;
	char text[512];

	InputBox(id, sizeof(id), "请输入准备攻击的网站ID：", "网站篡改攻击", 0, 250, 20); 
	InputBox(text, sizeof(text), "请输入嵌入的内容：", "网站篡改攻击", 0, 250, 20); 
	
	show_wait();

	string attackText = text;
	ConvertGBKToUtf8(attackText);
	hk_tamper(id,  (char*)(attackText.c_str()), response);
	waitBySound();
	//Sleep(3000);	
	
	string retStr = UTF8ToGBK(response);
	if (check_response(retStr.c_str())) {
		show_success();
	} else {
		show_fail();
	}
}

void attackRestore(void) {
	char response[MAXSIZE] ;
	char id[64];

	for (int i=0; i<MAXSIZE; i++) {
		response[i] = 0;
	}

	InputBox(id, sizeof(id), "请输入准备修复的网站ID：", "网站攻击修复", 0, 250, 20); 

	show_wait();
	hk_restore(id, response);
	waitBySound();
	//Sleep(3000); //增加延时,以便演示效果
	string retStr = UTF8ToGBK(response);

	if (check_response(retStr.c_str())) {
		show_success();
	} else {
		show_fail();
	}
}

void attackRecord(void) {
	char response[MAXSIZE] ;
	char id[64];

	for (int i=0; i<MAXSIZE; i++) {
		response[i] = 0;
	}

	InputBox(id, sizeof(id), "请输入准备获取记录的网站ID：", "攻击记录获取", 0, 260, 20); 

	show_wait();
	hk_record(id, response);
	waitBySound();
	//Sleep(3000); //增加延时,以便演示效果
	string retStr = UTF8ToGBK(response);

	if (check_response(retStr.c_str())) {
		show_success(retStr.c_str());
	} else {
		show_fail();
	}
}

void logout(void) {
	closegraph();
	exit(0);
}


typedef void (*selectedHandler_t)(void);
selectedHandler_t handlers[] = {
	attack404,
	attackTamper,
	attackRestore,
	attackRecord,
	logout
};

bool inRect(int  x, int y, RECT rect) {
	if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom) {
		return true;
	} else {
		return false;
	}
}

void reDrawLoginButton(int x, int y) {
	RECT rect = {258, 188, 446, 211};
	if (inRect(x, y, rect)) {
		IMAGE img;
		loadimage(&img, "login2.jpg");
		putimage(258, 188, &img);
	} else {
		putimage(258, 188, &loginButtonNormal);
	}
}

void inputName() {
	char c;
	int i = 0;
	int x = rectName.left+2;
	int y = rectName.top+2;
	
	setlinecolor(RGB (255, 0, 0));
	setlinestyle( PS_SOLID,  2);  //线宽10, 实线
	rectangle(rectName.left, rectName.top, rectName.right, rectName.bottom);

	while (1) {
		c = getch();
		if (c == '\r' || i >= sizeof(name)) {  
			name[i] = 0;
			setlinecolor(RGB (0, 0, 0));
			rectangle(rectName.left, rectName.top, rectName.right, rectName.bottom);
			break;
		}
		name[i++] = c;
		//printf("*", c);
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0,0,0));
		outtextxy(x, y, c);
		x += 9;
	}
}

void inputPwd() {
	char c;
	int i = 0;
	int x = rectPwd.left+2;
	int y = rectPwd.top+2;
	
	setlinecolor(RGB (255, 0, 0));
	setlinestyle( PS_SOLID,  2);  //线宽10, 实线
	rectangle(rectPwd.left, rectPwd.top, rectPwd.right, rectPwd.bottom);

	while (1) {
		c = getch();
		if (c == '\r' || i >= sizeof(name)) {  
			password[i] = 0;
			setlinecolor(RGB (0, 0, 0));
			rectangle(rectPwd.left, rectPwd.top, rectPwd.right, rectPwd.bottom);
			break;
		}
		password[i++] = c;
		//printf("*", c);
		setbkmode(TRANSPARENT);
		settextcolor(RGB(0,0,0));
		outtextxy(x, y, '*');
		x += 9;
	}
}

//直到移动鼠标
void pause(void) {
	FlushMouseMsgBuffer();
	GetMouseMsg();		
}

bool loginClick(int x, int y) {
	if (inRect(x, y, rectName)) {
		inputName();
	} else if (inRect(x, y, rectPwd)) {
		inputPwd();
	} else if (inRect(x, y, rectButton)) {
		mciSendString("play click.wav", 0, 0, 0);
		if (strcmp(name, NAME)==0 && strcmp(password, PASSWORD)==0) {
			return true;
		} else {
			putimage(rectName.left, rectName.top, &imgName); 
			putimage(rectPwd.left, rectPwd.top, &imgPwd); 
			IMAGE img;
			loadimage(&img, "error.jpg");
			putimage(146, 226, &img);
			pause();
			loadimage(0, "login.jpg");
		}
	}

	return false;
	return false;
}

void login(void) {
	MOUSEMSG m;	

	initgraph(600, 340);	
	loadimage(0,  "login.jpg");
	getimage(&loginButtonNormal, 258, 188, 188, 24);
	getimage(&imgName, rectName.left, rectName.top, rectName.right-rectName.left, rectName.bottom-rectName.top);
	getimage(&imgPwd, rectPwd.left, rectPwd.top, rectPwd.right-rectPwd.left, rectPwd.bottom-rectPwd.top);

	settextstyle(20, 0, "Consolas"); 

	while(true)
	{
		m = GetMouseMsg();

		switch(m.uMsg)
		{
			case WM_MOUSEMOVE:
				reDrawLoginButton(m.x, m.y);
				break;

			case WM_LBUTTONUP:
				if (loginClick(m.x, m.y)) {
					FlushMouseMsgBuffer();
					return;
				}
				break;
			case WM_RBUTTONUP:
				break;	// 按鼠标右键退出程序
		}
	}

}

int main(void) {
	int n; //用户选择的菜单编号

	login();

	systemInit(); //初始化
	
	while (1) {
		show_memu();		
		n = getSelectedIndex();  //获取单击的选项序号
		handlers[n-1]();  //调用对应的攻击处理函数
	}
	
	return 0;
}