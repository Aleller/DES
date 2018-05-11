#include <stdio.h>
#include <stdbool.h>

void changeStringToBit(char *plaintext_str[16], int *plaintext[64]);

int main() {
	/*以后可以尝试直接把输入的字符串转换成二进制数组
	反正DES只处理二进制数组，这里只是把十六进制数字（以字符串形式存储）转换成二进制数组
	*/

	//先以字符串存储输入的十六进制数字
	char plaintext_str[16] = "0123456789ABCDEF";
	char key_str[16] = "fedcba9876543210";
	
	//输入
	printf("输入和输出都是十六进制数字\n\n");
	printf("请输入明文（64bit，即16位十六进制数字（小写））：");
	//scanf("%s", plaintext_str);
	printf("请输入密码（64bit，即16位十六进制数字（小写））：");
	//scanf("%s", key_str);

	//把字符串状态的十六进制数字转换成二进制数组
	int plaintext[64];
	int key[64];
	changeStringToBit(plaintext_str, plaintext);
	changeStringToBit(key_str, key);

	return 0;
}

void changeStringToBit(char *plaintext_str, int *plaintext) {
	for (int i = 0; i < 16; i++) {
		if (plaintext_str[i] >= 'A' && plaintext_str[i] <= 'F') {
			plaintext_str[i] += 32;
		}

		int valueDec = 0;

		if ((plaintext_str[i] >= '0') && (plaintext_str[i] <= '9')) {
			valueDec = plaintext_str[i] - '0';
		}
		else {
			valueDec = plaintext_str[i] - 'a' + 10;
		}

		int valueBin0 = 0;
		int valueBin1 = 0;
		int valueBin2 = 0;
		int valueBin3 = 0;
		
		valueBin3 = valueDec % 2;
		valueDec = valueDec / 2;
		valueBin2 = valueDec % 2;
		valueDec = valueDec / 2;
		valueBin1 = valueDec % 2;
		valueDec = valueDec / 2;
		valueBin0 = valueDec % 2;
		
		plaintext[i * 4 + 0] = valueBin0;
		plaintext[i * 4 + 1] = valueBin1;
		plaintext[i * 4 + 2] = valueBin2;
		plaintext[i * 4 + 3] = valueBin3;
	}
}