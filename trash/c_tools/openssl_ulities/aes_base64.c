/*
 * file des - openssl中的AES CBC模式加密和base64编码
 * 
 * date - 2016.05.23
 * author - lc
 */


#include "aes_base64.h"



/* 密钥与向量值为随机生成 */
unsigned char	rkey[16] = {0xad, 0x1a, 0x21, 0x72, 0xed, 0x9f, 0x98, 0x6d, 0x6e, 0xf9, 0x31, 0xfa, 0x78, 0x9a, 0xa0, 0x5f}; 

unsigned char	iv[AES_BLOCK_SIZE * 4] = { 0x47, 0xbc, 0xfe, 0x9d, 0x80, 0xdb, 0x3c, 0xfe, 0xd1, 0x09, 0x4d, 0xd9, 0x70, 0x43, 0xa6, 0xbf,\
										   0x9a, 0xe1, 0x94, 0xc1, 0xa2, 0xb2, 0x24, 0xfb, 0x0d, 0x5d, 0x7f, 0xb0, 0x54, 0xc3, 0xe3, 0x90,\
										   0xb1, 0x2f, 0xfe, 0xd2, 0xa5, 0x81, 0x89, 0xaf, 0xf1, 0x6c, 0x13, 0xf0, 0x11, 0x85, 0x17, 0x1a,\
										   0xd1, 0xba, 0xde, 0x58, 0x42, 0x20, 0x30, 0x5e, 0xfe, 0xe8, 0xac, 0x70, 0xa5, 0xe6, 0x1f, 0xaf\
										  };  

/*
 * des - aes加密函数
 * param - plaintext: 明文数据
 * 		   ciphertext: 加密后密文，CBC模式加密长度与明文长度换算公式 cipher_len = (plain_len/16 + 1) *16
 *		   len: 明文数据的长度
 * ret - void		
 */
int aes_encrypte(const u_char *plaintext, u_char *ciphertext, u_int len)
{
	
	AES_KEY key; 
	unsigned char	inner_iv[AES_BLOCK_SIZE * 4];  

	int nr_of_bits = 0;  

	nr_of_bits = 8 * sizeof(rkey);  
	
	memcpy(inner_iv, iv, sizeof(iv));  
	/* 生成加密密钥 */
	AES_set_encrypt_key(rkey, nr_of_bits, &key); 
	
	
	AES_cbc_encrypt(plaintext,  ciphertext, len,  &key,  inner_iv,  AES_ENCRYPT);  

	return ((len/16 + 1) *16);
}

/*
 * des - aes加密函数
 * param - ciphertext: 密文数据
 * 		   plaintext: 解密后明文数据
 *		   len: 密文数据长度，CBC模式以bytes为单位进行加密，该len长度有一定容错空间，解密不会出错。建议传入密文数据长度。
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
 * des - base64编码，将二进制字符与64个可打印字符进行对应转化。 2^6 = 64，6bits对应一个字符，三个字节对应四个可见字符
 * param - str : 需编码的数据
 *		   str_len : str的长度
 *		   encode : 编码后数据存储
 *		   encode_len : encode缓冲区的长度，要求大小需大于str_len，建议为(str_len/3) * 4 + 8
 * ret - success : 编码后数据实际长度
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
 * des - base64解码函数，解码后数据长度小于原数据
 */
/*
 * des - base64解码函数
 * param - str : 需解码的数据
 *		   str_len : str的长度
 *		   encode : 解码后数据存储
 *		   encode_len : decode缓冲区的长度，大小参考编码函数
 *
 * ret - 解码出来数据的长度
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
	if(encode_buf_len < len)	/* 粗略判断 */
		return -1;
	
	/* 已知act_code长度，因此64bytes足够 */
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


