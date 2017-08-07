# aac-example
---
1. [fdkaac encode sample](./fdkaac/enc_test.cpp)
2. [fdkaac decode sample](./fdkaac/dec_test.cpp)

# Usage
---
## Step 1: get fdk-aac
```
git clone https://github.com/mstorsjo/fdk-aac.git
```
## Step 2: build fdk-aac
```
./autogen.sh && ./configure && make && make install
```
## Step 3: build aac-example
```
copy fdk-aac's (include and lib) folder to ./fdkaac folder then
cd ./fdkaac && make
```