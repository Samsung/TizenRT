# Sleep Callback Management System with Priority Support

## 📋 System Overview

A lightweight, priority-based sleep callback management system that allows application modules to execute custom operations when the system enters/exits sleep modes.

### Key Features

✅ **Priority-based execution** - Callbacks execute in configurable order  
✅ **Dual independent lists** - Separate lists for pre-sleep and post-sleep  
✅ **Memory efficient** - Only 16 bytes per callback (32-bit ARM)  
✅ **Easy to use** - Register with priority, system handles the rest  
✅ **Thread-safe** - Interrupt-protected operations  

### Core APIs (6 Functions)

| Function | Purpose | Complexity |
|----------|---------|------------|
| `bk_pm_pre_sleep_callback_register(cb, arg, priority)` | Register pre-sleep callback | O(n) |
| `bk_pm_pre_sleep_callback_unregister(cb)` | Unregister pre-sleep callback | O(n) |
| `bk_pm_pre_sleep_callback_execute()` | Execute all pre-sleep callbacks | O(n) |
| `bk_pm_post_sleep_callback_register(cb, arg, priority)` | Register post-wakeup callback | O(n) |
| `bk_pm_post_sleep_callback_unregister(cb)` | Unregister post-wakeup callback | O(n) |
| `bk_pm_post_sleep_callback_execute()` | Execute all post-wakeup callbacks | O(n) |

## 🎯 Priority System

### Priority Levels (0-255)

Lower value = Higher priority = Executes first

| Priority | Constant | Use Case | Examples |
|----------|----------|----------|----------|
| **0** | `PM_CALLBACK_PRIORITY_CRITICAL` | Critical hardware | Clock manager, PMU, Power domains |
| **50** | `PM_CALLBACK_PRIORITY_HIGH` | Important peripherals | UART, SPI, I2C |
| **100** | `PM_CALLBACK_PRIORITY_NORMAL` | Standard drivers | GPIO, ADC, PWM, Timers |
| **150** | `PM_CALLBACK_PRIORITY_LOW` | Non-critical modules | Sensors, Displays, LEDs |
| **200** | `PM_CALLBACK_PRIORITY_LOWEST` | Optional features | Debug logging, Statistics |

### Custom Priorities

You can use any value between 0-255 for fine-grained control:

```c
#define MY_WIFI_PRIORITY    45   // Between CRITICAL and HIGH
#define MY_BT_PRIORITY      55   // Between HIGH and NORMAL
#define MY_CUSTOM_PRIORITY  125  // Between NORMAL and LOW
```

### Execution Order Visualization

```
╔════════════════════════════════════╗
║  Pre-Sleep Callbacks (0 → 255)    ║
╠════════════════════════════════════╣
║  Priority   0: Clock Manager       ║  ← Executes FIRST
║  Priority  10: Power Manager       ║
║  Priority  45: WiFi Driver         ║
║  Priority  50: UART Driver         ║
║  Priority 100: GPIO Driver         ║
║  Priority 150: Sensor Driver       ║
║  Priority 200: Debug Logger        ║  ← Executes LAST
╠════════════════════════════════════╣
║      【 ENTER SLEEP MODE 】        ║
╠════════════════════════════════════╣
║  Post-Sleep Callbacks (0 → 255)    ║
╠════════════════════════════════════╣
║  Priority   0: Clock Manager       ║  ← Executes FIRST
║  Priority  10: Power Manager       ║
║  Priority  45: WiFi Driver         ║
║  Priority  50: UART Driver         ║
║  Priority 100: GPIO Driver         ║
║  Priority 150: Sensor Driver       ║
║  Priority 200: Debug Logger        ║  ← Executes LAST
╚════════════════════════════════════╝
```

## 📝 Usage Examples

### Example 1: Basic Usage with Priority

```c
#include "pm_sleep_callback.h"

void uart_pre_sleep(void *arg)
{
    printf("UART: saving state\n");
    uart_save_and_powerdown();
}

void uart_post_sleep(void *arg)
{
    printf("UART: restoring state\n");
    uart_powerup_and_restore();
}

void uart_driver_init(void)
{
    /* Register with HIGH priority (50) - UART is important for logging */
    bk_pm_pre_sleep_callback_register(uart_pre_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
    bk_pm_post_sleep_callback_register(uart_post_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
}

void uart_driver_deinit(void)
{
    bk_pm_pre_sleep_callback_unregister(uart_pre_sleep);
    bk_pm_post_sleep_callback_unregister(uart_post_sleep);
}
```

### Example 2: Priority Configuration Table

```c
typedef struct {
    const char *name;
    sleep_callback_t pre_cb;
    sleep_callback_t post_cb;
    void *context;
    uint8_t priority;
} module_config_t;

/* Centralized configuration table */
static module_config_t module_table[] = {
    // Name,      Pre-CB,          Post-CB,         Context, Priority
    {"Clock",     clock_pre_cb,    clock_post_cb,   NULL,    PM_CALLBACK_PRIORITY_CRITICAL},
    {"PMU",       pmu_pre_cb,      pmu_post_cb,     NULL,    10},
    {"UART0",     uart0_pre_cb,    uart0_post_cb,   &uart0,  PM_CALLBACK_PRIORITY_HIGH},
    {"SPI",       spi_pre_cb,      spi_post_cb,     &spi0,   60},
    {"GPIO",      gpio_pre_cb,     gpio_post_cb,    NULL,    PM_CALLBACK_PRIORITY_NORMAL},
    {"Sensor",    sensor_pre_cb,   sensor_post_cb,  &sns1,   PM_CALLBACK_PRIORITY_LOW},
    {"Debug",     debug_pre_cb,    debug_post_cb,   NULL,    PM_CALLBACK_PRIORITY_LOWEST},
};

void system_init_all_callbacks(void)
{
    int count = sizeof(module_table) / sizeof(module_config_t);
    
    for (int i = 0; i < count; i++) {
        module_config_t *m = &module_table[i];
        
        if (m->pre_cb) {
            bk_pm_pre_sleep_callback_register(m->pre_cb, m->context, m->priority);
        }
        if (m->post_cb) {
            bk_pm_post_sleep_callback_register(m->post_cb, m->context, m->priority);
        }
        
        printf("Registered [%s] with priority %d\n", m->name, m->priority);
    }
}
```

### Example 3: Context Sharing Between Pre and Post

```c
typedef struct {
    uint32_t gpio_states[32];
    uint32_t power_state;
    int saved;
} peripheral_ctx_t;

static peripheral_ctx_t periph_ctx = {0};

void peripheral_pre_sleep(void *arg)
{
    peripheral_ctx_t *ctx = (peripheral_ctx_t *)arg;
    
    /* Save all GPIO states */
    for (int i = 0; i < 32; i++) {
        ctx->gpio_states[i] = gpio_read(i);
    }
    
    /* Save power state */
    ctx->power_state = read_power_config();
    ctx->saved = 1;
    
    printf("Peripheral: saved %d GPIOs, power=0x%x\n", 32, ctx->power_state);
}

void peripheral_post_sleep(void *arg)
{
    peripheral_ctx_t *ctx = (peripheral_ctx_t *)arg;
    
    if (!ctx->saved) {
        printf("Warning: No saved state to restore!\n");
        return;
    }
    
    /* Restore power state first */
    write_power_config(ctx->power_state);
    
    /* Restore all GPIO states */
    for (int i = 0; i < 32; i++) {
        gpio_write(i, ctx->gpio_states[i]);
    }
    
    ctx->saved = 0;
    printf("Peripheral: restored %d GPIOs\n", 32);
}

void peripheral_init(void)
{
    /* Both callbacks share same context and priority */
    bk_pm_pre_sleep_callback_register(peripheral_pre_sleep, &periph_ctx, PM_CALLBACK_PRIORITY_NORMAL);
    bk_pm_post_sleep_callback_register(peripheral_post_sleep, &periph_ctx, PM_CALLBACK_PRIORITY_NORMAL);
}
```

## 🔄 System Integration

### Automatic Execution Flow

The system automatically calls callbacks at the right time in each sleep mode:

```c
/* Normal Sleep */
pm_normal_sleep_process() {
    // Preparation...
    bk_pm_pre_sleep_callback_execute();   // ← Executes all pre-sleep callbacks in priority order
    pm_enter_normal_sleep();
    bk_pm_post_sleep_callback_execute();  // ← Executes all post-sleep callbacks in priority order
    // Recovery...
}

/* Low Voltage Sleep */
pm_low_voltage_process() {
    // Preparation...
    bk_pm_pre_sleep_callback_execute();   // ← Automatic
    pm_enter_low_voltage();
    bk_pm_post_sleep_callback_execute();  // ← Automatic
    // Recovery...
}

/* Deep Sleep */
pm_deep_sleep_process() {
    // Preparation...
    bk_pm_pre_sleep_callback_execute();   // ← Automatic
    pm_enter_deep_sleep();
    bk_pm_post_sleep_callback_execute();  // ← Automatic
    // Recovery...
}
```

## 📊 Priority Configuration Guide

### Recommended Priority Assignment

#### Critical (0-49)
```c
Priority 0:  Clock source switching
Priority 5:  Power domain management
Priority 10: PMU (Power Management Unit)
Priority 15: Voltage regulator
Priority 20: Flash controller
```

#### High (50-99)
```c
Priority 50: UART (for debugging)
Priority 55: Bluetooth
Priority 60: SPI
Priority 70: I2C
Priority 80: DMA controller
```

#### Normal (100-149)
```c
Priority 100: GPIO
Priority 105: PWM
Priority 110: ADC
Priority 115: DAC
Priority 120: Timer
```

#### Low (150-199)
```c
Priority 150: Temperature sensor
Priority 160: Accelerometer
Priority 170: Display controller
Priority 180: LED controller
Priority 190: Audio codec
```

#### Lowest (200-255)
```c
Priority 200: Debug logging
Priority 210: Performance statistics
Priority 220: Event tracing
Priority 230: User applications
Priority 250: Test hooks
```

### Priority Selection Guidelines

**Rule 1: Dependencies**
- If module A depends on module B, B should have lower priority (higher priority value)
- Example: GPIO (100) depends on Clock (0), so Clock executes first

**Rule 2: Power Efficiency**
- Power-hungry modules should have lower priority
- Turn off power consumers as late as possible before sleep

**Rule 3: Wakeup Speed**
- Critical-path modules should have higher priority after wakeup
- Example: Clock must be restored before UART can log

**Rule 4: Similar Modules**
- Group similar modules with nearby priorities
- Easier to manage and understand

## 💾 Memory Management

### Memory Usage Formula

```
Total Memory = 8 + (N_pre × 16) + (N_post × 16) bytes

Where:
  8 = Fixed overhead (2 list heads)
  N_pre = Number of pre-sleep callbacks
  N_post = Number of post-sleep callbacks
  16 = Bytes per callback node (32-bit ARM, aligned)
```

### Memory Usage Table

| Pre-sleep | Post-sleep | Total Memory |
|-----------|------------|--------------|
| 0 | 0 | 8 bytes |
| 1 | 1 | 40 bytes |
| 3 | 2 | 88 bytes |
| 5 | 5 | 168 bytes |
| 10 | 10 | 328 bytes |

### Node Structure

```c
typedef struct sleep_callback_node {
    sleep_callback_t callback;  // 4 bytes
    void *arg;                  // 4 bytes
    uint8_t priority;           // 1 byte
    uint8_t padding[3];         // 3 bytes (compiler alignment)
    struct sleep_callback_node *next;  // 4 bytes
} sleep_callback_node_t;        // Total: 16 bytes
```

## ⚡ Performance Analysis

### Time Complexity

| Operation | Average Case | Worst Case |
|-----------|--------------|------------|
| Register | O(n/2) | O(n) |
| Unregister | O(n/2) | O(n) |
| Execute | O(n) | O(n) |

### Execution Time (32MHz ARM)

```
Registration time:  ~10n μs (n = current callback count)
Execution time:     ~5n μs + Σ(individual callback times)

Example with 10 callbacks, each taking 50μs:
  - Registration: ~100 μs
  - Execution:    ~50 μs + 500 μs = 550 μs
```

### Sleep Cycle Overhead

```
Pre-sleep callbacks (10 modules):   ~550 μs
Post-sleep callbacks (10 modules):  ~550 μs
Total overhead:                     ~1.1 ms

For typical sleep duration > 10ms, overhead < 10%
```

## 🚀 Quick Start Guide

### Step 1: Include Header

```c
#include "pm_sleep_callback.h"
```

### Step 2: Define Callback

```c
void my_pre_sleep_callback(void *arg)
{
    // Save state, power down, etc.
    printf("Preparing for sleep\n");
}

void my_post_sleep_callback(void *arg)
{
    // Restore state, power up, etc.
    printf("Waking up from sleep\n");
}
```

### Step 3: Register with Priority

```c
void my_module_init(void)
{
    // Register with appropriate priority
    bk_pm_pre_sleep_callback_register(my_pre_sleep_callback, NULL, PM_CALLBACK_PRIORITY_NORMAL);
    bk_pm_post_sleep_callback_register(my_post_sleep_callback, NULL, PM_CALLBACK_PRIORITY_NORMAL);
}
```

### Step 4: Unregister When Done

```c
void my_module_deinit(void)
{
    bk_pm_pre_sleep_callback_unregister(my_pre_sleep_callback);
    bk_pm_post_sleep_callback_unregister(my_post_sleep_callback);
}
```

### Step 5: System Handles Everything

No manual intervention needed - callbacks execute automatically!

## 📖 Complete Examples

### Example: Multi-Module System

```c
/* Priority allocation for system modules */

// Priority 0: Clock Manager (CRITICAL)
void clock_pre_sleep(void *arg) {
    switch_to_low_power_clock();
}

void clock_post_sleep(void *arg) {
    switch_to_high_performance_clock();
}

// Priority 50: UART (HIGH)
void uart_pre_sleep(void *arg) {
    uart_flush_and_powerdown();
}

void uart_post_sleep(void *arg) {
    uart_powerup_and_reconfigure();
}

// Priority 100: GPIO (NORMAL)
void gpio_pre_sleep(void *arg) {
    gpio_save_configuration();
}

void gpio_post_sleep(void *arg) {
    gpio_restore_configuration();
}

// Priority 150: Sensor (LOW)
void sensor_pre_sleep(void *arg) {
    sensor_enter_standby_mode();
}

void sensor_post_sleep(void *arg) {
    sensor_resume_normal_mode();
}

void system_init(void)
{
    /* Register all modules with appropriate priorities */
    bk_pm_pre_sleep_callback_register(clock_pre_sleep, NULL, PM_CALLBACK_PRIORITY_CRITICAL);
    bk_pm_post_sleep_callback_register(clock_post_sleep, NULL, PM_CALLBACK_PRIORITY_CRITICAL);
    
    bk_pm_pre_sleep_callback_register(uart_pre_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
    bk_pm_post_sleep_callback_register(uart_post_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
    
    bk_pm_pre_sleep_callback_register(gpio_pre_sleep, NULL, PM_CALLBACK_PRIORITY_NORMAL);
    bk_pm_post_sleep_callback_register(gpio_post_sleep, NULL, PM_CALLBACK_PRIORITY_NORMAL);
    
    bk_pm_pre_sleep_callback_register(sensor_pre_sleep, NULL, PM_CALLBACK_PRIORITY_LOW);
    bk_pm_post_sleep_callback_register(sensor_post_sleep, NULL, PM_CALLBACK_PRIORITY_LOW);
    
    /* Execution order (automatic, based on priority):
     * Before sleep:  Clock(0) → UART(50) → GPIO(100) → Sensor(150)
     * After wakeup:  Clock(0) → UART(50) → GPIO(100) → Sensor(150)
     */
}
```

## 🎓 Advanced Techniques

### Technique 1: Dynamic Priority Adjustment

```c
static uint8_t current_wifi_priority = PM_CALLBACK_PRIORITY_NORMAL;

void wifi_callback(void *arg) {
    // WiFi operations
}

void set_wifi_critical_mode(int enable)
{
    /* Dynamically change priority based on system state */
    if (enable) {
        bk_pm_pre_sleep_callback_unregister(wifi_callback);
        bk_pm_pre_sleep_callback_register(wifi_callback, NULL, PM_CALLBACK_PRIORITY_CRITICAL);
        current_wifi_priority = PM_CALLBACK_PRIORITY_CRITICAL;
    } else {
        bk_pm_pre_sleep_callback_unregister(wifi_callback);
        bk_pm_pre_sleep_callback_register(wifi_callback, NULL, PM_CALLBACK_PRIORITY_NORMAL);
        current_wifi_priority = PM_CALLBACK_PRIORITY_NORMAL;
    }
}
```

### Technique 2: Conditional Execution Based on Priority

```c
typedef struct {
    uint8_t min_priority_to_execute;  /* Only execute if priority <= this value */
} conditional_ctx_t;

static conditional_ctx_t cond_ctx = {.min_priority_to_execute = PM_CALLBACK_PRIORITY_NORMAL};

void conditional_callback(void *arg)
{
    conditional_ctx_t *ctx = (conditional_ctx_t *)arg;
    
    /* This callback was registered with a specific priority
     * We can check if we should execute based on system state
     */
    if (system_in_low_power_mode() && ctx->min_priority_to_execute < PM_CALLBACK_PRIORITY_HIGH) {
        printf("Skipping non-critical callback in low-power mode\n");
        return;
    }
    
    // Normal execution
    do_work();
}
```

### Technique 3: Same Callback, Different Priorities

```c
/* You can register same callback with different priorities for different contexts */
static int uart0_priority = PM_CALLBACK_PRIORITY_HIGH;
static int uart1_priority = PM_CALLBACK_PRIORITY_NORMAL;

void uart_common_handler(void *arg)
{
    int uart_id = (int)arg;
    printf("UART%d handler\n", uart_id);
}

void init_multi_uart(void)
{
    /* UART0 is debug console - HIGH priority */
    bk_pm_pre_sleep_callback_register(uart_common_handler, (void *)0, uart0_priority);
    
    /* UART1 is application port - NORMAL priority */
    bk_pm_pre_sleep_callback_register(uart_common_handler, (void *)1, uart1_priority);
}
```

## ⚠️ Important Notes

### Callback Restrictions

**✅ Safe to do in callbacks:**
- Read/write GPIO
- Save/restore register states
- Call non-blocking APIs
- Print messages (if UART priority is correct)
- Access memory

**❌ Avoid in callbacks:**
- Long delays (> 1ms recommended)
- Sleep/wait functions
- Registering/unregistering callbacks
- Complex blocking operations

### Thread Safety

- ✅ All register/unregister operations are interrupt-safe
- ✅ Callbacks execute sequentially (no concurrent execution)
- ⚠️ Shared resources in callbacks need additional protection

### Priority Best Practices

1. **Leave gaps** - Use 0, 50, 100, 150, 200 instead of 0, 1, 2, 3, 4
   - Allows inserting new modules between existing ones

2. **Group related modules** - Use nearby priorities (e.g., 50-59 for communication)

3. **Document your priorities** - Use comments or configuration table

4. **Test execution order** - Add debug prints to verify correct sequencing

## 🔍 Debugging

### Print All Registered Callbacks

```c
void debug_print_callbacks(void)
{
    // Add this to bk_pm_sleep_callback.c for debugging
    sleep_callback_node_t *curr;
    int count;
    
    printf("\n=== Pre-Sleep Callbacks ===\n");
    curr = g_pre_sleep_callback_head;
    count = 0;
    while (curr) {
        printf("  [%d] Priority %3d: callback=0x%p, arg=0x%p\n", 
               count++, curr->priority, curr->callback, curr->arg);
        curr = curr->next;
    }
    
    printf("\n=== Post-Sleep Callbacks ===\n");
    curr = g_post_sleep_callback_head;
    count = 0;
    while (curr) {
        printf("  [%d] Priority %3d: callback=0x%p, arg=0x%p\n", 
               count++, curr->priority, curr->callback, curr->arg);
        curr = curr->next;
    }
}
```

### Verify Execution Order

```c
void clock_cb(void *arg) { printf("[0] Clock\n"); }
void uart_cb(void *arg)  { printf("[50] UART\n"); }
void gpio_cb(void *arg)  { printf("[100] GPIO\n"); }

void test_priority_order(void)
{
    bk_pm_pre_sleep_callback_register(gpio_cb, NULL, 100);
    bk_pm_pre_sleep_callback_register(clock_cb, NULL, 0);
    bk_pm_pre_sleep_callback_register(uart_cb, NULL, 50);
    
    /* Trigger sleep (or call execute directly for testing) */
    bk_pm_pre_sleep_callback_execute();
    
    /* Expected output:
     * [0] Clock
     * [50] UART
     * [100] GPIO
     */
}
```

## ❓ FAQ

### Q1: What happens if two callbacks have the same priority?
**A:** They execute in registration order (FIFO for same priority level).

### Q2: Can I change a callback's priority after registration?
**A:** Yes, unregister and re-register with new priority:
```c
bk_pm_pre_sleep_callback_unregister(my_cb);
bk_pm_pre_sleep_callback_register(my_cb, arg, new_priority);
```

### Q3: What's the maximum number of callbacks?
**A:** Limited only by available memory. Recommended < 30 for optimal performance.

### Q4: Do pre-sleep and post-sleep callbacks need the same priority?
**A:** No. You can use different priorities if needed:
```c
bk_pm_pre_sleep_callback_register(cb, arg, 50);   // HIGH for pre-sleep
bk_pm_post_sleep_callback_register(cb, arg, 100); // NORMAL for post-sleep
```

### Q5: Can I use priority 255?
**A:** Yes, but 200 (LOWEST) is recommended as maximum for better debugging.

## 📂 File Structure

```
bk_pm/
├── include/
│   └── pm_sleep_callback.h              # API header with priority definitions
├── src/services/
│   ├── bk_pm_sleep_callback.c           # Implementation (267 lines)
│   ├── bk_pm_sleep_callback_example.c   # Complete examples
│   └── bk_pm_sleep.c                    # Integration (calls execute functions)
└── PM_SLEEP_CALLBACK_README.md          # This document
```

## ✅ Summary

**Key Improvements with Priority System:**
1. ✨ **Configurable execution order** - Full control over callback sequence
2. 🎯 **Predictable behavior** - Same order every time
3. 📊 **Easy to manage** - Configuration table approach
4. 🔧 **Flexible** - 256 priority levels, custom values supported
5. 💾 **Efficient** - Only 4 bytes overhead per node (priority + padding)

**Total Implementation:**
- ✅ 6 API functions
- ✅ 5 priority level constants
- ✅ Priority-sorted linked lists
- ✅ Complete examples
- ✅ Full documentation

**Ready to use - just register your callbacks with appropriate priorities!** 🚀
