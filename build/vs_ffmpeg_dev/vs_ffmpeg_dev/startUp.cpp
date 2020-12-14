#include <iostream>
#include <thread>
#include <windows.h>
#include <stdlib.h>

extern "C" {
	int func(char *mRtsp);
	int exitFlag = 0; // 还必须放在这里面
}

BOOL WINAPI  ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
		// handle the ctrl-c signal.
	case CTRL_C_EVENT:
		exitFlag = 1;
		printf("ctrl-c event\n\n");
		return (TRUE);
		// ctrl-close: confirm that the user wants to exit.
		//case CTRL_CLOSE_EVENT:
		//	printf("ctrl-close event\n\n");
		//	return(true);
		//	// pass other signals to the next handler.
		//case CTRL_BREAK_EVENT:
		//	printf("ctrl-break event\n\n");
		//	return false;
		//case CTRL_LOGOFF_EVENT:
		//	printf("ctrl-logoff event\n\n");
		//	return false;
		//case CTRL_SHUTDOWN_EVENT:
		//	printf("ctrl-shutdown event\n\n");
		//	return false;
	default:
		return false;
	}
}

int getRtspAddress(char * configFile, char * rtsp1, char * rtsp2, int bufSize) {
	FILE *in = fopen(configFile, "r");

	if (in == NULL) {
		printf("open configFile %s error \n", configFile);
		return -1;
	}

	char * res = fgets(rtsp1, bufSize, in);
	if (res == NULL) {
		printf("error: no rtsp addr \n");
		return -1;
	}
	if (rtsp1[strlen(rtsp1) - 1] == '\n') {
		rtsp1[strlen(rtsp1) - 1] = '\0';
	}
	res = fgets(rtsp2, bufSize, in);
	if (res == NULL) {
		printf("error: no rtsp addr 2\n");
		return -1;
	}
	if (rtsp2[strlen(rtsp2) - 1] == '\n') {
		rtsp2[strlen(rtsp2) - 1] = '\0';
	}
	fclose(in);
	return 0;
}



int main() {
	if (SetConsoleCtrlHandler(ctrlhandler, TRUE))
	{
		printf("\nThe Control Handler is installed.\n");
	}
	else {
		printf("\nThe Control Handler install failed.\n");
	}
	char rtsp1[1024];
	char rtsp2[1024];
	if (getRtspAddress("rtspAddr.config", rtsp1, rtsp2, 1024) != 0) {
		return -1;
	}

	char *mRtsp = rtsp1;// "rtsp://admin:bst12345678@192.168.2.188:554/cam/realmonitor?channel=1&subtype=0";
	std::thread t1(func, mRtsp);

	char *mRtsp2 = rtsp2;// "rtsp://admin:bst12345678@192.168.2.188:554/cam/realmonitor?channel=1&subtype=0";
	std::thread t2(func, mRtsp);

	t1.join();
	t2.join();

	return 0;
}