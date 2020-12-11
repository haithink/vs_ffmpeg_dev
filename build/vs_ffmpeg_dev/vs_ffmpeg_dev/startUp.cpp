#include <iostream>
#include <thread>

extern "C" {
	int func(char *mRtsp);
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