// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

extern "C" {
#include "libavformat/avformat.h"
}

#include <iostream>

using namespace std;

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")

int onError(int errNum) {
	char buf[1024] = { 0 };
	av_strerror(errNum, buf, sizeof(buf));
	cout << buf << endl;
	return -1;
}

int main(int argc, char* argv[]) {

	//初始化封装和解封装	
	//av_register_all();
	avformat_network_init();

	AVFormatContext* ictx = NULL;
	char inUrl[] = "assets/test.flv";
	char outUrl[] = "rtmp://192.168.204.130/live";

	//输入流 1 打开文件，解封装
	//输入封装上下文
	int result = avformat_open_input(&ictx, inUrl, 0, 0);

	cout << result << endl;
	if (result != 0) {

		return onError(result);
	}

	result = avformat_find_stream_info(ictx, 0);

	av_dump_format(ictx, 0, inUrl, 0);

	//输出流

	//创建输出流上下文
	AVFormatContext* octx = NULL;
	result = avformat_alloc_output_context2(&octx, 0, "flv", outUrl);
	if (!octx) {
		return onError(result);
	}
	cout << "octx create success!" << endl;

	//配置输出流
	//遍历输入的AVSream
	for (int i = 0;i < ictx->nb_streams;i++) {
		AVCodec* codec = avcodec_find_decoder(ictx->streams[i]->codecpar->codec_id);
		AVStream* out = avformat_new_stream(octx, codec);
		if (!out) {
			return onError(0);
		}

		//复制配置信息，用于MP4
		//result = avcodec_copy_context(out->codec, ictx->streams[i]->codec);
		result = avcodec_parameters_copy(out->codecpar, ictx->streams[i]->codecpar);
		out->codecpar->codec_tag = 0;
	}
	av_dump_format(octx, 0, outUrl, 1);


	//rtmp推流

	//打开io
	result = avio_open(&octx->pb, outUrl, AVIO_FLAG_WRITE);
	if (!octx->pb) {
		return onError(result);
	}

	//写入头信息
	result = avformat_write_header(octx, 0);
	if (result < 0) {
		return onError(result);
	}
	cout << "avformat_write_header " << result << endl;


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
