// SPDX-License-Identifier: GPL-2.0-only
/*
 * iris-flow_test.c - Test suite for Iris Flow driver
 */

#include "libiris-flow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int passed = 0, failed = 0;

#define TEST(name) do { \
	printf("  TEST: %s ... ", name); \
	fflush(stdout); \
} while (0)
#define PASS() do { printf("PASS\n"); passed++; } while(0)
#define FAIL(m) do { printf("FAIL: %s\n", m); failed++; } while(0)
#define SKIP() do { printf("SKIP (driver not loaded)\n"); } while(0)

static void test_open_close(void) {
	TEST("open/close");
	struct iris_flow_handle *h = iris_flow_open("/dev/iris-flow0");
	if (!h) { SKIP(); return; }
	iris_flow_close(h);
	PASS();
}

static void test_start_stop(void) {
	TEST("start/stop stream");
	struct iris_flow_handle *h = iris_flow_open("/dev/iris-flow0");
	if (!h) { SKIP(); return; }
	int ret = iris_flow_start_stream(h, 1920, 1080, 1, 30);
	if (ret != 0) { iris_flow_close(h); FAIL("start failed"); return; }
	ret = iris_flow_stop_stream(h);
	if (ret != 0) { iris_flow_close(h); FAIL("stop failed"); return; }
	iris_flow_close(h);
	PASS();
}

static void test_get_frame(void) {
	TEST("get frame");
	struct iris_flow_handle *h = iris_flow_open("/dev/iris-flow0");
	if (!h) { SKIP(); return; }
	uint32_t frame_id; uint64_t ts;
	int ret = iris_flow_get_frame(h, &frame_id, &ts);
	if (ret != 0) { iris_flow_close(h); FAIL("get_frame failed"); return; }
	iris_flow_close(h);
	PASS();
}

static void test_set_isp(void) {
	TEST("set ISP params");
	struct iris_flow_handle *h = iris_flow_open("/dev/iris-flow0");
	if (!h) { SKIP(); return; }
	int ret = iris_flow_set_isp(h, 128, 128, 128);
	if (ret != 0) { iris_flow_close(h); FAIL("set_isp failed"); return; }
	iris_flow_close(h);
	PASS();
}

static void test_get_status(void) {
	TEST("get status");
	struct iris_flow_handle *h = iris_flow_open("/dev/iris-flow0");
	if (!h) { SKIP(); return; }
	uint32_t status;
	int ret = iris_flow_get_status(h, &status);
	if (ret != 0) { iris_flow_close(h); FAIL("get_status failed"); return; }
	iris_flow_close(h);
	PASS();
}

static void test_invalid_args(void) {
	TEST("invalid args (NULL)");
	int ret = iris_flow_start_stream(NULL, 0, 0, 0, 0);
	if (ret == -EINVAL) PASS(); else FAIL("expected -EINVAL");
}

int main(void) {
	printf("Iris Flow Driver Test Suite\n");
	printf("============================\n\n");
	test_open_close();
	test_start_stop();
	test_get_frame();
	test_set_isp();
	test_get_status();
	test_invalid_args();
	printf("\nResults: %d passed, %d failed\n", passed, failed);
	return failed > 0 ? 1 : 0;
}
