#include"JPEGDecode.h"
#include"BMPheader.h"
#include"JPEGEncode.h"
#include"sha512.h"
#include<sstream>
void testDC() {
	using namespace std;
	vector<int> ori;
	vector<int> res;
	fstream test("ori.txt");
	while (test.good()) {
		int temp;
		test >> temp;
		ori.push_back(temp);
	}
	test.close();
	test.open("res.txt");
	while (test.good()) {
		int temp;
		test >> temp;
		res.push_back(temp);
	}
	test.close();

	int count = 0;
	int error = 0;
	vector<int>::iterator ori_it = ori.begin();
	vector<int>::iterator res_it = res.begin();
	for (; ori_it != ori.end(); ++ori_it, ++res_it) {
		if (*ori_it != *res_it) {
			error++;
		}
		count++;
	}
	cout << "DC变化率测试： " << (double)error / count << endl;
	return;
}
void testAC() {
	using namespace std;
	vector<int> ori;
	vector<int> res;
	fstream test("oriAC.txt");
	while (test.good()) {
		int temp;
		test >> temp;
		ori.push_back(temp);
	}
	test.close();
	test.open("resAC.txt");
	while (test.good()) {
		int temp;
		test >> temp;
		res.push_back(temp);
	}
	test.close();

	int count = 0;
	int error = 0;
	vector<int>::iterator ori_it = ori.begin();
	vector<int>::iterator res_it = res.begin();
	for (; ori_it != ori.end(); ++ori_it, ++res_it) {
		if (*ori_it != *res_it) {
			error++;
		}
		count++;
	}
	cout << "AC变化率测试： " << (double)error / count << endl;
	return;
}

int main(int argc, char *argv[]) {
	using namespace std;
	try {
		if (argc != 3) {
			throw "Usage: exe_file  bmp jpg";// 用法: 可执行文件  图像
		}
		{
			ofstream file("seed.dat", ios::binary);
			double a1 = 1.5121615730212, a2 = 12.102462138545, a3 = 9.4525140151405, a4 = 0.49161415934378, a5 = 0.63726194247947;
			file.write((const char*)&a1, sizeof(double));
			file.write((const char*)&a2, sizeof(double));
			file.write((const char*)&a3, sizeof(double));
			file.write((const char*)&a4, sizeof(double));
			file.write((const char*)&a5, sizeof(double));
			file.close();
		}
		{
			string jpgNewName;
			jpgNewName = argv[1];
			jpgNewName.erase(jpgNewName.length() - 4, 4);
			jpgNewName += ".jpg";
			BMPDecode bmpRead(argv[1]);
			cout << "BMP decode complete." << endl;
			clock_t time = clock();
			if (bmpRead.getNumberOfColor() == 3) {
#ifndef ENCRYPT
				JPEGEncode
#else
				JPEGEncodeEncrypt
#endif
					encode(jpgNewName, bmpRead.getWidth(), bmpRead.getHeight(), bmpRead.getR(), bmpRead.getG(), bmpRead.getB());
			}
			else {
				throw "grey jpeg encoding unsupported now";
			}
			cout << "JPEG encode complete: " << (double)(clock() - time) / CLOCKS_PER_SEC << 's' << endl;
		}
		{
			string bmpNewName;
			bmpNewName = argv[2];
			bmpNewName.erase(bmpNewName.length() - 4, 4);
			bmpNewName += "_new.bmp";
			clock_t time = clock();
#ifndef ENCRYPT
			JPEGDecode
#else
			JPEGDecodeDecrypt
#endif
				decode(argv[2]);//jpg_name
			cout << "JPEG decode complete: " << (double)(clock() - time) / CLOCKS_PER_SEC << 's' << endl;
			if (decode.getNumberOfColor() == 3) {
				BMPEncode bmpWrite(bmpNewName, decode.getWidth(), decode.getHeight(), 24, decode.getR(), decode.getG(), decode.getB());
			}
			else {
				BMPEncode bmpWrite(bmpNewName, decode.getWidth(), decode.getHeight(), 8, decode.getY());
			}
			cout << "BMP encode complete." << endl;
		}
	}
	catch (const char *error) {
		cerr << "--------------------------------------------------E---R---R---O---R----------------------------------------------------" << endl;
		cerr << error << endl;
		system("pause");
		return 1;
	}
	catch (exception &e) {
		cerr << "--------------------------------------------------E---R---R---O---R----------------------------------------------------" << endl;
		cerr << "Standard exception caught: " << e.what() << endl;
		system("pause");
		return 1;
	}

	testDC();
	testAC();

	return 0;
}
