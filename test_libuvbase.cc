
#include"gtest/gtest.h"
#include"uv.h"
#include"spdlog/spdlog.h"


static uv_idle_t idle_handle;
static uv_check_t check_handle;
static uv_timer_t timer_handle;

static int idle_cb_called = 0;
static int check_cb_called = 0;
static int timer_cb_called = 0;
static int close_cb_called = 0;


static void close_cb(uv_handle_t* handle) {
  close_cb_called++;
}


static void timer_cb(uv_timer_t* handle) {
 // ASSERT(handle == &timer_handle);

  uv_close((uv_handle_t*) &idle_handle, close_cb);
  uv_close((uv_handle_t*) &check_handle, close_cb);
  uv_close((uv_handle_t*) &timer_handle, close_cb);

  timer_cb_called++;
  fprintf(stderr, "timer_cb %d\n", timer_cb_called);
  fflush(stderr);
}


static void idle_cb(uv_idle_t* handle) {
 //ASSERT(handle == &idle_handle);

  idle_cb_called++;
  fprintf(stderr, "idle_cb %d\n", idle_cb_called);
  fflush(stderr);
}


static void check_cb(uv_check_t* handle) {
 // ASSERT(handle == &check_handle);

  check_cb_called++;
  fprintf(stderr, "check_cb %d\n", check_cb_called);
  fflush(stderr);
}


TEST(idle_starvation,func) {
  int r;

  r = uv_idle_init(uv_default_loop(), &idle_handle);
  EXPECT_TRUE(r == 0);
  r = uv_idle_start(&idle_handle, idle_cb);
  EXPECT_TRUE(r == 0);

  r = uv_check_init(uv_default_loop(), &check_handle);
  EXPECT_TRUE(r == 0);
  r = uv_check_start(&check_handle, check_cb);
  EXPECT_TRUE(r == 0);

  r = uv_timer_init(uv_default_loop(), &timer_handle);
  EXPECT_TRUE(r == 0);
  r = uv_timer_start(&timer_handle, timer_cb, 500, 0);
  EXPECT_TRUE(r == 0);

  r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  EXPECT_TRUE(r == 0);

  EXPECT_TRUE(idle_cb_called > 0);
  EXPECT_TRUE(timer_cb_called == 1);
  EXPECT_TRUE(close_cb_called == 3);

  //MAKE_VALGRIND_HAPPY();
  //return 0;
}
