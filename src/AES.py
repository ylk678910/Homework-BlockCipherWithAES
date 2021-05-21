import os
import sys
from Crypto.Cipher import AES
import threading
import time
from Crypto.Util.Padding import pad, unpad

kill_th = False
ciphertext = ''


def AESBlockCipher(one_block_text, key):
    cipher = AES.new(key, AES.MODE_ECB)

    # zero padding
    for t in range(16-len(one_block_text)):
        one_block_text = one_block_text+b'\x00'

    ciphert = cipher.encrypt(one_block_text)

    return ciphert.hex()


def Sender_th():
    global kill_th, ciphertext
    timer = 0
    cmd = ''
    pipe_w = os.open(read_path2, os.O_WRONLY)

    while(kill_th == False):
        if(ciphertext != ''):
            cmd = ciphertext
            os.write(pipe_w, cmd.encode('gb2312'))
            os.write(pipe_w, '\n'.encode('gb2312'))
            ciphertext = ''
        time.sleep(0.1)
    os.close(pipe_w)


def Receiver_th():
    global kill_th, ciphertext
    pipe_r = os.open(read_path, os.O_RDONLY)

    while(kill_th == False):
        data = os.read(pipe_r, 1024)
        if len(data) == 0:
            # time.sleep(0.1)
            time.sleep(0)
        else:
            cmd_l = data.split(b'\n')
            for cmd in cmd_l:
                try:
                    cmd_str = cmd.decode('utf-8')
                    if("exit" == cmd_str):
                        kill_th = True
                    elif(len(cmd) != 0):
                        [plaintext, key] = cmd.split(b',')
                        ciphertext = AESBlockCipher(plaintext, key)
                except:
                    if(len(cmd) != 0):
                        [plaintext, key] = cmd.split(b',')
                        ciphertext = AESBlockCipher(plaintext, key)

    os.close(pipe_r)


for i in range(len(sys.argv)):
    if(sys.argv[i] == '--pr'):
        read_path = sys.argv[i+1]
    if(sys.argv[i] == '--pw'):
        read_path2 = sys.argv[i+1]


Sender = threading.Thread(target=Sender_th)
Receiver = threading.Thread(target=Receiver_th)

Sender.start()
Receiver.start()

Sender.join()
Receiver.join()
