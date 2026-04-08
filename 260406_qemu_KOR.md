# 260406 QEMU-VIRT 포팅 및 로컬 검증 보고서

## 1. 작업 범위

이번 작업에서는 TizenRT에 `qemu-virt` 보드를 포팅하고, Docker 기반 로컬 2사이클 검증을 수행했습니다.

수행 범위:

- `dbuild.sh configure qemu-virt full_dramboot_xipelf`
- `dbuild.sh configure qemu-virt flat_dramboot`
- `dbuild.sh build`
- `dbuild.sh download bl1 kernel common app1 bootparam`
- `dbuild.sh download bl1 kernel bootparam`
- 런타임 Docker 이미지 빌드
- Docker 내부에서 QEMU 부팅
- CLI smoke 테스트 실행
- `virtio_blk_test` 실행
- `drivers_tc`, `filesystem_tc`, `kernel_tc` 실행
- 검증 결과 수집

이번 로컬 검증에서는 `full_dramboot_xipelf`, `flat_dramboot` 두 구성을 모두 실제 실행했습니다.

## 2. qemu-virt 포팅 요약

### 2.1 아키텍처 및 보드 활성화

다음 플랫폼 연결 작업이 추가되거나 수정되었습니다.

- [os/arch/arm/Kconfig](/C:/Users/seokhun/Desktop/TizenRT/os/arch/arm/Kconfig)의 chip 선택
- [os/board/Kconfig](/C:/Users/seokhun/Desktop/TizenRT/os/board/Kconfig)의 board 선택
- [os/arch/arm/src/armv7-a/Toolchain.defs](/C:/Users/seokhun/Desktop/TizenRT/os/arch/arm/src/armv7-a/Toolchain.defs)의 Cortex-A15 툴체인 설정
- [os/arch/arm/src/qemu-virt](/C:/Users/seokhun/Desktop/TizenRT/os/arch/arm/src/qemu-virt) 칩 트리 통합
- [os/board/qemu-virt](/C:/Users/seokhun/Desktop/TizenRT/os/board/qemu-virt) 보드 트리 통합
- [os/dbuild.sh](/C:/Users/seokhun/Desktop/TizenRT/os/dbuild.sh)의 `configure qemu-virt <config>`, `download ...` 경로 지원

포팅한 주요 기능:

- DRAM boot 경로
- GICv2 인터럽트 컨트롤러
- ARM generic/physical timer
- PSCI 전원 인터페이스
- PL011 UART 콘솔
- QEMU pflash용 CFI NOR flash 접근
- Virtio MMIO + virtio-blk
- qemu-virt 전용 S1 bootloader 및 linker script

### 2.2 빌드 구성

[build/configs/qemu-virt](/C:/Users/seokhun/Desktop/TizenRT/build/configs/qemu-virt)에 다음 구성을 추가했습니다.

- `full_dramboot_xipelf`
- `flat_dramboot`

자동화 검증용 대표 defconfig에는 다음을 반영했습니다.

- `CONFIG_DOCKER_VERSION="2.0.1"`
- `CONFIG_QEMU_VIRT_VIRTIO=y`
- `CONFIG_QEMU_VIRT_VIRTIO_BLK=y`
- testcase 실행 경로 활성화
- `virtio_blk_test`에 필요한 board ioctl 경로 활성화
- 런타임 testcase 세트에 필요한 OS API/driver 지원 활성화
- 본 타깃의 protected/XIP 링크를 깨던 BCH testcase 항목 비활성화

### 2.3 `virtio_blk_test` 명령 경로

`virtio_blk_test`는 `qemu-virt` 전용 built-in testcase 명령으로 동작합니다.

동작 방식:

- 부팅 중 virtio-blk 초기화
- TASH 명령이 `boardctl()` 호출
- qemu-virt 보드 코드가 ioctl을 받아 virtio block self-test 실행
- 성공 시 `VIRTIO_BLK_TEST: PASS`
- 실패 시 `VIRTIO_BLK_TEST: FAIL ...`

이 구조를 사용해 board 전용 테스트를 커널 초기화 코드에 직접 하드코딩하지 않고 분리했습니다.

### 2.4 Windows + Docker 빌드 경로 보정

이번 검증 호스트는 Windows + Docker Desktop 환경이었기 때문에 다음 보정이 필요했습니다.

- `dbuild`가 defconfig에서 Docker 버전을 읽어 `tizenrt/tizenrt:2.0.1`을 사용하도록 수정
- [os/dbuild.sh](/C:/Users/seokhun/Desktop/TizenRT/os/dbuild.sh)에서 Windows shell 기준 Docker volume path 처리 보정
- Linux 전용 mount 가정 제거
- Git Bash / Windows 경로 변환 문제 처리
- [os/Makefile.unix](/C:/Users/seokhun/Desktop/TizenRT/os/Makefile.unix)에서 `version.sh`를 `bash`로 실행하도록 수정

### 2.5 검증 중 발견된 보드/콘솔 통합 이슈 수정

실행 가능한 상태까지 올리면서 다음 문제를 수정했습니다.

- [qemu_uart.c](/C:/Users/seokhun/Desktop/TizenRT/os/arch/arm/src/qemu-virt/qemu_uart.c)에 `up_lowputc()`, `up_get_console_dev()` 추가
- [boardctl.c](/C:/Users/seokhun/Desktop/TizenRT/os/arch/boardctl.c)에 board ioctl forwarding 추가
- qemu-virt 전용 `BIOC_QEMU_VIRTIO_BLK_TEST` ioctl 추가
- [qemu_boardinit.c](/C:/Users/seokhun/Desktop/TizenRT/os/board/qemu-virt/src/qemu_boardinit.c)의 ioctl 처리와 `errno.h` 누락 수정
- [Make.defs](/C:/Users/seokhun/Desktop/TizenRT/build/configs/qemu-virt/Make.defs)에서 virtio 소스와 공통 ARM 오브젝트 링크 조정

## 3. Docker 런타임 이미지

런타임 자동화 관련 파일:

- [tools/qemu_test/Dockerfile](/C:/Users/seokhun/Desktop/TizenRT/tools/qemu_test/Dockerfile)
- [tools/qemu_test/run_qemu_tests.py](/C:/Users/seokhun/Desktop/TizenRT/tools/qemu_test/run_qemu_tests.py)
- [build/configs/qemu-virt/tools/run_qemu.sh](/C:/Users/seokhun/Desktop/TizenRT/build/configs/qemu-virt/tools/run_qemu.sh)

### 3.1 중요한 런타임 이슈

Ubuntu 패키지 기본 `qemu-system-arm`을 그대로 사용하면 게스트에서 다음과 같이 보였습니다.

- pflash erase size = `262144`
- TizenRT partition layout은 `bl1=32 KiB`, `bootparam=8 KiB` 같은 작은 partition 포함

이 때문에 TASH 프롬프트 도달 전 partition 초기화가 실패했습니다.

### 3.2 외부 이미지 의존성 제거 방식

외부 비공식 런타임 이미지 의존성을 제거하기 위해, 런타임 Dockerfile은 이제 다음 방식으로 repo-owned 이미지를 빌드합니다.

- `ubuntu:24.04` 기반
- `download.qemu.org`의 공식 `qemu-10.2.0-rc3` 소스 다운로드
- `hw/arm/virt.c`의 `VIRT_FLASH_SECTOR_SIZE`를 `256 * KiB`에서 `4 * KiB`로 치환
- 치환 결과를 build 로그에서 다시 확인한 뒤 QEMU 컴파일

이 방식으로 `qemu-virt` partition map과 SMARTFS/userfs가 요구하는 4 KiB flash geometry를 재현했습니다.

### 3.3 실제 검증 결과

재빌드된 런타임 이미지에서 `info qtree`를 확인한 결과:

- `cfi.pflash01 num-blocks = 16384`
- `cfi.pflash01 sector-length = 4096`

기존 참조 이미지와 동일한 속성으로 확인됐습니다.

로컬에서 사용한 런타임 이미지는 다음 태그입니다.

- `tizenrt-qemu-virt-runtime:local`

## 4. 로컬 검증 사이클

### 4.1 실행 명령

빌드/configure/download:

```bash
./os/dbuild.sh configure qemu-virt full_dramboot_xipelf
./os/dbuild.sh build
./os/dbuild.sh download bl1 kernel common app1 bootparam
```

런타임 이미지 빌드:

```bash
docker build -t tizenrt-qemu-virt-runtime:local -f tools/qemu_test/Dockerfile .
```

flash 초기화 및 재생성:

```bash
docker run --rm \
  --mount "type=bind,source=$PWD,target=/workspace" \
  -w /workspace \
  tizenrt/tizenrt:2.0.1 \
  bash -lc "TOPDIR=/workspace bash build/configs/qemu-virt/qemu-virt_download.sh ERASE && \
            TOPDIR=/workspace bash build/configs/qemu-virt/qemu-virt_download.sh bl1 kernel common app1 bootparam"
```

런타임 하네스 실행:

```bash
docker run --rm --network none \
  -v "$PWD:/workspace" \
  -w /workspace \
  tizenrt-qemu-virt-runtime:local \
  python3 tools/qemu_test/run_qemu_tests.py \
    --artifact-dir /workspace/runtime-results \
    --boot-timeout 120 \
    --command-timeout 15 \
    --tc-timeout 180
```

### 4.2 최종 결과

repo-owned 런타임 이미지 기준 최종 결과: **PASS**

수행된 단계:

- boot
- `help`
- `free`
- `ps`
- `pwd`
- `ls /mnt`
- `mkdir /mnt/qemu-smoke`
- `echo > /mnt/qemu-smoke.txt`
- `cat /mnt/qemu-smoke.txt`
- `rm /mnt/qemu-smoke.txt`
- `rmdir /mnt/qemu-smoke`
- `cd /mnt`
- `pwd`
- `cd /`
- `virtio_blk_test`
- `drivers_tc`
- `filesystem_tc`
- `kernel_tc`

요약:

- boot: PASS
- virtio-blk: PASS
- drivers testcase: PASS
- filesystem testcase: PASS
- kernel testcase: PASS
- JUnit: `19 tests, 0 failures`

### 4.3 최종 실행에서 확인한 핵심 증거

- `TASH>>` 프롬프트 도달
- `/mnt` mount 성공 및 쓰기 가능
- 파일 생성/읽기/삭제 smoke 통과
- `VIRTIO_BLK_TEST: PASS`
- `Drivers TC End ... FAIL : 0`
- `FileSystem TC End ... FAIL : 0`
- `Kernel TC End ... FAIL : 0`

### 4.4 clean flash 관련 메모

filesystem testcase는 동일한 `qemu_flash.bin`을 반복 재사용할 경우 항상 재현 가능하지 않습니다. userfs가 flash-backed SMARTFS이기 때문에 이전 실행 흔적이 남을 수 있습니다.

중간 재실행에서는 dirty flash 상태로 인해 다음 실패가 발생했습니다.

- `tc_fs_vfs_sendfile_p FAIL`

이후 `qemu-virt_download.sh ERASE`로 flash를 다시 초기화하고, `bl1/kernel/common/app1/bootparam`을 재배치한 뒤 전체 런타임 suite를 다시 실행해 PASS를 확인했습니다.

### 4.5 `flat_dramboot` 검증

`flat_dramboot`에 대해서도 별도의 로컬 1사이클을 수행했습니다.

build/configure/download:

```bash
./os/dbuild.sh configure qemu-virt flat_dramboot
./os/dbuild.sh build
./os/dbuild.sh download bl1 kernel bootparam
```

clean flash 재생성:

```bash
TOPDIR=$PWD bash build/configs/qemu-virt/qemu-virt_download.sh ERASE
TOPDIR=$PWD bash build/configs/qemu-virt/qemu-virt_download.sh bl1 kernel bootparam
```

`flat_dramboot` 최종 결과: **PASS**

요약:

- boot: `0.99s`
- virtio-blk: `PASS`
- drivers testcase: `PASS`
- filesystem testcase: `PASS`
- kernel testcase: `PASS`
- JUnit: `19 tests, 0 failures`

## 5. 현재 상태

현재 기준 상태:

- `qemu-virt` 포트는 `full_dramboot_xipelf`, `flat_dramboot` 두 구성 기준 빌드 가능
- Docker 기반 QEMU 런타임 사이클을 두 구성 모두에서 실행 가능
- CLI smoke + virtio block + testcase 흐름이 두 구성 모두에서 end-to-end 통과
- 로컬 하네스와 CI 워크플로 기준 런타임 결과 수집 경로 정리 완료
- 런타임 Dockerfile은 외부 비공식 이미지 없이 레포 내부에서 재현 가능

## 6. 후속 권장 사항

- GitHub Actions에서 두 defconfig 모두 실제 실행 검증
- 로컬 재실행 시 filesystem testcase 전에 `qemu_flash.bin`을 자동 재생성할지 결정
