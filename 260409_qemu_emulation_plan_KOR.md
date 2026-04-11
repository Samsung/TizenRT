<proposed_plan>
# RTL8730E / BK7239N QEMU 에뮬레이션 확장 구현 계획

## 1. 목표

- 기존 `qemu-virt` 포트를 `rtl8730e` 실행 모델 검증용 에뮬레이션 보드로 재정렬한다.
- 새 `qemu-mps2-an505` 포트를 `bk7239n` 실행 모델 검증용 에뮬레이션 보드로 추가한다.
- 지원 defconfig는 아래 5개로 고정한다.
  - `qemu-virt/flat`
  - `qemu-virt/loadable_xip_elf`
  - `qemu-mps2-an505/flat`
  - `qemu-mps2-an505/loadable_elf`
  - `qemu-mps2-an505/loadable_xip_elf`
- 각 defconfig는 Docker 기반 `dbuild configure/build/download`와 QEMU 부팅, CLI smoke, testcase 자동 검증까지 포함한다.

## 진행 상태

- `2026-04-09`: Phase 1 구현 완료
- 현재 완료 범위: Kconfig 심볼 추가, `qemu-mps2-an505` Kconfig 골격 추가, `dbuild.sh`의 `qemu-virt flat/loadable_xip_elf` alias 해석 추가
- `2026-04-09`: Phase 2 구현 완료
- 현재 완료 범위: `qemu-virt`의 `rtl8730e` execution profile 기본값 추가, PSCI/PL011/runtime 기본값 정리, timer/board init 경로 정리
- `2026-04-09`: Phase 3 구현 완료
- 현재 완료 범위: `qemu-virt/flat`, `qemu-virt/loadable_xip_elf` defconfig 재구성, alias 및 문서 이름 정리
- `2026-04-09`: Phase 4 구현 완료
- 현재 완료 범위: `qemu-mps2-an505` arch/board 포트 골격, CMSDK UART, SysTick, reset/loader 진입점 추가
- `2026-04-10`: Phase 5 구현 및 빌드 검증 완료
- 현재 완료 범위: `qemu-mps2-an505`용 Make.defs, linker script, download/run 스크립트, 3개 defconfig 추가
- `2026-04-10`: `qemu-mps2-an505/flat`, `qemu-mps2-an505/loadable_elf`, `qemu-mps2-an505/loadable_xip_elf` 모두 Docker 기반 `dbuild configure/build` 통과
- `2026-04-10`: `qemu-mps2-an505/loadable_xip_elf`의 `dbuild download` staging 확인
- 추가 메모: `os/binfmt/libxipelf/xipelf.c`에서 constructor 필드 접근을 `CONFIG_BINFMT_CONSTRUCTORS` guard로 보정해, `loadable_xip_elf`가 불필요한 C++ 런타임 없이 빌드되도록 수정
- 추가 메모: root에 복사된 `run_qemu.sh`가 topdir/TTY를 잘못 해석하던 문제를 보정
- 잔여 범위: `run_qemu.sh` quick boot에서는 아직 HardFault가 발생하므로, 실제 QEMU 런타임 안정화는 Phase 6 공용 런타임 하네스 작업에서 이어서 수행

## 2. 아키텍처 결정

- `rtl8730e`
  - 실행 보드: `qemu-virt`
  - 런타임 옵션: `-machine virt,virtualization=off,gic-version=2 -cpu cortex-a15 -smp 2`
  - 목적: Cortex-A 계열, MMU, SMP, loadable/XIP 실행 모델 재현
  - 주의: QEMU 10.2 계열에는 `cortex-a32` CPU 모델이 없으므로 v1은 `cortex-a15` 기반으로 간다.
- `bk7239n`
  - 실행 보드: `qemu-mps2-an505`
  - 런타임 옵션: `-machine mps2-an505 -cpu cortex-m33`
  - 목적: Cortex-M33 single-core, flat/loadable/XIP 실행 모델 재현
  - 주의: 실칩 TrustZone/TF-M/signing 완전 재현이 아니라 non-secure TizenRT 실행 모델 검증 중심으로 간다.

## 3. donor 매핑

| 에뮬레이션 보드 | 새 defconfig | donor defconfig | 빌드 모델 | 핵심 포인트 |
| --- | --- | --- | --- | --- |
| `qemu-virt` | `flat` | `rtl8730e/flat_dev_ddr` | Flat | SMP 2코어, MMU, `/mnt` smoke |
| `qemu-virt` | `loadable_xip_elf` | `rtl8730e/loadable_ext_ddr` | Protected + Loadable + XIP ELF | common/app 분리, XIP ELF |
| `qemu-mps2-an505` | `flat` | `bk7239n/hello` | Flat | Cortex-M33 단일 코어 |
| `qemu-mps2-an505` | `loadable_elf` | `bk7239n/loadable_all` | Protected + Loadable ELF | common/app 로딩 |
| `qemu-mps2-an505` | `loadable_xip_elf` | `bk7239n/xip_all` | Protected + Loadable + XIP ELF | XIP ELF 경로 |

## 4. 인터페이스

- `./os/dbuild.sh configure qemu-virt flat`
- `./os/dbuild.sh configure qemu-virt loadable_xip_elf`
- `./os/dbuild.sh configure qemu-mps2-an505 flat`
- `./os/dbuild.sh configure qemu-mps2-an505 loadable_elf`
- `./os/dbuild.sh configure qemu-mps2-an505 loadable_xip_elf`

## 5. 이름 정리 방침

- 현재 `qemu-virt/loadable_xip_elf`는 `qemu-virt/loadable_xip_elf`로 교체한다.
- 현재 `qemu-virt/flat`는 `qemu-virt/flat`으로 교체한다.
- 새 보드 이름은 `qemu-bk7239n`, `qemu-rtl8730e` 같은 칩명 조합을 쓰지 않고 `qemu-virt`, `qemu-mps2-an505`로 고정한다.
- CI, 런타임 하네스, 문서, `dbuild` 예시까지 모두 새 이름 기준으로 통일한다.

## 6. 구현 작업 단위

### Phase 1. 공통 Kconfig / board / dbuild wiring

목표:

- 새 보드 이름과 defconfig 이름을 TizenRT 설정 시스템에 연결한다.

수정 파일:

- `os/arch/arm/Kconfig`
- `os/board/Kconfig`
- `os/dbuild.sh`

추가 심볼:

- `CONFIG_ARCH_CHIP_QEMU_MPS2_AN505`
- `CONFIG_ARCH_BOARD_QEMU_MPS2_AN505`
- `CONFIG_QEMU_VIRT_PROFILE_RTL8730E`
- `CONFIG_QEMU_MPS2_AN505_PROFILE_BK7239N`

체크리스트:

- `ARCH_CHIP_QEMU_MPS2_AN505` 추가
- `ARCH_BOARD_QEMU_MPS2_AN505` 추가
- `ARCH_BOARD` 문자열 기본값에 `qemu-mps2-an505` 추가
- `dbuild.sh configure qemu-mps2-an505 <config>` 경로 인식
- `dbuild.sh`의 바이너리/다운로드 처리에서 새 보드 이름 분기 추가
- 기존 `qemu-virt` config 이름 참조를 `flat`, `loadable_xip_elf` 기준으로 교체

완료 기준:

- 두 보드 모두 `configure` 단계가 정상 동작

### Phase 2. `qemu-virt`를 `rtl8730e` 실행 모델로 재정렬

목표:

- 현재 `qemu-virt` 포트를 `rtl8730e` donor 기반 실행 모델 검증 보드로 정리한다.

수정 파일:

- `os/arch/arm/src/qemu-virt/Kconfig`
- `os/arch/arm/src/qemu-virt/Make.defs`
- `os/arch/arm/src/qemu-virt/qemu_boot.c`
- `os/arch/arm/src/qemu-virt/qemu_cpuboot.c`
- `os/arch/arm/src/qemu-virt/qemu_irq.c`
- `os/arch/arm/src/qemu-virt/qemu_timer.c`
- `os/arch/arm/src/qemu-virt/qemu_memorymap.c`
- `os/board/qemu-virt/Kconfig`
- `os/board/qemu-virt/src/qemu_boardinit.c`
- `os/board/qemu-virt/src/qemu_reset.c`

유지 심볼:

- `CONFIG_QEMU_VIRT_VIRTIO`
- `CONFIG_QEMU_VIRT_VIRTIO_BLK`

추가 심볼:

- `CONFIG_QEMU_VIRT_USE_CORTEX_A15`
- `CONFIG_QEMU_VIRT_SMP_DEFAULT`

체크리스트:

- 런타임 기본 CPU를 `cortex-a15`로 고정
- SMP 기본 코어 수를 `2`로 고정
- GICv2, timer, PSCI, SMP boot 경로 유지
- donor `rtl8730e`에서 필요한 빌드 모델 특성만 반영
- Realtek 전용 Wi-Fi, BLE, DDR, flash driver 종속 제거
- `virtio-blk` self-test 경로 유지

완료 기준:

- `qemu-virt` flat/loadable_xip_elf 두 구성 모두 부팅 가능

### Phase 3. `qemu-virt` defconfig 2종 정리

목표:

- `rtl8730e` donor 기준으로 `qemu-virt/flat`, `qemu-virt/loadable_xip_elf`를 새 이름으로 제공한다.

수정/추가 파일:

- `build/configs/qemu-virt/Make.defs`
- `build/configs/qemu-virt/qemu-virt_download.sh`
- `build/configs/qemu-virt/qemu_make_bin.sh`
- `build/configs/qemu-virt/tools/run_qemu.sh`
- `build/configs/qemu-virt/flat/defconfig`
- `build/configs/qemu-virt/loadable_xip_elf/defconfig`
- `build/configs/qemu-virt/scripts/qemu.ld`
- `build/configs/qemu-virt/scripts/qemu_dramboot.ld`
- `build/configs/qemu-virt/scripts/xipelf/userspace_all.ld`

삭제/대체 대상:

- `build/configs/qemu-virt/flat/defconfig`
- `build/configs/qemu-virt/loadable_xip_elf/defconfig`

체크리스트:

- `flat`에 `CONFIG_BUILD_FLAT=y`
- `flat`에 `CONFIG_SMP=y`, `CONFIG_SMP_NCPUS=2`
- `loadable_xip_elf`에 `CONFIG_APP_BINARY_SEPARATION=y`
- `loadable_xip_elf`에 `CONFIG_BUILD_PROTECTED=y`
- `loadable_xip_elf`에 `CONFIG_SUPPORT_COMMON_BINARY=y`
- `loadable_xip_elf`에 `CONFIG_XIP_ELF=y`
- `loadable_xip_elf`에 `CONFIG_SMP=y`, `CONFIG_SMP_NCPUS=2`
- testcase 및 `virtio_blk_test` 활성화
- `run_qemu.sh`의 QEMU 명령을 `-cpu cortex-a15 -M virt -smp 2` 기준으로 정리

완료 기준:

- `qemu-virt/flat`
- `qemu-virt/loadable_xip_elf`
- 두 구성 모두 `build/download/run` 가능

### Phase 4. `qemu-mps2-an505` 새 포트 추가

목표:

- `bk7239n` 실행 모델용 Cortex-M33 보드를 새로 추가한다.

추가 파일:

- `os/arch/arm/src/qemu-mps2-an505/Kconfig`
- `os/arch/arm/src/qemu-mps2-an505/Make.defs`
- `os/arch/arm/src/qemu-mps2-an505/qemu_mps2_boot.c`
- `os/arch/arm/src/qemu-mps2-an505/qemu_mps2_irq.c`
- `os/arch/arm/src/qemu-mps2-an505/qemu_mps2_timer.c`
- `os/arch/arm/src/qemu-mps2-an505/qemu_mps2_serial.c`
- `os/arch/arm/src/qemu-mps2-an505/qemu_mps2_loader.c`
- `os/arch/arm/include/qemu-mps2-an505/chip.h`
- `os/arch/arm/include/qemu-mps2-an505/irq.h`
- `os/board/qemu-mps2-an505/Kconfig`
- `os/board/qemu-mps2-an505/include/board.h`
- `os/board/qemu-mps2-an505/src/qemu_mps2_boardinit.c`
- `os/board/qemu-mps2-an505/src/qemu_mps2_reset.c`

추가 심볼:

- `CONFIG_QEMU_MPS2_AN505_CMSDK_UART`
- `CONFIG_QEMU_MPS2_AN505_TESTFS`
- `CONFIG_QEMU_MPS2_AN505_LOADABLE_LOADER`

체크리스트:

- NVIC 초기화
- SysTick timer 연결
- CMSDK UART 콘솔 연결
- 기본 메모리 맵 정의
- loadable/XIP용 loader 진입점 추가
- `boardctl(BOARDIOC_RESET)` 지원

완료 기준:

- `qemu-mps2-an505` flat이 프롬프트까지 부팅

### Phase 5. `qemu-mps2-an505` defconfig 3종

목표:

- `bk7239n` donor 기준으로 M33 계열 3개 실행 모델을 추가한다.

추가 파일:

- `build/configs/qemu-mps2-an505/Make.defs`
- `build/configs/qemu-mps2-an505/board_metadata.txt`
- `build/configs/qemu-mps2-an505/qemu-mps2-an505_download.sh`
- `build/configs/qemu-mps2-an505/qemu_make_bin.sh`
- `build/configs/qemu-mps2-an505/tools/run_qemu.sh`
- `build/configs/qemu-mps2-an505/flat/defconfig`
- `build/configs/qemu-mps2-an505/loadable_elf/defconfig`
- `build/configs/qemu-mps2-an505/loadable_xip_elf/defconfig`
- `build/configs/qemu-mps2-an505/scripts/qemu_mps2.ld`
- `build/configs/qemu-mps2-an505/scripts/qemu_mps2_loadable.ld`
- `build/configs/qemu-mps2-an505/scripts/xipelf/userspace_all.ld`

체크리스트:

- `flat`에 `CONFIG_BUILD_FLAT=y`
- `flat`에 `CONFIG_ARCH_CORTEXM33=y`
- `loadable_elf`에 `CONFIG_APP_BINARY_SEPARATION=y`
- `loadable_elf`에 `CONFIG_BUILD_PROTECTED=y`
- `loadable_elf`에 `CONFIG_SUPPORT_COMMON_BINARY=y`
- `loadable_xip_elf`에 위 설정 + `CONFIG_XIP_ELF=y`
- testcase 실행에 필요한 공통 옵션 반영
- `bk7239n` 실보드 전용 signing, TF-M, 배포 스크립트 종속 제거

완료 기준:

- `qemu-mps2-an505/flat`
- `qemu-mps2-an505/loadable_elf`
- `qemu-mps2-an505/loadable_xip_elf`
- 세 구성 모두 `build/download/run` 가능

현재 상태:

- `2026-04-10` 기준 `configure/build`는 세 구성 모두 검증 완료
- `2026-04-10` 기준 `loadable_xip_elf`의 `download` staging까지 확인 완료
- `run_qemu.sh` quick boot에서는 아직 HardFault가 발생하며, 이 이슈는 Phase 6 런타임 하네스 일반화와 함께 해결

### Phase 6. 런타임 하네스 일반화

목표:

- 현재 `qemu-virt` 전용 런타임 하네스를 두 보드 공용으로 확장한다.

수정 파일:

- `tools/qemu_test/Dockerfile`
- `tools/qemu_test/run_qemu_tests.py`

추가 파일:

- `tools/qemu_test/platform_qemu_virt.py`
- `tools/qemu_test/platform_qemu_mps2_an505.py`

체크리스트:

- 보드별 QEMU command builder 분리
- 보드별 smoke/testcase profile 분리
- `qemu-virt`는 `virtio_blk_test` 유지
- `qemu-mps2-an505`는 `virtio_blk_test` 제외
- 결과 출력 디렉터리 인자를 `--results-dir`로 정리하고 기존 `--artifact-dir`는 호환 alias 유지
- transcript, JUnit, summary 생성 로직은 공용 유지

완료 기준:

- 동일 하네스로 두 보드 모두 런타임 검증 가능

현재 상태:

- `2026-04-10` 기준 공용 하네스 구조 분리 완료
- 반영 범위:
  - `tools/qemu_test/run_qemu_tests.py`를 보드 공용 하네스로 일반화
  - `tools/qemu_test/platform_base.py`, `tools/qemu_test/platform_qemu_virt.py`, `tools/qemu_test/platform_qemu_mps2_an505.py` 추가
  - `qemu-mps2-an505`용 `run_qemu.sh` / `download` staging 경로를 공용 하네스와 맞게 정리
  - `ARMV8M_SECURE_STATE`, secure stack restore용 `EXC_RETURN` bit 6 반영
  - `qemu-mps2-an505` RAM window를 secure context restore 기준으로 재조정
  - `qemu_mps2_serial.c`에서 CMSDK UART RX/TX 경로를 QEMU 모델 기준으로 재정렬
  - RX interrupt clear를 data read 이전으로, TX interrupt clear를 data write 이전으로 이동
  - IRQ handler를 `intstatus` 의존 대신 `state` 기반 `uart_recvchars` / `uart_xmitchars` 처리로 단순화
- 실제 검증 결과:
  - `qemu-mps2-an505/flat`는 Docker `dbuild configure/build/download` 후 QEMU에서 `TASH>>` prompt까지 도달 확인
  - 수동 `help` 입력 실행 확인
  - 공용 하네스로 `boot`, `help`, `free`, `ps`, `pwd`, `/mnt` file I/O smoke, `drivers_tc`, `filesystem_tc`, `kernel_tc`까지 통과
- 현재 blocker:
  - `qemu-mps2-an505` 출력 스트림에는 여전히 NUL byte와 중복 echo가 섞이므로, transcript 품질과 `loadable_elf` / `loadable_xip_elf` 런타임 검증은 추가 확인 필요
  - 따라서 Phase 6은 "flat CLI smoke/testcase 검증 완료, 나머지 M33 런타임 구성 검증 확장 필요" 상태

### Phase 7. GitHub Actions 5-config matrix

목표:

- 5개 defconfig를 CI에서 자동 검증한다.

수정 파일:

- `.github/workflows/qemu-runtime-tests.yml`

삭제/대체 대상:

- `.github/workflows/qemu-virt-runtime-tests.yml`

체크리스트:

- matrix에 5개 대상 추가
- matrix 필드에 `board`, `config`, `download_args`, `runtime_board`, `test_profile` 추가
- `qemu-virt`와 `qemu-mps2-an505` 각각 다른 `run_qemu.sh` 호출 경로 반영
- 결과 업로드 경로를 `test-results/<board>-<config>`로 통일
- 실패 시에도 transcript/JUnit/summary 업로드

완료 기준:

- 5개 matrix job이 모두 실행되며 결과를 남김

### Phase 8. 문서 및 정리

목표:

- 새 보드/defconfig/검증 방법을 문서화하고 기존 이름을 정리한다.

수정 파일:

- `build/configs/qemu-virt/README.md`
- `build/configs/qemu-virt/README_KOR.md`
- `build/configs/qemu-mps2-an505/README.md`
- `build/configs/qemu-mps2-an505/README_KOR.md`
- `260406_qemu.md`
- `260406_qemu_KOR.md`

체크리스트:

- `qemu-virt` 문서의 old defconfig 이름 제거
- `qemu-mps2-an505` 사용자 가이드 추가
- 5개 defconfig build/run 예시 추가
- CI matrix 설명 업데이트
- 본 계획 문서 링크 추가

완료 기준:

- 문서만 보고 configure/build/run/CI 흐름 확인 가능

## 7. 검증 체크리스트

1. `qemu-virt/flat` configure/build/download/run 통과
2. `qemu-virt/loadable_xip_elf` configure/build/download/run 통과
3. `qemu-mps2-an505/flat` configure/build/download/run 통과
4. `qemu-mps2-an505/loadable_elf` configure/build/download/run 통과
5. `qemu-mps2-an505/loadable_xip_elf` configure/build/download/run 통과
6. `qemu-virt` 두 구성에서 SMP 2코어 부팅 확인
7. `qemu-virt` 두 구성에서 `virtio_blk_test` PASS 확인
8. `qemu-mps2-an505` 세 구성에서 CLI smoke 확인
9. 각 구성에서 `drivers_tc`, `filesystem_tc`, `kernel_tc` 실행 가능 여부 검증
10. CI 5-config matrix 결과 업로드 확인

## 8. 범위 제외

- `rtl8730e` 실칩의 Wi-Fi, BLE, display, vendor flash 드라이버 재현
- `bk7239n`의 TF-M secure world 완전 재현
- 실보드용 signing / deployment 패키징 재현
- `qemu-virt`에서 실제 `cortex-a32` CPU 모델 사용

## 9. 작업 순서 고정

1. Phase 1
2. Phase 2
3. Phase 3
4. Phase 6
5. Phase 4
6. Phase 5
7. Phase 7
8. Phase 8

이 순서를 유지한다. 이유는 `qemu-virt` 쪽이 기존 포트 재활용 비중이 높아 먼저 안정화할 수 있고, 이후 공용 하네스를 일반화한 뒤 `qemu-mps2-an505`를 붙이는 편이 전체 리스크가 낮기 때문이다.
## 10. 진행 업데이트

- `2026-04-09`: Phase 3 구현 완료
- 반영 범위: `qemu-virt/flat`, `qemu-virt/loadable_xip_elf` defconfig 재구성, SMP 2코어 및 `rtl8730e` profile 설정 명시, `dbuild.sh` old/new config name 호환 경로 정리
- `2026-04-09`: Phase 4 구현 완료
- 반영 범위: `qemu-mps2-an505` armv8-m 포트 골격 추가, AN505 기준 NVIC/IRQ map 반영, SysTick timer 연결, CMSDK APB UART0 콘솔 드라이버 추가, board init/reset 및 loader stub 추가
- `2026-04-10`: Phase 6 부분 진행
- 반영 범위: 공용 런타임 하네스 구조 분리, `qemu-mps2-an505` secure restore 경로 보정, CMSDK UART RX/TX 경로 재정렬, `qemu-mps2-an505/flat` smoke/testcase 검증
- 현재 상태: `qemu-mps2-an505/flat` 기준 `boot/help/free/ps/pwd`, `/mnt` file I/O smoke, `drivers_tc/filesystem_tc/kernel_tc` 통과. `loadable_elf`, `loadable_xip_elf` 런타임은 후속 검증 필요
</proposed_plan>

## 11. Phase 6 결과 정리 (2026-04-10)

- `qemu-mps2-an505/loadable_elf`
  - Docker 기반 `dbuild configure/build/download` 완료
  - QEMU boot, `help/free/ps/pwd`, `/mnt` file I/O smoke, `drivers_tc/filesystem_tc/kernel_tc` 통과
  - 최신 안정 통과 기준: `runtime-results/phase6-mps2-loadable-elf-v13`

- `qemu-mps2-an505/loadable_xip_elf`
  - Docker 기반 `dbuild configure/build/download` 완료
  - QEMU boot, `help/free/ps/pwd`, `/mnt` file I/O smoke, `drivers_tc/filesystem_tc/kernel_tc` 통과
  - 최신 안정 통과 기준: `runtime-results/phase6-mps2-loadable-xip-v7`

- 이번 Phase 6에서 실제로 해결한 핵심 이슈
  - `loadable_xip_elf`의 exec alias와 flash staging layout이 어긋나서 `task_startup`가 0으로 읽히던 문제를 수정
  - `build/configs/qemu-mps2-an505/loadable_xip_elf/defconfig`의 XIP partition layout과 `CONFIG_FLASH_VSTART_LOADABLE`을 QEMU 실행 모델에 맞게 재정렬
  - `tools/qemu_test/platform_qemu_mps2_an505.py`와 `build/configs/qemu-mps2-an505/tools/run_qemu.sh`에서 `common.trpk`, `app1.trpk`를 exec alias 주소에도 직접 loader attach 하도록 수정
  - `os/arch/arm/src/common/up_restoretask.c`, `os/arch/arm/src/armv8-m/up_svcall.c`에서 common binary MPU restore 경로를 보강

- transcript 품질 정리
  - `tools/qemu_test/run_qemu_tests.py`의 transcript sanitizer에서 NUL 제거, CR overwrite 정리, 연속 빈 줄 압축을 반영
  - `qemu-mps2-an505` profile은 boot/prompt settle delay를 늘리고 line 단위 입력으로 전환해서 초기 shell race와 입력 echo 왜곡을 줄임

- 현재 해석
  - `qemu-mps2-an505`의 `flat`, `loadable_elf`, `loadable_xip_elf` 세 구성이 모두 공용 하네스로 runtime 검증 가능한 상태
  - 남은 작업은 CI matrix 연결과 `qemu-virt` 2개 구성까지 포함한 통합 문서 정리 중심

## 12. Phase 7 결과 정리 (2026-04-10)

- GitHub Actions 워크플로를 `qemu-virt-runtime-tests.yml`에서 `qemu-runtime-tests.yml`로 일반화
- CI matrix 검증 대상을 다음 5개 구성으로 확장
  - `qemu-virt/flat`
  - `qemu-virt/loadable_xip_elf`
  - `qemu-mps2-an505/flat`
  - `qemu-mps2-an505/loadable_elf`
  - `qemu-mps2-an505/loadable_xip_elf`
- 각 matrix 항목은 다음 순서로 동작
  - `./os/dbuild.sh configure <board> <config>`
  - `./os/dbuild.sh build`
  - board별 `download`
  - `tools/qemu_test/run_qemu_tests.py`로 runtime smoke/testcase 검증
  - transcript, JUnit XML, summary artifact 업로드
- `qemu-mps2-an505` 사용자 가이드를 새로 추가
  - `build/configs/qemu-mps2-an505/README.md`
  - `build/configs/qemu-mps2-an505/README_KOR.md`
- `qemu-virt` 사용자 가이드도 새 workflow 이름과 5-config matrix 기준으로 정리

## 13. Phase 8 결과 정리 (2026-04-11)

- 문서 정리 범위를 다음 파일까지 확장
  - `260406_qemu.md`
  - `260406_qemu_KOR.md`
  - `build/configs/qemu-virt/README.md`
  - `build/configs/qemu-virt/README_KOR.md`
  - `build/configs/qemu-mps2-an505/README.md`
  - `build/configs/qemu-mps2-an505/README_KOR.md`
- `260406` 보고서는 원래 `qemu-virt` 포팅 보고서 성격을 유지하되, 현재 프로젝트가 5-config 에뮬레이션 체계로 확장되었음을 명시
- `qemu-virt`, `qemu-mps2-an505` 가이드에서 계획 문서 `260409_qemu_emulation_plan_KOR.md`로 바로 이동할 수 있도록 링크/참조 추가
- 문서 기준 상태 정리
  - `qemu-virt`: `flat`, `loadable_xip_elf`
  - `qemu-mps2-an505`: `flat`, `loadable_elf`, `loadable_xip_elf`
  - CI: `qemu-runtime-tests.yml` 5-config matrix
- 최종 요약 문서 추가
  - `260411_qemu_emulation_summary_KOR.md`
