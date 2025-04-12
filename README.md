# Usage

Just compile the C code and pass the Base64 encoded file as a parameter when executing the file.

Directory struture before:
```
C:\Path
|-- base64.txt
|-- base64Decoder.c
```

Compiling and running:
```
C:\Path> gcc -Wall -o Base64Decoder base64Decoder.c
C:\Path> Base64Decoder.exe "base64.txt"
C:\Path> Type the output file extension: .bin
C:\Path>
```

Directory struture after:
```
C:\Path
|-- base64.bin
|-- base64.txt
|-- base64Decoder.c
|-- base64Decoder.exe
```
