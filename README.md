# 🏎️ ARM Cortex-M4 기반 초음파 센서 자율주행 RC카 설계
> **3방향 초음파 센서 데이터와 Hysteresis 제어를 활용한 지능형 회피 주행 시스템**

<img width="865" height="649" alt="image" src="https://github.com/user-attachments/assets/8ca921ea-3f5a-42fb-b46c-8a1e5e135e57" />
<img width="2048" height="529" alt="image" src="https://github.com/user-attachments/assets/dada7a2f-79a8-4141-8663-249d21bde6b0" />
<img width="2048" height="505" alt="image" src="https://github.com/user-attachments/assets/0c0390d9-4ea2-4ed5-b056-5b1f99f269d1" />



## 📌 Project Overview
STM32 마이크로컨트롤러를 중심으로 3방향 초음파 신호를 실시간 처리하여 장애물을 회피하는 자율주행 플랫폼입니다. 단순 거리 측정을 넘어 **이동평균 필터**를 통한 데이터 정제와 **주행 이력(`Last_ACT`) 기반의 의사결정**을 통해 시스템의 신뢰성을 확보하는 데 집중했습니다.

## 🛠 Tech Stack
- **Hardware:** STM32 (ARM Cortex-M4), 3-Way Ultrasonic Sensors (HC-SR04), DC Motors
- **Software:** C (STM32 HAL Library)
- **Peripherals:** Timer Input Capture (Distance), PWM (Motor Control), UART DMA (Bluetooth), GPIO

## 🏗 System Architecture
실시간 데이터 처리와 CPU 효율화를 위해 **인터럽트 및 비동기 처리 구조**를 채택했습니다.

- **Sensing Layer:** Timer Input Capture 인터럽트로 에코 펄스를 정밀 측정하며, 3개 센서 동기화 플래그(`US_F`)를 통해 데이터 일관성 유지.
- **Control Layer:** 수집된 거리 데이터를 바탕으로 9가지 주행 케이스를 정의한 `Auto_Mode` 상태 머신 구동.
- **Actuation Layer:** 결정된 상태에 따라 2채널 하드웨어 PWM을 통해 좌우 모터의 속도와 방향을 독립 제어.

## 🚀 Key Implementations

### **⚙️ 하드웨어: 고정밀 센싱 및 비동기 제어**
- **Timer Input Capture (IC) 기반 거리 측정:** CPU 폴링을 배제하고 인터럽트 방식을 사용하여 $\mu s$ 단위의 정밀한 거리 계산 및 시스템 부하 최소화.
- **Multi-Sensor Sync Logic:** 좌/중/우 3개 센서의 수신 완료 플래그(`US_F`)가 모두 1인 시점에만 연산을 시작하여 데이터 부정확성 방지.
- **Hardware PWM Generator:** 독립적인 2채널 PWM 채널을 구축하여 조향 각도와 주행 속도를 하드웨어 레지스터 수준에서 정밀 제어.

### **🛰️ 소프트웨어: 지능형 자율주행 알고리즘**
- **이동평균 필터 (Moving Average Filter):** 초음파 센서의 노이즈를 억제하기 위해 최근 5개 샘플의 평균값을 계산하는 필터링 로직 구현.
- **Centering (미세 조향) 로직:** 단순 회전이 아닌 좌우 거리 차이($\Delta Distance > 10cm$)를 계산하여 넓은 공간으로 경로를 보정하는 `MOVEQ`/`MOVEE` 모드 설계.
- **UART DMA Debugging:** 실시간 센서 데이터와 `Auto_Pilot` 상태를 UART DMA로 전송하여 주행 중 시스템 내부 상태 실시간 모니터링.

## 🔍 Technical Deep Dive: Hysteresis Control

단순 거리 비교 방식의 한계를 극복하기 위해 **히스테리시스(Hysteresis)** 개념을 도입했습니다. 특정 거리 임계값에서 빈번한 조향 변화를 방지하기 위해 이전 동작 상태를 유지하려는 관성을 부여하여 주행의 매끄러움을 확보했습니다.

## 🛠 Troubleshooting & Optimization

**✅ 조향 진동(Hunting) 및 코너 갇힘 해결**
- **문제:** 장애물 사이에서 좌/우 회전을 반복하며 앞으로 나아가지 못하는 '도리도리' 현상 및 코너 유턴 문제 발생.
- **해결:** `Last_ACT`(이전 동작) 변수를 도입하여 현재 조향 방향에 관성을 부여하고, 특정 거리 이하 위급 상황에서만 강제 회피하도록 로직을 개선하여 주행 안정성 확보.

**✅ 데이터 처리 지연 최적화**
- **내용:** 시리얼 통신으로 인한 처리 지연을 방지하기 위해 UART 통신에 **DMA(Direct Memory Access)**를 적용하여 CPU 개입 없이 백그라운드에서 데이터를 송수신하도록 최적화.

## 📈 Results & Review
하드웨어의 물리적 한계(센서 노이즈, 모터 지터)를 소프트웨어 알고리즘으로 극복하며 임베디드 시스템 설계의 전체 사이클을 경험했습니다. 특히 인터럽트와 레지스터 맵 기반의 주변장치 제어를 통해 하드웨어 최적화의 중요성을 깊이 체감할 수 있었습니다.


<img width="360" height="236" alt="image" src="https://github.com/user-attachments/assets/c24242aa-b2d9-4557-9834-294ec06cdd43" />



