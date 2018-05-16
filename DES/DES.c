//输入（包括明文和密码）：十六进制数字
//输出（密文）：十六进制数字

#include <math.h>
#include <stdio.h>

void changeHexStringToBin(char *plaintext_str, int *plaintext);
void f(int *result, int *plaintextRight, int *subKey);
void XOR(int *result, int *a, int *b);
void changeBinToHexString(char *hexStr, int *bin);

int main() {
	/*以后可以尝试直接把输入的字符串转换成二进制数组
	反正DES只处理二进制数组，这里只是把十六进制数字（以字符串形式存储）转换成二进制数组
	*/

	/*先以字符串存储输入的十六进制数字*/
	char plaintext_str[16] = "0123456789ABCDEF";
	char key_str[16] = "fedcba9876543210";
	
	/*输入*/
	printf("输入和输出都是十六进制数字\n\n");
	printf("请输入明文（64bit，即16位十六进制数字（小写））：");
	//scanf("%s", plaintext_str);
	printf("请输入密码（64bit，即16位十六进制数字（小写））：");
	//scanf("%s", key_str);

	/*把字符串状态的十六进制数字转换成二进制数组*/
	int plaintext[64];
	int key[64];
	changeHexStringToBin(plaintext_str, plaintext);
	changeHexStringToBin(key_str, key);

	/*初始置换*/
	int IP[64] = {
		58,50,42,34,26,18,10,2,
		60,52,44,36,28,20,12,4,
		62,54,46,38,30,22,14,6,
		64,56,48,40,32,24,16,8,
		57,49,41,33,25,17,9,1,
		59,51,43,35,27,19,11,3,
		61,53,45,37,29,21,13,5,
		63,55,47,39,31,23,15,7
	};

	int plaintext_AfterIP[64];
	for (int i = 0; i < 64;i++) {
		plaintext_AfterIP[i] = plaintext[IP[i]-1];/*注意数组下标！！！*/
	}

	/*处理秘钥，产生每轮迭代所需要的共16个子秘钥，每个子秘钥48位*/
	int subKey[16][48];
	int key_AfterP1[56];

	int key_P1[56] = {//进行置换选择1
		57,49,41,33,25,17,9,
		1,58,50,42,34,26,18,
		10,2,59,51,43,35,27,
		19,11,3,60,52,44,36,
		63,55,47,39,31,23,15,
		7,62,54,46,38,30,22,
		14,6,61,53,45,37,29,
		21,13,5,28,20,12,4
	};
	for (int i = 0;i < 56;i++) {
		key_AfterP1[i] = key[key_P1[i] - 1];
	}

	int subKeyLeft[28];//产生子秘钥的左部和右部
	int subKeyRight[28];
	for (int i = 0;i < 28;i++) {
		subKeyLeft[i] = key_AfterP1[i];
	}
	for (int i = 28;i < 56;i++) {
		subKeyRight[i - 28] = key_AfterP1[i];
	}
	
	for (int subKeyNum = 0;subKeyNum < 16;subKeyNum++) {
		if (subKeyNum == 0 || subKeyNum == 1 || subKeyNum == 8 || subKeyNum == 15) {//循环左移两位
			int tempLeft0 = subKeyLeft[0];
			int tempLeft1 = subKeyLeft[1];
			int tempRight0 = subKeyRight[0];
			int tempRight1 = subKeyRight[1];

			for (int subKeyBit = 0;subKeyBit < 26;subKeyBit++) {
				subKeyLeft[subKeyBit] = subKeyLeft[subKeyBit + 2];
				subKeyRight[subKeyBit] = subKeyRight[subKeyBit + 2];
			}

			subKeyLeft[26] = tempLeft0;
			subKeyLeft[27] = tempLeft1;
			subKeyRight[26] = tempRight0;
			subKeyRight[27] = tempRight1;
		}
		else {//循环左移一位
			int tempLeft = subKeyLeft[0];
			int tempRight = subKeyRight[0];

			for (int subKeyBit = 0;subKeyBit < 27;subKeyBit++) {
				subKeyLeft[subKeyBit] = subKeyLeft[subKeyBit + 1];
				subKeyRight[subKeyBit] = subKeyRight[subKeyBit + 1];
			}

			subKeyLeft[27] = tempLeft;
			subKeyRight[27] = tempRight;
		}
		int key_P2[48] = {//进行置换选择2
			14,17,11,24,1,5,
			3,28,15,6,21,10,
			23,19,12,4,26,8,
			16,7,27,20,13,2,
			41,52,31,37,47,55,
			30,40,51,45,33,48,
			44,49,39,56,34,53,
			46,42,50,36,29,32
		};
		for (int subKeyBit = 0;subKeyBit < 24;subKeyBit++) {
			subKey[subKeyNum][subKeyBit] = subKeyLeft[key_P2[subKeyBit] - 1];
		}
		for (int subKeyBit = 24;subKeyBit < 48;subKeyBit++) {
			subKey[subKeyNum][subKeyBit] = subKeyRight[key_P2[subKeyBit] - 28 - 1];
		}
	}

	/*进行十六轮迭代*/
	int plaintextLeft[32];
	int plaintextRight[32];
	for (int i = 0;i < 32;i++) {
		plaintextLeft[i] = plaintext_AfterIP[i];
	}
	for (int i = 32;i < 64;i++) {
		plaintextRight[i - 32] = plaintext_AfterIP[i];
	}
	for (int i = 0;i < 16;i++) {//迭代开始
		int plaintextLeftNew[32];
		int plaintextRightNew[32];
		for (int j = 0;j < 32;j++) {
			plaintextLeftNew[j] = plaintextRight[j];
		}

		int temp[32];
		f(temp, plaintextRight, subKey[i]);
		XOR(plaintextRightNew, temp, plaintextLeft);

		for (int i = 0;i < 32;i++) {
			plaintextLeft[i] = plaintextLeftNew[i];
			plaintextRight[i] = plaintextRightNew[i];
		}
	}

	int resultBeforeIP_1[64];
	for (int i = 0;i < 32;i++) {
		resultBeforeIP_1[i] = plaintextLeft[i];
		resultBeforeIP_1[i + 32] = plaintextRight[i];
	}

	/*进行逆初始置换*/
	int IP_1[64] = {
		40,8,48,16,56,24,64,32,
		39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30,
		37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28,
		35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26,
		33,1,41,9,49,17,57,25
	};

	int resultBin[64];
	for (int bit = 0;bit < 64;bit++) {
		resultBin[bit] = resultBeforeIP_1[IP_1[bit] - 1];
	}

	char resultStr[17];

	changeBinToHexString(resultStr, resultBin);

	printf("\n密文为：%s\n", resultStr);

	return 0;
}

void changeHexStringToBin(char *plaintext_str, int *plaintext) {//将输入的字符串转化为比特数组
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

void f(int *result/*32bit*/, int *plaintextRight/*32bit*/, int *subKey/*48bit*/) {
	int E[48] = {
		32,1,2,3,4,5,
		4,5,6,7,8,9,
		8,9,10,11,12,13,
		12,13,14,15,16,17,
		16,17,18,19,20,21,
		20,21,22,23,24,25,
		24,25,26,27,28,29,
		28,29,30,31,32,1
	};
	int afterExtend[48];
	for (int bit = 0;bit < 48;bit++) {//进行E盒扩展
		afterExtend[bit] = plaintextRight[E[bit] - 1];
	}
	int beforeSBox[48];
	for (int bit = 0;bit < 48;bit++) {//进行异或
		if ((subKey[bit] == 0) && (afterExtend[bit] == 0)) {
			beforeSBox[bit] = 0;
			continue;
		}
		if ((subKey[bit] == 1) && (afterExtend[bit] == 1)) {
			beforeSBox[bit] = 0;
			continue;
		}
		if ((subKey[bit] == 0) && (afterExtend[bit] == 1)) {
			beforeSBox[bit] = 1;
			continue;
		}
		if ((subKey[bit] == 1) && (afterExtend[bit] == 0)) {
			beforeSBox[bit] = 1;
			continue;
		}
		printf("f()	error\n");
	}

	int SBox[8][4][16] = {//进行S盒压缩，有8个S盒，每个S盒4行16列，现在有第2个
		//0,done
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,

		//1,done
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,

		//2,done
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,

		//3,done
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,

		//4,done
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,

		//5,done
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,

		//6,done
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,

		//7
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	};

	for (int bitGroup = 0;bitGroup < 8; bitGroup++) {
		int row = beforeSBox[bitGroup * 6] * 2 + beforeSBox[bitGroup * 6 + 5] * 1;
		int column = beforeSBox[bitGroup * 6 + 1] * 8 + beforeSBox[bitGroup * 6 + 2] * 4 + beforeSBox[bitGroup * 6 + 3] * 2 + beforeSBox[bitGroup * 6 + 4] * 1;
		
 		int sBoxOutput = SBox[bitGroup][row][column];

		int sBoxOutputBin[4];
		sBoxOutputBin[3] = sBoxOutput % 2;
		sBoxOutput = sBoxOutput / 2;
		sBoxOutputBin[2] = sBoxOutput % 2;
		sBoxOutput = sBoxOutput / 2;
		sBoxOutputBin[1] = sBoxOutput % 2;
		sBoxOutput = sBoxOutput / 2;
		sBoxOutputBin[0] = sBoxOutput % 2;

		result[bitGroup * 4 + 0] = sBoxOutputBin[0];
		result[bitGroup * 4 + 1] = sBoxOutputBin[1];
		result[bitGroup * 4 + 2] = sBoxOutputBin[2];
		result[bitGroup * 4 + 3] = sBoxOutputBin[3];
	}

	return;
}

void XOR(int *result/*32bit*/, int *a/*32bit*/, int *b/*32bit*/) {
	for (int bit = 0;bit < 32;bit++) {
		if ((a[bit] == 0) && (b[bit] == 0)) {
			result[bit] = 0;
			continue;
		}
		if ((a[bit] == 1) && (b[bit] == 1)) {
			result[bit] = 0;
			continue;
		}
		if ((a[bit] == 0) && (b[bit] == 1)) {
			result[bit] = 1;
			continue;
		}
		if ((a[bit] == 1) && (b[bit] == 0)) {
			result[bit] = 1;
			continue;
		}
		printf("XOR() error!\n");
	}
	return;
}

void changeBinToHexString(char *hexStr, int *bin) {
	for (int ch = 0;ch < 16;ch++) {
		int tempSum = 0;
		tempSum += bin[ch * 4 + 0] * 8;
		tempSum += bin[ch * 4 + 1] * 4;
		tempSum += bin[ch * 4 + 2] * 2;
		tempSum += bin[ch * 4 + 3] * 1;
		if (tempSum < 10) {
			hexStr[ch] = '0' + tempSum;
		}
		else {
			hexStr[ch] = 'a' + (tempSum - 10);
		}
	}
	hexStr[16] = '\0';
	return;
}