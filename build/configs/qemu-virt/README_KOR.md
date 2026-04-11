# QEMU 가상 플랫폼 사용자 가이드 (qemu-virt)

이 문서는 ARM 아키텍처용 QEMU `virt` 머신에서 TizenRT를 실행하는 방법을 설명합니다. 실제 보드 없이도 `qemu-virt` 포트를 빌드하고, flash 이미지를 생성하고, QEMU에서 부팅 및 테스트를 수행할 수 있습니다.

## 1. 개요

### 주요 특징

- 실제 하드웨어 없이 소프트웨어만으로 개발 및 검증 가능
- 재부팅과 배포 시간이 짧아 빠른 테스트 사이클 구성 가능
- 모든 지원 구성은 DRAM boot 기반으로 동작
- `loadable_xip_elf`에서는 커널은 DRAM에서 실행되고 `common`, `app1`은 flash에서 XIP 형태로 실행

### 지원 구성

이 디렉터리에는 다음 `qemu-virt` 보드 구성이 포함되어 있습니다.

| 구성 | 빌드 모델 | CPU | 바이너리 형식 | 설명 |
|------|-----------|-----|----------------|------|
| `flat` | Flat (`CONFIG_BUILD_FLAT=y`) | Dual Core (SMP 2) | - | 커널과 앱이 하나의 이미지로 링크되며 `bl1/kernel/bootparam` 조합으로 내려받음 |
| `loadable_xip_elf` | Protected (`CONFIG_BUILD_PROTECTED=y`) | Dual Core (SMP 2) | XIP-ELF (`CONFIG_XIP_ELF=y`) | 커널은 DRAM에서 실행되고 `common`, `app1`은 flash에서 실행되며 `bl1/kernel/common/app1/bootparam` 조합으로 내려받음 |

## 2. 빠른 시작

### 2.1 Configure 및 Build

```bash
cd os
./dbuild.sh configure qemu-virt loadable_xip_elf
./dbuild.sh build
```

기본 Docker 빌드 이미지는 `tizenrt/tizenrt:2.0.1`을 사용합니다.

`flat`를 빌드하려면 다음을 사용합니다.

```bash
./dbuild.sh configure qemu-virt flat
./dbuild.sh build
```

### 2.2 QEMU flash 이미지 생성

```bash
./dbuild.sh download bl1 kernel common app1 bootparam
```

또는 대화형 선택이 필요하면 다음을 사용할 수 있습니다.

```bash
./dbuild.sh download ALL
```

`flat`에서는 다음 layout으로 내려받습니다.

```bash
./dbuild.sh download bl1 kernel bootparam
```

필요한 경우 프로젝트 루트에 `qemu_flash.bin`이 생성됩니다. 내려받기 과정이 끝나면 프로젝트 루트에 `run_qemu.sh`도 함께 복사됩니다.

주의:

- filesystem testcase를 여러 번 반복 실행할 경우 `qemu_flash.bin`을 다시 생성한 뒤 실행해야 합니다.
- userfs는 flash 기반 SMARTFS를 사용하므로, 이전 테스트 흔적이 남아 있으면 재실행 시 testcase가 실패할 수 있습니다.

### 2.3 QEMU 실행

프로젝트 루트에서 다음을 실행합니다.

```bash
./run_qemu.sh
```

기본 런타임 이미지를 다른 태그로 바꿔 실행하려면 환경변수를 사용할 수 있습니다.

```bash
QEMU_VIRT_DOCKER_IMAGE=tizenrt-qemu-virt-runtime:ci ./run_qemu.sh
```

기본 런타임 이미지는 [tools/qemu_test/Dockerfile](/C:/Users/seokhun/Desktop/TizenRT/tools/qemu_test/Dockerfile)로 빌드합니다. 이 이미지는 레포 안에서 공식 QEMU 소스를 직접 빌드하고, `qemu-virt`에 필요한 4 KiB pflash geometry를 적용하므로 외부 비공식 이미지가 필요하지 않습니다.

### 2.4 Virtio smoke 명령 실행

부팅 후 TASH에서 다음 명령으로 virtio block self-test를 실행할 수 있습니다.

```bash
virtio_blk_test
```

성공 시 다음 문자열이 출력됩니다.

```text
VIRTIO_BLK_TEST: PASS
```

## 3. 하드웨어/소프트웨어 구성

### PL011 UART

- Base Address: `0x09000000`
- Interrupt: `IRQ 33`
- Clock: `24 MHz`
- 역할: 콘솔 출력, 부트로더 로그, 셸 입출력

### Virtio-MMIO 장치

- Base Address: `0x0A000000`
- Device spacing: `0x200`
- 최대 장치 수: `32`
- 현재 자동화 범위: `virtio-blk`

### 메모리 맵

| 영역 | 주소 범위 | 크기 | 설명 |
|------|-----------|------|------|
| Flash 0 | `0x00000000-0x04000000` | 64MB | 기본 부트/애플리케이션 저장소 |
| Flash 1 | `0x04000000-0x08000000` | 64MB | 보조 flash 영역 |
| I/O | `0x08000000-0x0e000000` | 96MB | UART, Virtio 등 MMIO 장치 |
| Secure Memory | `0x0e000000-0x0f000000` | 16MB | Secure world 메모리 |
| PCIe | `0x10000000-0x40000000` | 768MB | PCIe 장치 공간 |
| DDR | `0x40000000-0x50000000` | 256MB | 시스템 RAM |

### 소프트웨어 구성 요소

#### S1-boot (BL1)

QEMU 전용 1차 부트로더입니다. 다음 역할을 수행합니다.

- 하드웨어 초기화
- bootparam 및 kernel partition checksum 검증
- 커널을 RAM으로 적재
- 커널 엔트리로 점프

#### Virtio 드라이버 스택

- `virtio-mmio`
- `virtio-blk`
- `virtio-queue`

#### PL011 UART 드라이버

- FIFO 기반 송수신
- 콘솔 지원
- 인터럽트 기반 입출력

## 4. 디버깅

### 사전 준비

- Visual Studio Code
- `Cortex-Debug` 확장
- `gdb-multiarch` 또는 ARM GDB
- Docker

### 설정

프로젝트 루트에서 `.vscode` 디렉터리를 만들고 `launch.json`을 복사합니다.

```bash
mkdir -p .vscode
cp build/configs/qemu-virt/tools/launch.json .vscode/launch.json
```

### 디버깅 시작

1. 프로젝트 루트에서 `./run_qemu.sh` 실행
2. VS Code에서 Run and Debug 열기
3. 원하는 구성을 선택하여 attach

사용 가능한 예시는 다음과 같습니다.

- `QEMU(S1-Boot)`
- `QEMU(Kernel)`
- `QEMU(XIPELF)`
- `QEMU(ELF)`

## 5. CI 동작

GitHub Actions 워크플로 `qemu-runtime-tests.yml`은 다음 5개 구성을 matrix로 자동 수행합니다.

- `qemu-virt/flat`
- `qemu-virt/loadable_xip_elf`
- `qemu-mps2-an505/flat`
- `qemu-mps2-an505/loadable_elf`
- `qemu-mps2-an505/loadable_xip_elf`

이 중 `qemu-virt` 구성에 대해서는 다음을 자동 수행합니다.

- 지원 defconfig 빌드
- flash 이미지 생성
- QEMU 부팅
- 셸 smoke 테스트 실행
- `virtio_blk_test` 실행
- `drivers_tc`, `filesystem_tc`, `kernel_tc` 실행
- transcript, JUnit, summary 아티팩트 저장

로컬 Docker 검증도 `loadable_xip_elf`, `flat` 두 구성 모두 완료했습니다.

RTL8730E/BK7239N 실행 모델 확장 계획과 순차 구현 체크리스트는 루트 문서 `260409_qemu_emulation_plan_KOR.md`를 기준으로 관리합니다.

## 6. 알려진 제한 사항

- 현재 자동화 범위는 `virtio-blk`까지만 포함
- `virtio-net` 및 네트워크 검증은 제외
- `qemu-virt` 자동화 범위는 `flat`, `loadable_xip_elf` 두 구성입니다.
