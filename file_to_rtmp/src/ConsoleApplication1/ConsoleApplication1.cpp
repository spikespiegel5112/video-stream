// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

extern "C" {
#include "libavformat/avformat.h"
}

#include <iostream>

using namespace std;

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")

int onError(int errNum) {
	char buf[1024] = { 0 };
	av_strerror(errNum, buf, sizeof(buf));
	cout << buf << endl;
	return -1;
}

int main(int argc, char* argv[]) {
	//av_register_all();
	avformat_network_init();

	AVFormatContext* ictx = NULL;
	char inUrl[] = "assets/test.mp4";

	int result = avformat_open_input(&ictx, inUrl, 0, 0);

	cout << result << endl;
	if (result != 0) {

		return onError(result);
	}

	result = avformat_find_stream_info(ictx, 0);

	av_dump_format(ictx, 0, inUrl, 0);


	getchar();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
