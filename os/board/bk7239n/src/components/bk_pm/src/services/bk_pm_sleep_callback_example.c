// Copyright 2021-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * Sleep Callback Priority-Based System - Complete Examples
 * 
 * This file demonstrates the priority-based callback system with real-world scenarios
 */

#include <stdio.h>
#include "pm_sleep_callback.h"

/****************************************************************************
 * Example 1: Priority-based Execution Order Demonstration
 ****************************************************************************/

void example1_clock_callback(void *arg)
{
	printf("[Priority 0-CRITICAL] Clock manager executing\n");
	// Critical: Must execute first to ensure clock stability
}

void example1_uart_callback(void *arg)
{
	printf("[Priority 50-HIGH] UART driver executing\n");
	// Important: Needs early execution for logging
}

void example1_gpio_callback(void *arg)
{
	printf("[Priority 100-NORMAL] GPIO driver executing\n");
	// Standard: Normal priority is fine
}

void example1_sensor_callback(void *arg)
{
	printf("[Priority 150-LOW] Sensor driver executing\n");
	// Non-critical: Can execute later
}

void example1_debug_callback(void *arg)
{
	printf("[Priority 200-LOWEST] Debug logger executing\n");
	// Optional: Executes last
}

void example1_register_all(void)
{
	/* Register in random order - system will sort by priority */
	bk_bk_pm_pre_sleep_callback_register(example1_gpio_callback, NULL, PM_CALLBACK_PRIORITY_NORMAL);
	bk_bk_pm_pre_sleep_callback_register(example1_debug_callback, NULL, PM_CALLBACK_PRIORITY_LOWEST);
	bk_bk_pm_pre_sleep_callback_register(example1_clock_callback, NULL, PM_CALLBACK_PRIORITY_CRITICAL);
	bk_bk_pm_pre_sleep_callback_register(example1_sensor_callback, NULL, PM_CALLBACK_PRIORITY_LOW);
	bk_bk_pm_pre_sleep_callback_register(example1_uart_callback, NULL, PM_CALLBACK_PRIORITY_HIGH);
	
	/* Actual execution order (sorted by priority):
	 * 1. Clock manager   (0)
	 * 2. UART driver     (50)
	 * 3. GPIO driver     (100)
	 * 4. Sensor driver   (150)
	 * 5. Debug logger    (200)
	 */
}

/****************************************************************************
 * Example 2: Complete UART Driver with Save/Restore
 ****************************************************************************/

typedef struct {
	int uart_id;
	uint32_t baud_rate;
	uint32_t control_reg;
	uint32_t fifo_reg;
	int is_initialized;
} uart_context_t;

static uart_context_t uart1_ctx = {
	.uart_id = 1,
	.baud_rate = 115200,
	.is_initialized = 1
};

void uart_pre_sleep_handler(void *arg)
{
	uart_context_t *ctx = (uart_context_t *)arg;
	
	if (!ctx->is_initialized) return;
	
	printf("UART%d: Saving state before sleep\n", ctx->uart_id);
	
	/* Step 1: Flush TX buffer */
	uart_wait_tx_complete(ctx->uart_id);
	
	/* Step 2: Save registers */
	ctx->control_reg = uart_read_reg(ctx->uart_id, UART_CTRL_REG);
	ctx->fifo_reg = uart_read_reg(ctx->uart_id, UART_FIFO_REG);
	
	/* Step 3: Disable UART clock */
	uart_clock_disable(ctx->uart_id);
	
	printf("UART%d: State saved (ctrl=0x%x, fifo=0x%x)\n", 
	       ctx->uart_id, ctx->control_reg, ctx->fifo_reg);
}

void uart_post_sleep_handler(void *arg)
{
	uart_context_t *ctx = (uart_context_t *)arg;
	
	if (!ctx->is_initialized) return;
	
	printf("UART%d: Restoring state after wakeup\n", ctx->uart_id);
	
	/* Step 1: Enable UART clock */
	uart_clock_enable(ctx->uart_id);
	
	/* Step 2: Restore registers */
	uart_write_reg(ctx->uart_id, UART_CTRL_REG, ctx->control_reg);
	uart_write_reg(ctx->uart_id, UART_FIFO_REG, ctx->fifo_reg);
	
	/* Step 3: Re-configure baud rate */
	uart_set_baudrate(ctx->uart_id, ctx->baud_rate);
	
	printf("UART%d: State restored\n", ctx->uart_id);
}

void example2_uart_init(void)
{
	/* UART is important - use HIGH priority (50)
	 * This ensures UART is ready before normal drivers need logging
	 */
	bk_pm_pre_sleep_callback_register(uart_pre_sleep_handler, &uart1_ctx, PM_CALLBACK_PRIORITY_HIGH);
	bk_pm_post_sleep_callback_register(uart_post_sleep_handler, &uart1_ctx, PM_CALLBACK_PRIORITY_HIGH);
}

/****************************************************************************
 * Example 3: Multi-Module System with Dependency Management
 ****************************************************************************/

/* Priority Table Configuration:
 * 
 * Module          | Pre-Sleep Priority | Post-Sleep Priority | Rationale
 * ----------------+--------------------+---------------------+------------------
 * Clock Manager   | 0 (CRITICAL)       | 0 (CRITICAL)        | Must be first/last
 * Power Manager   | 10                 | 10                  | Controls power domains
 * UART Driver     | 50 (HIGH)          | 50 (HIGH)           | Needed for logging
 * SPI Driver      | 60                 | 60                  | Important peripheral
 * GPIO Driver     | 100 (NORMAL)       | 100 (NORMAL)        | Standard driver
 * ADC Driver      | 110                | 110                 | Standard driver
 * Sensor Driver   | 150 (LOW)          | 150 (LOW)           | Non-critical
 * LED Driver      | 180                | 180                 | Visual indicator
 * Debug Logger    | 200 (LOWEST)       | 200 (LOWEST)        | Optional
 */

/* Clock Manager - CRITICAL (0) */
void clock_pre_sleep(void *arg) {
	printf("[0] Switching to low-power clock\n");
	switch_to_lp_clock();
}

void clock_post_sleep(void *arg) {
	printf("[0] Switching to high-performance clock\n");
	switch_to_hp_clock();
}

/* UART - HIGH (50) */
void uart_pre_sleep(void *arg) {
	printf("[50] UART: flushing and power down\n");
	uart_flush();
	uart_powerdown();
}

void uart_post_sleep(void *arg) {
	printf("[50] UART: power up and reconfigure\n");
	uart_powerup();
	uart_reconfigure();
}

/* GPIO - NORMAL (100) */
void gpio_pre_sleep(void *arg) {
	printf("[100] GPIO: saving configuration\n");
	gpio_save_state();
}

void gpio_post_sleep(void *arg) {
	printf("[100] GPIO: restoring configuration\n");
	gpio_restore_state();
}

/* Sensor - LOW (150) */
void sensor_pre_sleep(void *arg) {
	printf("[150] Sensor: entering standby mode\n");
	sensor_enter_standby();
}

void sensor_post_sleep(void *arg) {
	printf("[150] Sensor: resuming normal operation\n");
	sensor_resume();
}

/* Debug - LOWEST (200) */
void debug_pre_sleep(void *arg) {
	printf("[200] Debug: logging sleep event\n");
	log_sleep_event();
}

void debug_post_sleep(void *arg) {
	printf("[200] Debug: logging wakeup event\n");
	log_wakeup_event();
}

void example3_register_system(void)
{
	/* Register all modules - they will execute in priority order */
	bk_pm_pre_sleep_callback_register(clock_pre_sleep, NULL, PM_CALLBACK_PRIORITY_CRITICAL);
	bk_pm_pre_sleep_callback_register(uart_pre_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
	bk_pm_pre_sleep_callback_register(gpio_pre_sleep, NULL, PM_CALLBACK_PRIORITY_NORMAL);
	bk_pm_pre_sleep_callback_register(sensor_pre_sleep, NULL, PM_CALLBACK_PRIORITY_LOW);
	bk_pm_pre_sleep_callback_register(debug_pre_sleep, NULL, PM_CALLBACK_PRIORITY_LOWEST);
	
	bk_pm_post_sleep_callback_register(clock_post_sleep, NULL, PM_CALLBACK_PRIORITY_CRITICAL);
	bk_pm_post_sleep_callback_register(uart_post_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
	bk_pm_post_sleep_callback_register(gpio_post_sleep, NULL, PM_CALLBACK_PRIORITY_NORMAL);
	bk_pm_post_sleep_callback_register(sensor_post_sleep, NULL, PM_CALLBACK_PRIORITY_LOW);
	bk_pm_post_sleep_callback_register(debug_post_sleep, NULL, PM_CALLBACK_PRIORITY_LOWEST);
}

/****************************************************************************
 * Example 4: Custom Priority Values
 ****************************************************************************/

/* Sometimes you need fine-grained control beyond predefined levels */

void wifi_pre_sleep(void *arg) {
	printf("[Priority 45] WiFi driver\n");
}

void bluetooth_pre_sleep(void *arg) {
	printf("[Priority 55] Bluetooth driver\n");
}

void example4_custom_priorities(void)
{
	/* WiFi needs to execute before Bluetooth but after clock manager
	 * Use custom priority values for fine control
	 */
	bk_pm_pre_sleep_callback_register(wifi_pre_sleep, NULL, 45);        // Between CRITICAL(0) and HIGH(50)
	bk_pm_pre_sleep_callback_register(bluetooth_pre_sleep, NULL, 55);   // Just after HIGH(50)
	
	/* Execution order:
	 * 1. wifi_pre_sleep (45)
	 * 2. bluetooth_pre_sleep (55)
	 */
}

/****************************************************************************
 * Example 5: Conditional Priority Adjustment
 ****************************************************************************/

typedef struct {
	int device_id;
	int is_critical_mode;  /* Dynamic priority adjustment */
} adaptive_ctx_t;

static adaptive_ctx_t adaptive_ctx = {
	.device_id = 1,
	.is_critical_mode = 0
};

void adaptive_callback(void *arg)
{
	adaptive_ctx_t *ctx = (adaptive_ctx_t *)arg;
	printf("[Adaptive] Device %d callback\n", ctx->device_id);
}

void example5_set_critical_mode(int enable)
{
	/* Change device importance at runtime */
	if (enable && !adaptive_ctx.is_critical_mode) {
		/* Entering critical mode - re-register with higher priority */
		bk_pm_pre_sleep_callback_unregister(adaptive_callback);
		bk_pm_pre_sleep_callback_register(adaptive_callback, &adaptive_ctx, PM_CALLBACK_PRIORITY_CRITICAL);
		adaptive_ctx.is_critical_mode = 1;
	} else if (!enable && adaptive_ctx.is_critical_mode) {
		/* Exiting critical mode - re-register with normal priority */
		bk_pm_pre_sleep_callback_unregister(adaptive_callback);
		bk_pm_pre_sleep_callback_register(adaptive_callback, &adaptive_ctx, PM_CALLBACK_PRIORITY_NORMAL);
		adaptive_ctx.is_critical_mode = 0;
	}
}

/****************************************************************************
 * Example 6: Priority Configuration Table
 ****************************************************************************/

/* Configuration table approach for managing multiple callbacks */

typedef struct {
	const char *name;
	sleep_callback_t pre_sleep_cb;
	sleep_callback_t post_sleep_cb;
	void *ctx;
	uint8_t priority;
} callback_config_entry_t;

/* Centralized configuration table */
static callback_config_entry_t callback_config_table[] = {
	/* name,         pre_cb,            post_cb,           ctx,   priority */
	{"ClockMgr",    clock_pre_sleep,   clock_post_sleep,  NULL,  PM_CALLBACK_PRIORITY_CRITICAL},
	{"UART",        uart_pre_sleep,    uart_post_sleep,   NULL,  PM_CALLBACK_PRIORITY_HIGH},
	{"GPIO",        gpio_pre_sleep,    gpio_post_sleep,   NULL,  PM_CALLBACK_PRIORITY_NORMAL},
	{"Sensor",      sensor_pre_sleep,  sensor_post_sleep, NULL,  PM_CALLBACK_PRIORITY_LOW},
	{"Debug",       debug_pre_sleep,   debug_post_sleep,  NULL,  PM_CALLBACK_PRIORITY_LOWEST},
};

#define CALLBACK_CONFIG_COUNT  (sizeof(callback_config_table) / sizeof(callback_config_entry_t))

void example6_register_from_table(void)
{
	int ret;
	
	printf("Registering %d modules from configuration table:\n", CALLBACK_CONFIG_COUNT);
	
	for (int i = 0; i < CALLBACK_CONFIG_COUNT; i++) {
		const callback_config_entry_t *entry = &callback_config_table[i];
		
		/* Register pre-sleep callback */
		if (entry->pre_sleep_cb) {
			ret = bk_pm_pre_sleep_callback_register(entry->pre_sleep_cb, entry->ctx, entry->priority);
			if (ret == 0) {
				printf("  [%s] Pre-sleep registered (priority %d)\n", entry->name, entry->priority);
			} else {
				printf("  [%s] Pre-sleep registration failed: %d\n", entry->name, ret);
			}
		}
		
		/* Register post-sleep callback */
		if (entry->post_sleep_cb) {
			ret = bk_pm_post_sleep_callback_register(entry->post_sleep_cb, entry->ctx, entry->priority);
			if (ret == 0) {
				printf("  [%s] Post-sleep registered (priority %d)\n", entry->name, entry->priority);
			} else {
				printf("  [%s] Post-sleep registration failed: %d\n", entry->name, ret);
			}
		}
	}
	
	printf("\nExpected execution order (by priority):\n");
	printf("  1. ClockMgr (0)\n");
	printf("  2. UART (50)\n");
	printf("  3. GPIO (100)\n");
	printf("  4. Sensor (150)\n");
	printf("  5. Debug (200)\n");
}

void example6_unregister_all(void)
{
	for (int i = 0; i < CALLBACK_CONFIG_COUNT; i++) {
		if (callback_config_table[i].pre_sleep_cb) {
			bk_pm_pre_sleep_callback_unregister(callback_config_table[i].pre_sleep_cb);
		}
		if (callback_config_table[i].post_sleep_cb) {
			bk_pm_post_sleep_callback_unregister(callback_config_table[i].post_sleep_cb);
		}
	}
}

/****************************************************************************
 * Example 7: Real-World WiFi Driver with State Machine
 ****************************************************************************/

typedef enum {
	WIFI_STATE_OFF,
	WIFI_STATE_IDLE,
	WIFI_STATE_CONNECTED,
	WIFI_STATE_SLEEPING
} wifi_state_t;

typedef struct {
	wifi_state_t state;
	uint32_t saved_regs[16];
	int connection_active;
	uint32_t ip_address;
} wifi_ctx_t;

static wifi_ctx_t wifi_ctx = {.state = WIFI_STATE_OFF};

void wifi_pre_sleep(void *arg)
{
	wifi_ctx_t *ctx = (wifi_ctx_t *)arg;
	
	printf("[WiFi] Pre-sleep handler (state=%d)\n", ctx->state);
	
	if (ctx->state == WIFI_STATE_CONNECTED) {
		/* Save connection info */
		ctx->connection_active = 1;
		ctx->ip_address = wifi_get_ip();
		
		/* Notify AP about sleep */
		wifi_send_null_frame_with_pm_bit();
		wifi_wait_ack();
		
		printf("[WiFi] Connection saved, IP=0x%x\n", ctx->ip_address);
	}
	
	/* Save WiFi registers */
	for (int i = 0; i < 16; i++) {
		ctx->saved_regs[i] = wifi_read_reg(i);
	}
	
	/* Power down WiFi */
	wifi_power_off();
	ctx->state = WIFI_STATE_SLEEPING;
}

void wifi_post_sleep(void *arg)
{
	wifi_ctx_t *ctx = (wifi_ctx_t *)arg;
	
	printf("[WiFi] Post-sleep handler (state=%d)\n", ctx->state);
	
	if (ctx->state != WIFI_STATE_SLEEPING) return;
	
	/* Power up WiFi */
	wifi_power_on();
	
	/* Restore registers */
	for (int i = 0; i < 16; i++) {
		wifi_write_reg(i, ctx->saved_regs[i]);
	}
	
	if (ctx->connection_active) {
		/* Restore connection */
		printf("[WiFi] Reconnecting to AP (IP=0x%x)\n", ctx->ip_address);
		wifi_fast_reconnect(ctx->ip_address);
		ctx->state = WIFI_STATE_CONNECTED;
	} else {
		ctx->state = WIFI_STATE_IDLE;
	}
}

void example7_wifi_init(void)
{
	/* WiFi needs moderate priority (between HIGH and NORMAL)
	 * - Not as critical as UART (for debugging)
	 * - More important than simple GPIO
	 */
	uint8_t wifi_priority = 75;  /* Custom priority */
	
	bk_pm_pre_sleep_callback_register(wifi_pre_sleep, &wifi_ctx, wifi_priority);
	bk_pm_post_sleep_callback_register(wifi_post_sleep, &wifi_ctx, wifi_priority);
}

/****************************************************************************
 * Example 8: Priority Visualization - Complete System
 ****************************************************************************/

void example8_print_execution_visualization(void)
{
	printf("\n");
	printf("╔═══════════════════════════════════════════════════════════════╗\n");
	printf("║          Sleep Callback Execution Order (by Priority)        ║\n");
	printf("╠═══════════════════════════════════════════════════════════════╣\n");
	printf("║                                                               ║\n");
	printf("║  BEFORE SLEEP (Pre-sleep callbacks, priority 0→255)          ║\n");
	printf("║  ───────────────────────────────────────────────────────      ║\n");
	printf("║   [Priority   0] Clock Manager    (CRITICAL)                 ║\n");
	printf("║   [Priority  10] Power Manager                               ║\n");
	printf("║   [Priority  45] WiFi Driver                                 ║\n");
	printf("║   [Priority  50] UART Driver      (HIGH)                     ║\n");
	printf("║   [Priority  55] Bluetooth Driver                            ║\n");
	printf("║   [Priority  60] SPI Driver                                  ║\n");
	printf("║   [Priority 100] GPIO Driver      (NORMAL)                   ║\n");
	printf("║   [Priority 110] ADC Driver                                  ║\n");
	printf("║   [Priority 150] Sensor Driver    (LOW)                      ║\n");
	printf("║   [Priority 180] LED Driver                                  ║\n");
	printf("║   [Priority 200] Debug Logger     (LOWEST)                   ║\n");
	printf("║                                                               ║\n");
	printf("║  【 ENTER SLEEP MODE 】                                      ║\n");
	printf("║  【 zzz... Hardware sleeping ... zzz 】                      ║\n");
	printf("║  【 HARDWARE WAKEUP 】                                       ║\n");
	printf("║                                                               ║\n");
	printf("║  AFTER WAKEUP (Post-sleep callbacks, priority 0→255)         ║\n");
	printf("║  ───────────────────────────────────────────────────────      ║\n");
	printf("║   [Priority   0] Clock Manager    (CRITICAL)                 ║\n");
	printf("║   [Priority  10] Power Manager                               ║\n");
	printf("║   [Priority  45] WiFi Driver                                 ║\n");
	printf("║   [Priority  50] UART Driver      (HIGH)                     ║\n");
	printf("║   [Priority  55] Bluetooth Driver                            ║\n");
	printf("║   [Priority  60] SPI Driver                                  ║\n");
	printf("║   [Priority 100] GPIO Driver      (NORMAL)                   ║\n");
	printf("║   [Priority 110] ADC Driver                                  ║\n");
	printf("║   [Priority 150] Sensor Driver    (LOW)                      ║\n");
	printf("║   [Priority 180] LED Driver                                  ║\n");
	printf("║   [Priority 200] Debug Logger     (LOWEST)                   ║\n");
	printf("║                                                               ║\n");
	printf("╚═══════════════════════════════════════════════════════════════╝\n");
	printf("\n");
}

/****************************************************************************
 * Memory and Performance Analysis
 ****************************************************************************/

/*
 * Per-callback memory overhead (with priority):
 *   - Callback function pointer: 4 bytes
 *   - User argument pointer:     4 bytes
 *   - Priority field:            1 byte
 *   - Padding (alignment):       3 bytes
 *   - Next pointer:              4 bytes
 *   Total per node:             16 bytes (32-bit ARM)
 * 
 * System overhead:
 *   - Pre-sleep list head:       4 bytes
 *   - Post-sleep list head:      4 bytes
 *   Total fixed overhead:        8 bytes
 * 
 * For N callbacks:
 *   Total memory = 8 + (N × 16) bytes
 * 
 * Examples:
 *   - 5 callbacks  (3 pre + 2 post): 8 + 80 = 88 bytes
 *   - 10 callbacks (5 pre + 5 post): 8 + 160 = 168 bytes
 *   - 20 callbacks (10 pre + 10 post): 8 + 320 = 328 bytes
 */

/*
 * Performance characteristics:
 * 
 * Operation complexities (with priority-sorted list):
 *   - Register callback:      O(n) - Insert in sorted position
 *   - Unregister callback:    O(n) - Search and remove
 *   - Execute all callbacks:  O(n) - Sequential iteration (already sorted)
 * 
 * Execution time estimates (32MHz ARM):
 *   - Register:    ~10n μs (search + malloc + insert)
 *   - Unregister:  ~5n μs (search + free)
 *   - Execute:     ~5n μs + Σ(callback execution time)
 * 
 * For n=10 callbacks, each taking 50μs:
 *   Pre-sleep overhead:  ~50μs + 500μs = 550μs
 *   Post-sleep overhead: ~50μs + 500μs = 550μs
 *   Total sleep cycle overhead: ~1.1ms (negligible for sleep > 10ms)
 */
