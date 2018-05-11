#include <stdio.h>
#include <stdbool.h>

void changeStringToBit(char *plaintext_str[16], int *plaintext[64]);

int main() {
	/*�Ժ���Գ���ֱ�Ӱ�������ַ���ת���ɶ���������
	����DESֻ������������飬����ֻ�ǰ�ʮ���������֣����ַ�����ʽ�洢��ת���ɶ���������
	*/

	//�����ַ����洢�����ʮ����������
	char plaintext_str[16] = "0123456789ABCDEF";
	char key_str[16] = "fedcba9876543210";
	
	//����
	printf("������������ʮ����������\n\n");
	printf("���������ģ�64bit����16λʮ���������֣�Сд������");
	//scanf("%s", plaintext_str);
	printf("���������루64bit����16λʮ���������֣�Сд������");
	//scanf("%s", key_str);

	//���ַ���״̬��ʮ����������ת���ɶ���������
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