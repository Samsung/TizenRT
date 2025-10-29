import os
import platform
import random

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad


def generate_random_8bytes():
    '''
    生成8字节的随机数
    :returns:
        返回一个包含8字节随机数据的bytes对象
    '''
    # 生成8个随机字节
    random_bytes = bytes([random.randint(0, 255) for _ in range(8)])
    return random_bytes

# AES128-ECB 加密解密函数
def aes128_ecb_encrypt(plaintext, key):
    """AES128-ECB 加密
    
    Args:
        plaintext (bytes): 明文数据
        key (bytes): 16字节密钥
    
    Returns:
        bytes: 加密后的密文，长度与明文相同
    """
    # 创建 AES128-ECB 加密器
    cipher = AES.new(key, AES.MODE_ECB)
    # padded_plaintext = pad(plaintext, AES.block_size)
    # ciphertext = cipher.encrypt(padded_plaintext)
    #检查明文长度是否为AES块大小的整数倍
    if len(plaintext) % AES.block_size != 0:
        # 长度不是块大小整数倍时进行填充
        padded_plaintext = pad(plaintext, AES.block_size)
        ciphertext = cipher.encrypt(padded_plaintext)
    else:
        # 长度已经是块大小整数倍时直接加密
        ciphertext = cipher.encrypt(plaintext)
        
    return ciphertext

def aes128_ecb_decrypt(ciphertext, key):
    """AES128-ECB 解密
    
    Args:
        ciphertext (bytes): 密文数据
        key (bytes): 16字节密钥
    
    Returns:
        bytes: 解密后的明文
    """
    # 创建 AES128-ECB 加密器
    cipher = AES.new(key, AES.MODE_ECB)
    # 解密
    decrypted_data = cipher.decrypt(ciphertext)
    
    try:
        # 尝试去除填充（如果有填充）
        plaintext = unpad(decrypted_data, AES.block_size)
    except ValueError:
        # 如果无法去除填充，说明数据可能没有填充，直接返回解密结果
        plaintext = decrypted_data
        
    return plaintext


      
   
    

def encrypt_random(s:str, key:int=914)->str:
    """
    Implements the encryption algorithm as described in the C++ code.

    Args:
        s: The string to encrypt.
        key: The integer key.

    Returns:
        The encrypted string.
    """
    p_data_temp = ""
    p_data_final = ""
    p_data = list(s)  # Convert string to list of characters for modification

    # First loop: XOR and Key update
    for i in range(len(s)):
        i_data = ord(p_data[i])
        p_data[i] = chr(i_data ^ (key >> 8))
        s = "".join(p_data) # Update s after each XOR operation
        key = (ord(p_data[i]) + key)

    p_data = list(s) #Reset p_data to the updated string s

    # Second loop: Character conversion
    for i in range(len(s)):
        j = ord(p_data[i])
        p_data_temp = "12"  # This seems to be a placeholder, not used for actual encryption

        # Calculate the two characters based on j
        char1 = chr(97 + j // 26)
        char2 = chr(97 + j % 26)

        p_data_temp = char1 + char2
        p_data_final += p_data_temp

    return p_data_final


def decrypt_random(s:str, key:int=914)->str:
  """
  Decrypts a string using the provided algorithm.

  Args:
    s: The string to decrypt.
    key: The integer key to use for decryption.

  Returns:
    The decrypted string.
  """

  p_data = list(s)  # Convert string to list for mutable character access
  p_data_temp = [""]
  p_data_final = []

  for i in range(len(s) // 2):
    j = ((ord(s[2 * i]) - ord('a')) * 26) + (ord(s[2 * i + 1]) - ord('a'))
    p_data_temp[0] = chr(j)
    p_data_final.append(p_data_temp[0])

  p_data = "".join(p_data_final)  # Convert list back to string
  
  result = list(p_data)  # Convert string to list for mutable character access

  for i in range(len(p_data)):
    result[i] = chr(ord(p_data[i]) ^ (key >> 8))
    key = (ord(p_data_final[i]) + key)

  return "".join(result)  # Convert list back to string





# 测试加密解密过程
if __name__ == "__main__":
    # # 128 位密钥（16 个字节）
    key = bytes.fromhex("254b939c97b307ba9f48d038067e99ac")
    # 明文
    plaintext = bytes.fromhex("a90a36b5289fea49")+bytes.fromhex("0000000000000000")
    print(plaintext)
    print(len(plaintext))
    # 加密
    ciphertext = aes128_ecb_encrypt(plaintext, key)
    print(ciphertext)
    print(len(ciphertext))
    print(f"Ciphertext: {ciphertext.hex()}")
   
    # with open("E:\\BK_File\\BK7236\\all-app-en.bin",'rb') as f:
    #     ciphertext = f.read()
    #     f.close()
    # 解密
    decrypted_plaintext = aes128_ecb_decrypt(ciphertext, key)
    print(decrypted_plaintext)
    print(len(decrypted_plaintext))
    print(f"Decrypted Plaintext: {decrypted_plaintext.hex()}")
    

    # s = "cbdsbxcdcjbxdtbycdcibxduckbycabxdwckbwegbxefcfeccleackcfcebyegeaebcbchcacfedcbeccjcecfegeebgblejbgbpeleibmbgepbjbheobnbnbmekeibt"
    # key = 914
    # p_data = decrypt_random(s,key)
    # print(p_data)
    # s = "6c5395e7297d8436c98f9e0c6b4339ece9794f8d366ac05a09ba42e13e445ad9"
    # key = 914
    # p_data = encrypt_random(s,key)
    # print(p_data)
    
    # s = decrypt_random(p_data,key)
    # print(s)

    # out_str = ""      
    # subp = subprocess.Popen(["E:\\BK_File\\BK7258\\image_en\\bk_image_en.exe", 'decrypt','-infile', "E:\\BK_File\\BK7258\\image_en\\config.json.en",'-outstream','-keywords',"10000000fbca4d11822942fb"], stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
    # while True:
    #     outp = subp.stdout.readline()           
    #     if outp=='' and subp.poll() is not None:
    #         break
    #     if outp:
    #         out_str += outp.strip()
    # if out_str.find("outstream:")!=-1:
    #     print(out_str)
    
    
     