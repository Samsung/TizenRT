# QEMU MPS2 AN505 사용자 가이드 (qemu-mps2-an505)

이 문서는 ARM Cortex-M33 기반 QEMU `mps2-an505` 머신에서 TizenRT를 실행하는 방법을 설명합니다. 목표는 BK7239N의 전체 SoC를 그대로 재현하는 것이 아니라, BK7239N과 같은 Cortex-M33 단일 코어 실행 모델을 QEMU에서 검증하는 것입니다.

## 1. 개요

### 주요 특징

- Cortex-M33 단일 코어 실행 모델 검증
- 실제 하드웨어 없이 QEMU로 boot, shell smoke, testcase 검증 가능
- `flat`, `loadable_elf`, `loadable_xip_elf` 3개 구성을 같은 보드 계열에서 지원

### 지원 구성

이 디렉터리에는 다음 `qemu-mps2-an505` 구성이 포함되어 있습니다.

| 구성 | 빌드 모델 | CPU | 바이너리 형식 | 설명 |
|------|-----------|-----|----------------|------|
| `flat` | Flat (`CONFIG_BUILD_FLAT=y`) | Cortex-M33 | Raw/XIP kernel | 커널과 앱이 하나의 이미지로 링크되며 `./dbuild.sh download`로 QEMU runtime 이미지가 생성됨 |
| `loadable_elf` | Protected (`CONFIG_BUILD_PROTECTED=y`) | Cortex-M33 | ELF loadable apps | 커널과 `common` / `app1`이 분리되어 staging 이미지에서 로드됨 |
| `loadable_xip_elf` | Protected (`CONFIG_BUILD_PROTECTED=y`) | Cortex-M33 | XIP-ELF loadable apps | 커널과 `common` / `app1`이 분리되고, `common` / `app1`은 flash/XIP layout에서 실행됨 |

## 2. 빠른 시작

### 2.1 Configure 및 Build

```bash
cd os
./dbuild.sh configure qemu-mps2-an505 loadable_xip_elf
./dbuild.sh build
```

다른 구성은 다음과 같이 사용합니다.

```bash
./dbuild.sh configure qemu-mps2-an505 flat
./dbuild.sh build

./dbuild.sh configure qemu-mps2-an505 loadable_elf
./dbuild.sh build
```

기본 Docker 빌드 이미지는 `tizenrt/tizenrt:2.0.1`입니다.

### 2.2 Runtime 이미지 staging

```bash
./dbuild.sh download
```

이 명령은 프로젝트 루트에 `qemu_mps2_an505_images/`를 생성하고, 다음 실행 단계에서 사용할 `run_qemu.sh`를 프로젝트 루트로 복사합니다. staging 디렉터리에는 `kernel-runtime.elf`, `flash-runtime.bin`, 그리고 구성에 따라 `common.trpk`, `app1.trpk`, `bootparam.bin`이 포함됩니다.

### 2.3 QEMU 실행

프로젝트 루트에서 다음을 실행합니다.

```bash
cd ..
./run_qemu.sh
```

런타임 이미지를 다른 태그로 바꾸려면 다음과 같이 실행할 수 있습니다.

```bash
QEMU_RUNTIME_DOCKER_IMAGE=tizenrt-qemu-test:ci ./run_qemu.sh
```

기본 런타임 이미지는 [tools/qemu_test/Dockerfile](/C:/Users/seokhun/Desktop/TizenRT/tools/qemu_test/Dockerfile)에서 빌드합니다.

## 3. Runtime 검증 범위

공용 runtime 하네스는 `qemu-mps2-an505`에서 다음 항목을 검증합니다.

- `TASH>>` 프롬프트까지 부팅
- `help`, `free`, `ps`, `pwd`
- `/mnt` 파일 I/O smoke
- `drivers_tc`
- `filesystem_tc`
- `kernel_tc`

로컬 Docker 기준으로 `flat`, `loadable_elf`, `loadable_xip_elf` 세 구성 모두 runtime 검증을 완료했습니다.

## 4. CI

GitHub Actions 워크플로 `qemu-runtime-tests.yml`은 `qemu-mps2-an505` 3개 defconfig와 `qemu-virt` 2개 defconfig를 하나의 5-config matrix로 검증합니다. 각 실행은 transcript, JUnit XML, summary 결과를 artifact로 업로드합니다.

전체 RTL8730E / BK7239N 에뮬레이션 확장 계획과 phase별 진행 상태는 [260409_qemu_emulation_plan_KOR.md](/C:/Users/seokhun/Desktop/TizenRT/260409_qemu_emulation_plan_KOR.md)를 기준으로 확인할 수 있습니다.

## 5. 알려진 제한 사항

- 이 포트는 BK7239N의 전체 SoC 주변장치가 아니라 실행 모델 검증이 목적입니다.
- TF-M secure world, 벤더 Wi-Fi/BLE 블록, 실보드 signing/deployment 흐름은 범위에서 제외합니다.
- `virtio_blk_test`는 `qemu-mps2-an505` 검증 항목에 포함되지 않습니다.
