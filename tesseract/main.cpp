#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string.h>

using namespace std;

//  Utf-8 转 GBK  C风格字符串版本
char* ConvertUtf8ToGBK(char* strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL, 0);
	WCHAR* wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	strcpy(strUtf8, szGBK);
	delete[] szGBK;
	delete[] wszGBK;
	return strUtf8;
}
//  Utf-8 转 GBK  string版本包装C风格字符串版本
string& ConvertUtf8ToGBK(string& strUtf8)
{
	char* psUTF8ToGBK = new char[strUtf8.size() + 1];
	strcpy(psUTF8ToGBK, strUtf8.c_str());
	strUtf8 = ConvertUtf8ToGBK(psUTF8ToGBK);
	delete[] psUTF8ToGBK;
	return strUtf8;
}

void outputTxtFile(const char* outputFileName, char* outText)
{
	FILE* fout = fopen(outputFileName, "w");
	fwrite(outText, 1, strlen(outText), fout);// 将识别结果写入输出文件
	fclose(fout);
	return;
}

char* recognizeText(const char* fileName, const char* language)
{
	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, language)) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	// 读取个人配置文件，位于./Tesseract-OCR/tessdata 目录下
	api->ReadConfigFile("write_images");
	// Open input image with leptonica library
	Pix *image = pixRead(fileName);
	api->SetImage(image);
	// Get OCR result
	outText = api->GetUTF8Text();
	// 	int *aaa = api->AllWordConfidences();
	// 	while (*aaa != '\0')
	// 	{
	// 		printf("%d\n", *aaa);
	// 		aaa++;
	// 	}
	string outString;
	outString = outText;
	outString = ConvertUtf8ToGBK(outString);
	printf("OCR output:\n%s", outString.c_str());
	// Destroy used object and release memory
	api->End();
	pixDestroy(&image);
	return outText;
}

int main()
{
	DWORD start_time = GetTickCount();
	cout << "Recognizing image...please wait on..." << endl;

	const char* fileName = "E:/[code]/Tesseract/tesseract/test4.png";
	const char* outputFileName = "E:/[code]/Tesseract/tesseract/test4.txt";

	char* outText = recognizeText(fileName, "chidri+chi_sim");
	outputTxtFile(outputFileName, outText);

	DWORD end_time = GetTickCount();
	cout << "Processing is completed.";
	cout << "The run time is:" << (end_time - start_time) / 1000.0 << "s!" << endl;
	getchar();
	return 0;
}

