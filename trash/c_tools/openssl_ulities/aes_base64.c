/*
 * file des - openssl�е�AES CBCģʽ���ܺ�base64����
 * 
 * date - 2016.05.23
 * author - lc
 */


#include "aes_base64.h"



/* ��Կ������ֵΪ������� */
unsigned char	rkey[16] = {0xad, 0x1a, 0x21, 0x72, 0xed, 0x9f, 0x98, 0x6d, 0x6e, 0xf9, 0x31, 0xfa, 0x78, 0x9a, 0xa0, 0x5f}; 

unsigned char	iv[AES_BLOCK_SIZE * 4] = { 0x47, 0xbc, 0xfe, 0x9d, 0x80, 0xdb, 0x3c, 0xfe, 0xd1, 0x09, 0x4d, 0xd9, 0x70, 0x43, 0xa6, 0xbf,\
										   0x9a, 0xe1, 0x94, 0xc1, 0xa2, 0xb2, 0x24, 0xfb, 0x0d, 0x5d, 0x7f, 0xb0, 0x54, 0xc3, 0xe3, 0x90,\
										   0xb1, 0x2f, 0xfe, 0xd2, 0xa5, 0x81, 0x89, 0xaf, 0xf1, 0x6c, 0x13, 0xf0, 0x11, 0x85, 0x17, 0x1a,\
										   0xd1, 0xba, 0xde, 0x58, 0x42, 0x20, 0x30, 0x5e, 0xfe, 0xe8, 0xac, 0x70, 0xa5, 0xe6, 0x1f, 0xaf\
										  };  

/*
 * des - aes���ܺ���
 * param - plaintext: ��������
 * 		   ciphertext: ���ܺ����ģ�CBCģʽ���ܳ��������ĳ��Ȼ��㹫ʽ cipher_len = (plain_len/16 + 1) *16
 *		   len: �������ݵĳ���
 * ret - void		
 */
int aes_encrypte(const u_char *plaintext, u_char *ciphertext, u_int len)
{
	
	AES_KEY key; 
	unsigned char	inner_iv[AES_BLOCK_SIZE * 4];  

	int nr_of_bits = 0;  

	nr_of_bits = 8 * sizeof(rkey);  
	
	memcpy(inner_iv, iv, sizeof(iv));  
	/* ���ɼ�����Կ */
	AES_set_encrypt_key(rkey, nr_of_bits, &key); 
	
	
	AES_cbc_encrypt(plaintext,  ciphertext, len,  &key,  inner_iv,  AES_ENCRYPT);  

	return ((len/16 + 1) *16);
}

/*
 * des - aes���ܺ���
 * param - ciphertext: ��������
 * 		   plaintext: ���ܺ���������
 *		   len: �������ݳ��ȣ�CBCģʽ��bytesΪ��λ���м��ܣ���len������һ���ݴ�ռ䣬���ܲ���������鴫���������ݳ��ȡ�
 *
 * ret - void		
 */
void aes_decrypte(const u_char *ciphertext, u_char *plaintext, u_int len)
{
	AES_KEY key; 
	
	unsigned char	inner_iv[AES_BLOCK_SIZE * 4];  
	int nr_of_bits = 0;  

	memcpy(inner_iv, iv, sizeof(iv));  
	nr_of_bits = 8 * sizeof(rkey);  
	AES_set_decrypt_key(rkey, nr_of_bits, &key);  

	AES_cbc_encrypt(ciphertext, plaintext,  len, &key, inner_iv, AES_DECRYPT);  
}

/*
 * des - base64���룬���������ַ���64���ɴ�ӡ�ַ����ж�Ӧת���� 2^6 = 64��6bits��Ӧһ���ַ��������ֽڶ�Ӧ�ĸ��ɼ��ַ�
 * param - str : ����������
 *		   str_len : str�ĳ���
 *		   encode : ��������ݴ洢
 *		   encode_len : encode�������ĳ��ȣ�Ҫ���С�����str_len������Ϊ(str_len/3) * 4 + 8
 * ret - success : ���������ʵ�ʳ���
 * 		 fail : -1
 */
int base64_encode(char *str,int str_len,char *encode, u_int encode_len)
{
	BIO *bmem,*b64;
	BUF_MEM *bptr;

	b64=BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem=BIO_new(BIO_s_mem());
	b64=BIO_push(b64, bmem);

	BIO_write(b64, str, str_len); //encode
	BIO_flush(b64);
	BIO_get_mem_ptr(b64,&bptr);

//	printf("%d\n", bptr->length);

	if(bptr->length > encode_len){
		 printf("encode_len too small\n");
		 return -1; 
	}   

	encode_len=bptr->length;
	memcpy(encode,bptr->data,bptr->length);
	encode[bptr->length] = '\0';
	BIO_free_all(b64);

	return encode_len;
}

/*
 * des - base64���뺯������������ݳ���С��ԭ����
 */
/*
 * des - base64���뺯��
 * param - str : ����������
 *		   str_len : str�ĳ���
 *		   encode : ��������ݴ洢
 *		   encode_len : decode�������ĳ��ȣ���С�ο����뺯��
 *
 * ret - ����������ݵĳ���
 */
int base64_decode(char *str,int str_len,char *decode,int decode_buffer_len)
{
	 int len=0;
	 BIO *b64,*bmem;
	 
	 b64=BIO_new(BIO_f_base64());
	 BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	 
	 bmem=BIO_new_mem_buf(str,str_len);
	 bmem=BIO_push(b64,bmem);
	 
	 len=BIO_read(bmem,decode,str_len);
	 
	 decode[len]=0;
	 
	 BIO_free_all(bmem);
//	 printf("len = %d\n", len);
	 
	 return len;
}

int act_code_encrypt_encode(char *act_code, u_int len, char *encode_buf, u_int encode_buf_len)
{
	if((NULL == act_code) || (NULL == encode_buf))
		return -1;
	if(encode_buf_len < len)	/* �����ж� */
		return -1;
	
	/* ��֪act_code���ȣ����64bytes�㹻 */
	unsigned char	ciphertext[AES_BLOCK_SIZE * 4];
	
	aes_encrypte(act_code, ciphertext, len);
	
	u_int ciphertext_len = ((len/16) + 1) * 16;		/* 48 bytes */

    return base64_encode(ciphertext, ciphertext_len, encode_buf, encode_buf_len);		
}

int act_code_decrypt_decode(char *encode, u_int encode_len, char *act_code, u_int act_code_len)
{
	
	unsigned char	ciphertext[AES_BLOCK_SIZE * 4];  
	int len = 0;
	
	len = base64_decode(encode, encode_len, ciphertext, AES_BLOCK_SIZE * 4 - 1);
		
	aes_decrypte(ciphertext, act_code, len);
}

#if 0
int main(int argc, char **argv)  
{  
        unsigned char   plaintext[AES_BLOCK_SIZE * 4] =  "0664064IGAL7IA7DYAH58HOJJA8O0547RFYJ7D1D";
        unsigned char   checktext[AES_BLOCK_SIZE * 4]; 
		unsigned char   base64[80]; 
		
		unsigned char	ciphertext[AES_BLOCK_SIZE * 4];  
  		
		u_int ciphertext_len = ((strlen(plaintext)/16) + 1) * 16; 	/* 48 bytes */
		printf("plaintext: %s\n", plaintext);
		aes_encrypte(plaintext, ciphertext, strlen(plaintext));
		int len = 0;
        base64_encode(ciphertext, ciphertext_len, base64, 80);
		
		printf("base64: %s", base64);
//		aes_decrypte(ciphertext, checktext, sizeof(ciphertext));
        unsigned char   ciphertext_2[AES_BLOCK_SIZE * 4];  

		base64_decode(base64, 65, ciphertext_2, 64);
		
		aes_decrypte(ciphertext_2, checktext,45);
		
		printf("checktext: %s\n", checktext);
		printf("ori len = %d, %d, cipher len = %d, base64 len = %d\n",strlen(plaintext), (40/16), ciphertext_len, strlen(base64));
		
        return 0;  
}  

#endif

#if 0
int main()
{
	unsigned char   plaintext[] =  "0664064IGAL7IA7DYAH58HOJJA8O0547RFYJ7D1D";
	unsigned char   base64[80]; 
	unsigned char check_out[64];
	int base_len;
	printf("plaintext : %s\n", plaintext);
	base_len = act_code_encrypt_encode(plaintext, strlen(plaintext), base64, 80);
	printf("encodetext : %s\n", base64);

	act_code_decrypt_decode(base64,  base_len, check_out, 64);
	printf("check_out: %s\n", check_out);
}


#endif


