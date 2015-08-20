with open("/tmp/psk.txt", "w") as fout:
    with open("/tmp/psk.bin", "rb") as fin:
        for bit in fin.read():
            fout.write(chr(ord(bit) + ord('0')))
