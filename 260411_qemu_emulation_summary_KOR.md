# QEMU 에뮬레이션 작업 요약

## 1. 무엇을 했는가

이번 작업의 목적은 실칩을 직접 에뮬레이션하는 것이 아니라, 주력 칩의 실행 모델을 QEMU에서 재현해 TizenRT를 자동 검증할 수 있게 만드는 것이었습니다.

- `rtl8730e` 실행 모델용 `qemu-virt` 포트 정리
- `bk7239n` 실행 모델용 `qemu-mps2-an505` 포트 추가
- 5개 defconfig 구성
  - `qemu-virt/flat`
  - `qemu-virt/loadable_xip_elf`
  - `qemu-mps2-an505/flat`
  - `qemu-mps2-an505/loadable_elf`
  - `qemu-mps2-an505/loadable_xip_elf`
- Docker 기반 `dbuild configure/build/download`
- QEMU 부팅 후 CLI smoke / testcase 자동 검증 하네스 구축
- GitHub Actions 5-config matrix workflow 정리

## 2. 보드별로 어떻게 구현했는가

### 2.1 `qemu-virt`

`qemu-virt`는 `rtl8730e`의 실행 모델을 검증하는 방향으로 정리했습니다.

- 런타임 CPU 모델은 `cortex-a15`
- 머신은 `virt`
- SMP 2코어 기준
- PSCI, GICv2, PL011 UART, architected timer 유지
- `virtio-blk` 및 `/mnt` filesystem smoke 유지
- `virtio_blk_test` built-in command 유지

### 2.2 `qemu-mps2-an505`

`qemu-mps2-an505`는 `bk7239n`의 실행 모델을 검증하는 방향으로 추가했습니다.

- 런타임 CPU 모델은 `cortex-m33`
- 머신은 `mps2-an505`
- single core 기준
- NVIC, SysTick, CMSDK UART 경로 구현
- loadable / XIP ELF용 staging, loader, exec alias 처리 추가

## 3. 구현 중 어떤 문제를 고쳤는가

### 3.1 `qemu-virt`

- `dbuild`가 새 config 이름 `flat`, `loadable_xip_elf`를 인식하도록 정리
- `qemu-virt` runtime helper가 `-cpu cortex-a15 -smp 2`를 읽도록 정리
- `up_cpu_up()` 심볼이 없어 SMP 링크가 깨지던 문제 수정
- QEMU runtime Docker 이미지가 4 KiB pflash geometry를 재현하도록 유지

### 3.2 `qemu-mps2-an505`

- CMSDK UART RX/TX 경로 재정렬
- secure restore / EXC_RETURN 경로 보정
- common binary MPU restore 경로 보강
- `loadable_xip_elf`에서 exec alias 주소에 `common.trpk`, `app1.trpk`를 직접 loader attach 하도록 수정
- transcript sanitizer에서 NUL 제거, CR overwrite 처리, blank line 압축 반영

### 3.3 공용 하네스 / CI

- `tools/qemu_test/run_qemu_tests.py`를 보드 공용 하네스로 일반화
- `platform_qemu_virt.py`, `platform_qemu_mps2_an505.py` 분리
- GitHub Actions를 `qemu-runtime-tests.yml` 5-config matrix로 일반화

## 4. 현재 상태

아래 5개 구성은 모두 로컬 Docker + QEMU 기준으로 `configure/build/download/runtime`까지 실제 통과 확인했습니다.

| 보드 | defconfig | build | download | runtime smoke | testcase |
|------|-----------|-------|----------|---------------|----------|
| `qemu-virt` | `flat` | PASS | PASS | PASS | PASS |
| `qemu-virt` | `loadable_xip_elf` | PASS | PASS | PASS | PASS |
| `qemu-mps2-an505` | `flat` | PASS | PASS | PASS | PASS |
| `qemu-mps2-an505` | `loadable_elf` | PASS | PASS | PASS | PASS |
| `qemu-mps2-an505` | `loadable_xip_elf` | PASS | PASS | PASS | PASS |

runtime smoke 범위:

- `boot`
- `help`
- `free`
- `ps`
- `pwd`
- `/mnt` file I/O smoke
- `qemu-virt`의 경우 `virtio_blk_test`
- `drivers_tc`
- `filesystem_tc`
- `kernel_tc`

## 5. 검증 시 주의한 점

- 로컬에서 서로 다른 보드를 같은 workspace에서 연속 빌드할 때 `os/userspace/up_userspace*.o` 같은 stale object가 남으면 아키텍처 충돌이 발생할 수 있음을 확인했습니다.
- 이 문제는 fresh checkout으로 동작하는 GitHub Actions matrix에는 직접 영향이 없고, 로컬 순차 검증에서는 stale object 정리 후 재검증해 통과를 확인했습니다.
- filesystem testcase는 flash-backed userfs 상태에 영향을 받기 때문에, 재실행 시 board별 runtime image를 새로 생성하는 것이 안전합니다.

## 6. 관련 문서

- 구현 계획 및 phase 로그: [260409_qemu_emulation_plan_KOR.md](/C:/Users/seokhun/Desktop/TizenRT/260409_qemu_emulation_plan_KOR.md)
- `qemu-virt` 가이드: [README_KOR.md](/C:/Users/seokhun/Desktop/TizenRT/build/configs/qemu-virt/README_KOR.md)
- `qemu-mps2-an505` 가이드: [README_KOR.md](/C:/Users/seokhun/Desktop/TizenRT/build/configs/qemu-mps2-an505/README_KOR.md)
- `qemu-virt` 포팅 보고서: [260406_qemu_KOR.md](/C:/Users/seokhun/Desktop/TizenRT/260406_qemu_KOR.md)

## 7. 결론

계획했던 범위 기준으로는 구현과 로컬 실행 검증까지 완료된 상태입니다.

- QEMU 모델 2종 포팅 완료
- `qemu-virt` 2개 defconfig 완료
- `qemu-mps2-an505` 3개 defconfig 완료
- 공용 runtime 하네스 완료
- 5-config CI workflow 정리 완료
