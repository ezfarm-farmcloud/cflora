# cflora : Greenhouse environment management system for Smart Farm in Korea

## Introduction

cflora 는 (주)이지팜에서 농촌진흥청과 함께 개발한 온실환경관리 플랫폼입니다. TTAK.06.0288 Part 1, Part 2, Part 3를 지원하며, 오픈소스 하드웨어/소프트웨어로 구성됩니다. 개발 시작단계에서 [네이버개발자센터](http://dev.naver.com/projects/cflora/)에서 관리되었으나, 네이버 개발자 센터의 오픈프로젝트가 2016년 12월 26일부로 종료됨에 따라 github으로 마이그레이션되었습니다.

## Feature highlights
### Hardware
* 제어를 위해 [Cubieboard 2](http://cubieboard.org/model/cb2/) 를 사용합니다.
* Base 보드, AI 보드, DI 보드, DO 보드 로 구성됩니다.
* 각 보드는 CAN으로 연결되고, AI, DI, DO 보드는 확장이 가능합니다.
* AI 보드는 8개의 아날로그 입력을 받을 수 있고, 전압, 전류, 저항을 측정할 수 있습니다.
* DI 보드는 8개의 디지털 입력을 받을 수 있습니다.
* DO 보드는 8개의 디지털 출력을 보낼 수 있습니다.

### Software
* TTAK.06.0286의 온실관제시스템 요구사항 프로파일을 만족합니다.
 * 온실운영시스템(gos), 온실통합제어기(gcg), 센서노드/제어노드(node)로 구성됩니다.
* 온실관제시스템 모듈간 통신인터페이스인 TTAK.06.0288 Part 1, Part 2, Part 3를 지원합니다.
* 센서값 변환 기능을 제공합니다.
 * 선형변환기능을 제공합니다.
 * 계단형 변환기능을 제공합니다.
 * 고차형 변환을 위해 구간별 선형변환기능을 제공합니다.
* 가상센서를 지원합니다.
* 자동제어룰을 지원합니다.

## Setup
### Hardware
* Cubieboard 2는 온라인 쇼핑몰에서 구매가 가능합니다.
* Base 보드, AI 보드, DI 보드, DO 보드는 공개된 PCB를 이용하여 직접 제작하실 수 있으며,  (주)이지팜을 통해서 구매도 가능합니다.

### Image
cflora 는 cubieboard 2를 사용하고 있습니다. 이를 위한 이미지 파일은 [링크](http://dl.cubieboard.org/parteners/waveshare/Image/a20-cubieboard-dvk/lubuntu/cb2-dvk-sdcard-lubuntu-lcd%20v1.0.img)에서 구할 수 있습니다. 해당 파일을 받아서 sd 카드에 구워서 사용하시면 됩니다.

### Dependency
cflora 는 다음의 오픈소스를 사용하고 있습니다.
* [iniparser](https://github.com/ndevilla/iniparser)
* [libtp3](https://github.com/ezfarm-farmcloud/libtp3)

iniparser 와 libtp3는 git submodule 로 연동되어 있습니다. 따라서 다음과 같이 입력하면 컴파일을 위한 소스들을 받아올 수 있습니다.
```
git submodule init
git submodule update
```

단 libtp3는 [libuv](https://github.com/libuv/libuv) 를 사용하기 때문에 libuv 를 먼저 설치해주어야 한다. 더 상세한 설명은 libuv 홈페이지를 참조한다.
```
wget https://github.com/libuv/libuv/archive/v1.x.zip
unzip v1.x.zip -d v1.x
cd v1.x/libuv-1.x
./autogen.sh
./configure
make
make install
```

* [mysql](https://www.mysql.com)
mysql 은 사용하는 OS에 따라 적절히 설치하면 된다. debian 계열의 Ubuntu나 Rasibian 등을 사용하는 경우에는 다음과 같이 설치할 수 있다.
```
sudo apt install mysql-server
```

### Build Instructions
cflora 는 cmake 를 이용해서 컴파일이 가능합니다. 개별 폴더로 이동하셔서 다음의 명령을 입력하시면 컴파일이 됩니다.
```
cmake .
make
```

컴파일 진행시 다음의 순서로 컴파일을 진행하시면 됩니다.
* libcflora
* libtp12
* node
* gcg
* gos

일련의 과정을 한번에 수행하기 위해서 install 폴더의 build.sh를 실행하실 수 있습니다.
```
cd install
./build.sh
```

마지막으로 설치를 위해 install 폴더의 install.sh 를 실행하시면 됩니다.


