#include"JPEGDecode.h"
#include"BMPheader.h"
#include"JPEGEncode.h"
#include"sha512.h"
#include<sstream>
int main(int argc, char *argv[]) {
	using namespace std;
	try {
		if (argc != 3) {
			throw "Usage: exe_file  bmp jpg";// 用法: 可执行文件  图像
		}
		{

			string sha = sha512file(argv[1]);
			std::stringstream stream;
			ofstream file("seed.dat", ios::binary);
			for (int i = 0; i < 5; ++i) {
				string temp(sha.substr(16 * i, 16));
				double seed;
				unsigned long long toULL;
				stream.clear();
				stream << hex << temp;
				stream >> hex >> toULL;
				seed = (double)toULL / (double)0xffffffffffffffffull;
				cout << seed << endl;
				file.write((const char*)&seed, sizeof(double));
			}
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
	return 0;
}
