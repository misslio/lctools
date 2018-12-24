/* filename: tmp.c
 * 
 * 生成密钥文件的shell命令,已包含公钥,1024 是位数，即128字节
 * 	openssl genrsa -out plainPrv.key 1024
 *
 * 提取公钥，生成不含有RSA的公钥文件
 * openssl rsa -in plainPrv.key -pubout -out plainPub.key
 * 提取公钥，生成含有RSA的公钥文件
 * openssl rsa -in plainPrv.key -RSAPublicKey_out -out plainPub.key

 * 生成使用3DES加密的私钥文件，之后会提示输入密码
 * openssl genrsa -des3 -out cipherPrv.key 1024
 * 提取不含RSA格式的公钥文件
 * openssl rsa -in cipherPrv.key -pubout -out cipherPub.key
 * 提取含有RSA格式的密钥文件
 * openssl rsa -in cipherPrv.key -RSAPublicKey_out -out cipherPub2.key
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>

#include "lc_rsa.h"


#ifdef DEBUG
#define PRINT_DEBUG(format, args...)	printf("[\033[32mDEBUG\033[0m] FUNC: %s LINE:%d INFO:"format, __func__, __LINE__, ##args)
#else
#define PRINT_DEBUG(format, args...)	do{;}while(0)
#endif

#define PRINT_ERR(format, args...)		fprintf(stderr, "[\033[31mERRG\033[0m] FUNC: %s LINE: %d INFO:"format, __func__, __LINE__, ##args)

#ifndef LC_FREE_MALLOC
#define LC_FREE_MALLOC(p) do\
	{\
		if((p) != NULL)\
		{\
			free(p);\
			(p) = NULL;\
		}\
	}while(0)
#endif


static unsigned char private_key_V[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIICXAIBAAKBgQCTEV9wHAac6xpivAamANJjiBDwn7lEAT5KGH+pKPDJQkrKonum\n\
g3/cWmGXHc0FnOZNyjkCIZeyw16SXcREt53eXoURSJiH+N4REN4WOHY18oWylnfa\n\
kcE+zuIXjNRcG/YGzDkqTwFa+nDZGVBzrAGDftPz7pA59CrSqbj5zN3tBQIDAQAB\n\
AoGBAJI17F8lzcFFzzan3ig9muzPAqBm+yZL4iSCFLEVfcVo/bhVL39NLtnglTNK\n\
smDJKwHdcS3YPdvAdg4n4I+8yNof5zAODSxNabHFsh68WLGNpnV70WpNP14SbUGL\n\
wXesXgLbW0DmR4GuTKs5SAtre95ytCknAsGgap74eh8QbCklAkEAwugoDUnSxQ8H\n\
nu6lojwYJTXz+gQ+0xsgitcvG4KMF9bHxS1HjtgXFrCNX+TgpSVlCvfewCEnuOPv\n\
W4ejvDQEOwJBAMEqfPr1tmYAvjydML6+/aggZABSWB5W3P4afaljPofDeztHgzx5\n\
3+YhfNmVLDxZcAeyf29l2N26adpW0lAI/78CQEAAG6bBZwRuOwJlOt5a/VK6tLKU\n\
Ukzfy3/LyRwvcfEiBdcpVyYArxQ0RTgH3/xamyNO4UYVnQWJUz+q+okCS6ECQCCe\n\
hypt8dY5y+Y6eS/M2tLYXWq8fQrqijkSyiOfkcClXxr8kYnV3txizU2E5kpuo/94\n\
whJStZOD1jo7rRo82ssCQApitXsrbKrGQGA6P6KmnE8eq46khkfczVZQZfeT85pX\n\
XfUpPuBlfKwxNOsIAkAAxaEFr2DaV/R7c4T5nTh8B+o=\n\
-----END RSA PRIVATE KEY-----\n";


static unsigned char public_key_V[] = "-----BEGIN RSA PUBLIC KEY-----\n\
MIGJAoGBAJMRX3AcBpzrGmK8BqYA0mOIEPCfuUQBPkoYf6ko8MlCSsqie6aDf9xa\n\
YZcdzQWc5k3KOQIhl7LDXpJdxES3nd5ehRFImIf43hEQ3hY4djXyhbKWd9qRwT7O\n\
4heM1Fwb9gbMOSpPAVr6cNkZUHOsAYN+0/PukDn0KtKpuPnM3e0FAgMBAAE=\n\
-----END RSA PUBLIC KEY-----\n";

/* 服务器加密，客户端解密密钥对 ，服务端发送数据*/
static unsigned char private_key_pair1[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIICXQIBAAKBgQDn3xqgSinF7+58n16c8ed1lNZWUN87TanMGnKTtXt88jYBFFHZ\n\
pkoNdeHX57FnaTMQ1z97jAbcFaMyIhVleKV5+ddT4+cNdpLQFEGrOHtjFq5rzfZQ\n\
kUV523IPChtfHjnifBcwRnpu2giL3i7Ef37zF/JgajeqMbpQAEtc2d7uUQIDAQAB\n\
AoGBAI4Is1W2uEoAhDUgpqM65062YmkzVtsL4tQc60yh4cgS0pPsr4rEex4ttK89\n\
xQUUguxw/fkfOGB+fJ5MFJE4yxt3JWFPeZmjZ7AJ3gd0oVr1xdMMTjzkBU3dNO/O\n\
W2Br+n+Z9cWCaOlIYQVlPUIHMDU5yKGmNRQX4ASMGZzk99zxAkEA+aZkVbNK2jek\n\
k5kZWX+2rShORHldC9Aa4SJPc9x5fEoo66nbn9WNr4uFfYXpBjPmkHsumflYzcTi\n\
5H5XGphWzQJBAO3E8lsZMYE2yxmEsP+vWeaggQqHZTTuEShL5y3ZO1RJHmLbAPJi\n\
0SbaA5oa0sKYDBOSsgCqMzwGpB8IbfX8DZUCQGnysKAiQAEOHj05eyWdasRf1599\n\
tfw+bDnhWsC7757MxkMFShyx/7hhSNy1iOhF/HKSpHtZ89MX3IbGC2dRAX0CQQDn\n\
fT3wD6ExCmXGnHevF8vRTCcWVyfj1nyB3RRggQTQVLfuywYq9qZmyw+Cs+bKajon\n\
vETtfMeRA246gHx+Zz3ZAkBpU+qbFZtrCmHrSfvkgQXjnj9znySP4TRcL9nZGBqM\n\
GRLnDWjX/zTJLQG9SOU+jItLC7m1HbwcOigLqkCnAhot\n\
-----END RSA PRIVATE KEY-----\n";


static unsigned char public_key_pair1[] = "-----BEGIN PUBLIC KEY-----\n\
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDn3xqgSinF7+58n16c8ed1lNZW\n\
UN87TanMGnKTtXt88jYBFFHZpkoNdeHX57FnaTMQ1z97jAbcFaMyIhVleKV5+ddT\n\
4+cNdpLQFEGrOHtjFq5rzfZQkUV523IPChtfHjnifBcwRnpu2giL3i7Ef37zF/Jg\n\
ajeqMbpQAEtc2d7uUQIDAQAB\n\
-----END PUBLIC KEY-----\n";

/* 客户端加密，服务端解密密钥对，服务端接收数据 */
static unsigned char private_key_pair2[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIICXAIBAAKBgQDp6kSLSTBEjtQVZCR2huCAM8zibWT6VlKXMbvZ2W32O3xlljlQ\n\
9fNAoHdDzgZl4w1AEcLh0uoBd/RiI+31sbrFsNvqtWOUTrnO4G1aLwM67KRTy7sZ\n\
sBYAzGGyz9kEWFIqJvBROxXnUukhJznYx88Xy7ElgexW74wi0SiaATL7rQIDAQAB\n\
AoGBAIB4U/ET31ioAT9GYuheJ4sB6DMr1dvP8Uhs3jlFu6kS/r1khlI64sDJ4Pby\n\
fn0S+MIJfzk7952a3jPQe+8HEAZavtcf/mGMZFcTDM4chZYd4SQfUwOU0Be8z0gP\n\
JktONlwNOYc2yjqppVO7GqVosqYgIDtdTbiS17Xg3XOMsyeZAkEA+nlTN0I8WAEe\n\
m+t1Bp9PhP0K+ajkI4bai9B8B4xZTuybN/lEZ12fjISFiU9G5XgxHBVltp+8f930\n\
oB5qJt+6RwJBAO8Taym19yVLJhR8bLVt5pKEdm+ANzqSKT9Hn74ctrnIt1Hts5DB\n\
IcgvdHtgVOTMxkM6ir1dmj9Qk9Ye91K+4GsCQCQooafHixiVSMrvidxj4UHsmcoC\n\
5LRwQlqOxWj9jOME6z+AX6ygamkJtJFm/XAhEs3gFOfxVXAMJcV7vo1QHP8CQEVZ\n\
6bUwzt+nnAVHTrWWitMnz79qfxrjeUmc+k7Iv4zHF76RaIxuKeGLOovrltTEPiMU\n\
YVirRWpmcmwjkXUUX78CQGiKoAeqAiOxEq/TcVrSbD/LGcDAkl1f5DQQv6grM11t\n\
VCorNBqUMlC1UJkkgJh02rjSA6Wm3rAYa1ZmFyQPkw8=\n\
-----END RSA PRIVATE KEY-----\n";


static unsigned char public_key_pair2[] = "-----BEGIN PUBLIC KEY-----\n\
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDp6kSLSTBEjtQVZCR2huCAM8zi\n\
bWT6VlKXMbvZ2W32O3xlljlQ9fNAoHdDzgZl4w1AEcLh0uoBd/RiI+31sbrFsNvq\n\
tWOUTrnO4G1aLwM67KRTy7sZsBYAzGGyz9kEWFIqJvBROxXnUukhJznYx88Xy7El\n\
gexW74wi0SiaATL7rQIDAQAB\n\
-----END PUBLIC KEY-----\n";



/* 从内存中读取PEM格式的公钥私钥内容 */
RSA* create_RSA(unsigned char* key, int flag)  
{  
    RSA *rsa= NULL;  
    BIO *keybio ;  
    keybio = BIO_new_mem_buf(key, -1);  /*  */
  
    if (keybio==NULL) {  
        printf( "Failed to create key BIO\n");  
        return 0;  
    }  
  
    if(PUB_KEY == flag)  
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);  /* 读取不带有"RSA"字样的公钥文件 */
//		rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);	/* 读取带有"RSA"字样的公钥文件 */
    else if(PRIV_KEY == flag)
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);  /* 私钥读取只有带"RSA"字样的版本 */
	
#if 0	/* 调试信息，以十六进制打印密钥到文件 */
	char file_name[20] = "./key_";
	int len = strlen(file_name);
	sprintf(file_name + len, "%d", flag);
	FILE *fp = fopen(file_name, "w+");
	if(NULL != fp){
		RSA_print_fp(fp, rsa, 0);
		fclose(fp);
	}
#endif	

    if(rsa == NULL)  
		PRINT_ERR("Failed to create RSA!");
	
    return rsa;  
}  

/*
 * param - rsa_ctx: 加密/解密密钥
 *		   instr: 输入字符，明文/密文
 *		   inlen: instr的长度
 *		   outstr: 输出字符串，函数内部申请，调用成功则由调用者释放
 *		   type: 加密或解密标识
 */
static int rsa_handle(RSA* rsa_ctx, char *instr, int inlen, char** outstr, int type)
{
    if(rsa_ctx == NULL || instr == NULL)
    {
        PRINT_ERR( "Input param error,please check them!");
        return -1;
    }
	
    int rsa_len, ret;
    rsa_len=RSA_size(rsa_ctx);
    *outstr=(unsigned char *)malloc(rsa_len+1);
    memset(*outstr,0,rsa_len+1);
	
    switch(type)
	{
        case PUB_KEY: /* 公钥加密 */
	        ret = RSA_public_encrypt(inlen, (unsigned char *)instr, (unsigned char*)*outstr, rsa_ctx, RSA_PKCS1_PADDING);
	        break;
			
        case PRIV_KEY: /* 私钥解密 */
        	ret = RSA_private_decrypt(inlen, (unsigned char *)instr, (unsigned char*)*outstr, rsa_ctx, RSA_PKCS1_PADDING);   
			break;
			
        default:
			ret = -1;
			break;
    }

	RSA_free(rsa_ctx);
	
    if(ret == -1)
    {
        PRINT_ERR("Got error on %d!", type);
err:
        LC_FREE_MALLOC(*outstr);
    }
	
    return ret;
}

/*
 * outstr : 存放加密/解密后的内容，由调用者free
 */
int rsa_base64(char *instr, int inlen, char** outstr, int type)
{
	RSA *rsa = NULL;
	int ret = -1, len = 0;
	char *out = NULL;

	switch(type){
		case PUB_KEY:	/* 加密 */
			rsa = create_RSA(public_key_pair1, type);
			if(NULL == rsa){
				ret = -1;
				break;
			}
			
			ret = rsa_handle(rsa, instr, inlen, &out, type);
			if(ret < 0){
				goto rsa_base64_err_exit;
			}
//			printf("encrypt len = %d\n", ret);
			
			len = (ret/3) * 4 + 8 * 2;
			*outstr = (char *)malloc(len);
			if(NULL == *outstr){
				ret = -1;
				goto rsa_base64_err_exit;
			}
			#if 0
			memcpy(*outstr, out, ret);
			return ret;
			#endif
			ret = base64_encode(out, ret , *outstr, len);
			len = ret;
//			printf("base64 len = %d\n", ret);
			break;
			
		case PRIV_KEY:		/* 解密，先解码然后解密 */
			out = (char *)malloc(inlen);
			if(NULL == out)
				return -1;
			
			memset(out, 0, inlen);
			ret = base64_decode(instr, inlen, out, inlen);
//			printf("ret = %d\n", ret);
			if(ret < 0){
				goto rsa_base64_err_exit;
			}
			rsa = create_RSA(private_key_pair2, type);
			if(NULL == rsa){
				ret = -1;
				goto rsa_base64_err_exit;
			}
			ret = rsa_handle(rsa, out, ret, outstr, type);
			len = ret;	/* 只是为了同步加密返回值 */
			break;
			
		default:
			ret = -1;
			break;
	}

rsa_base64_err_exit:
	LC_FREE_MALLOC(out);
	if(ret >= 0)
		return len;
	return ret;
}


int just_test()
{
	char buf[] = "DATE:20160831,HDCODE:C89EC44082CE64EA6F37347135C13848";
	char *en = NULL;
	rsa_base64(buf, strlen(buf), &en, PUB_KEY);
	printf("--------->%s\n", en);
	LC_FREE_MALLOC(en);
}

/*
 * des - 读取公钥文件加密str
 * param - str: 明文字符串
 * 		   path_key: 公钥文件路径
 *		   outstr: 密文存储，函数内部malloc申请，由调用者释放
 */
int rsa_pub_crypt(char *str,char *path_key,char** outstr)
{
    RSA *p_rsa;
    FILE *file;
    int flen, rsa_len, num;
	
    if((file=fopen(path_key, "r"))==NULL)
	{
        PRINT_ERR("Open key file error");
        return -1; 
    } 
	
#ifdef RSAPUBKEY
    if((p_rsa=PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL)
#else
    if((p_rsa=PEM_read_RSAPublicKey(file, NULL, NULL, NULL)) == NULL)
#endif
	{
        ERR_print_errors_fp(stdout);
        return -1;
    } 
	
    num = rsa_handle(p_rsa, str, strlen(str), outstr, PUB_KEY);
    RSA_free(p_rsa);
    fclose(file);
	
    return num;
}

/* 读取开头时说的3DES加密的文件，可是密码没有用，该函数搁置 */
RSA* getPRV(char *path_key_fullname, char* pwd)
{
    RSA *rsaK=RSA_new();
    OpenSSL_add_all_algorithms();
    BIO *BP = BIO_new_file(path_key_fullname,"rb");
    if(NULL == BP)
        return NULL;

    rsaK=PEM_read_bio_RSAPrivateKey(BP,NULL,NULL,pwd);
	
    return rsaK;
}


/*
 * des - 读取密钥文件加密str
 * param - str: 密文字符串
 * 		   path_key: 密钥文件路径
 *		   outstr: 明文存储，函数内部malloc申请，由调用者释放
 */
int rsa_prv_crypt(char *str, char *path_key, int inlen, char** outstr)
{    
    RSA *p_rsa;
    FILE *file;
    int rsa_len, ret = -1;

#if 1       
    if((file=fopen(path_key, "r"))==NULL)
	{
        PRINT_ERR("Open key file error");
        return -1;
    }
	
    if((p_rsa=PEM_read_RSAPrivateKey(file, NULL, NULL, NULL))==NULL)
	{
        ERR_print_errors_fp(stdout);
        return -1;
    }    
#else 	/* 采用加密后的密钥测了下，密码错误也可以跑?????? 呃，还是采用内存的方式吧 */
	p_rsa = getPRV(path_key, "sfsdf");
    ret = rsa_handle(p_rsa, str, inlen, outstr, PRIV_KEY);
#endif

    fclose(file);
	
    return ret;
}

#if 0
int main(int argc,char** argv)
{
	/* 公钥加密，私钥解密 */
	RSA* rsa_pub = create_RSA(public_key, PUB_KEY);
	char *instr = "I am the plain text!";
	char *out;
	int ret = 0;
   	ret = rsa_handle(rsa_pub, instr, strlen(instr), &out, PUB_KEY);
	printf("plaintext info : %s len = %d\n", instr, ret);

//	char *check_out = (char *)malloc(200);
	char *check_out = NULL;
//	memset(check_out, 0, 200);
	RSA* rsa_pri = create_RSA(private_key, PRIV_KEY);
   	ret = rsa_handle(rsa_pri, out, ret, &check_out, PRIV_KEY);
	printf("pri_decrypt : %s len = %d, addr: %p\n", check_out, ret, check_out);
	
	LC_FREE_MALLOC(out);
	LC_FREE_MALLOC(check_out);
	
	return 0;
}
#endif

