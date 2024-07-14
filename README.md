# MiniOICQ

This is the course project for "Integrate Practice for Courses Ⅱ", Zhejiang University, 2024 Summer.

MiniOICQ is a simple instant messaging software.

## Build

### Client

You need to have Qt5 installed.

Using brew:

```bash
brew install qt@5
```

Then in the project root directory:

```bash
qmake
make -j
```

### Server

You need to have python3 installed. Then install the required packages:

```bash
pip install -r requirements.txt
```

## Run

### Client

```bash
src/client.exe.app/Contents/MacOS/client.exe
```

### Server

```bash
python3 minioicq.py
```