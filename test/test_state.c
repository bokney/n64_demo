#include "unity.h"
#include "state.h"

static int init_call_count;
static int update_call_count;
static int exit_call_count;
static uint8_t update_return_value;
static uint8_t last_returned_state;

static void mock_init(void) {
    init_call_count++;
}

static uint8_t mock_update(void) {
    update_call_count++;
    return update_return_value;
}

static uint8_t mock_exit(void) {
    exit_call_count++;
    return 0;
}

void setUp(void) {
    init_call_count = 0;
    update_call_count = 0;
    exit_call_count = 0;
    update_return_value = 0;
    last_returned_state = 0;
}

void tearDown(void) {
}

void test_assign_state_sets_function_pointers(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    TEST_ASSERT_EQUAL_PTR(mock_init, s.init);
    TEST_ASSERT_EQUAL_PTR(mock_update, s.update);
    TEST_ASSERT_EQUAL_PTR(mock_exit, s.exit);
}

void test_assign_state_resets_phase_and_step(void) {
    state s;
    s.phase = 2;
    s.step = 999;
    assign_state(&s, mock_init, mock_update, mock_exit);
    TEST_ASSERT_EQUAL_UINT8(0, s.phase);
    TEST_ASSERT_EQUAL_UINT32(0, s.step);
}

void test_state_update_phase_transitions_0_to_1(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    TEST_ASSERT_EQUAL_UINT8(1, s.phase);
}

void test_state_update_returns_zero_when_no_transition(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    last_returned_state = state_update(&s);
    TEST_ASSERT_EQUAL(0, last_returned_state);
    TEST_ASSERT_EQUAL(2, update_call_count);
}

void test_state_update_transitions_on_nonzero_return(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    update_return_value = 42;
    last_returned_state = state_update(&s);
    TEST_ASSERT_EQUAL(42, last_returned_state);
    TEST_ASSERT_EQUAL(1, exit_call_count);
}

void test_state_update_resets_phase_after_transition(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    update_return_value = 10;
    state_update(&s);
    TEST_ASSERT_EQUAL_UINT8(0, s.phase);
}

void test_state_update_step_increments_on_each_call(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    TEST_ASSERT_EQUAL(1, s.step);
    state_update(&s);
    TEST_ASSERT_EQUAL(2, s.step);
}

void test_state_update_init_and_update_on_same_first_tick(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    TEST_ASSERT_EQUAL(1, init_call_count);
    TEST_ASSERT_EQUAL(1, update_call_count);
    TEST_ASSERT_EQUAL(0, exit_call_count);
}

void test_state_update_does_not_call_init_on_second_call(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    state_update(&s);
    TEST_ASSERT_EQUAL(1, init_call_count);
    TEST_ASSERT_EQUAL(2, update_call_count);
}

void test_state_update_with_null_init(void) {
    state s;
    assign_state(&s, NULL, mock_update, mock_exit);
    last_returned_state = state_update(&s);
    TEST_ASSERT_EQUAL(0, last_returned_state);
    TEST_ASSERT_EQUAL(0, init_call_count);
    TEST_ASSERT_EQUAL(1, update_call_count);
}

void test_state_update_with_null_update(void) {
    state s;
    assign_state(&s, mock_init, NULL, mock_exit);
    last_returned_state = state_update(&s);
    TEST_ASSERT_EQUAL(0, last_returned_state);
    TEST_ASSERT_EQUAL(1, init_call_count);
    TEST_ASSERT_EQUAL(0, update_call_count);
}

void test_state_update_with_null_exit_does_not_crash(void) {
    state s;
    assign_state(&s, mock_init, mock_update, NULL);
    state_update(&s);
    update_return_value = 5;
    last_returned_state = state_update(&s);
    TEST_ASSERT_EQUAL(5, last_returned_state);
}

void test_state_update_resets_step_after_transition(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    update_return_value = 99;
    state_update(&s);
    TEST_ASSERT_EQUAL_UINT32(0, s.step);
}

void test_state_update_reentrant_init(void) {
    state s;
    assign_state(&s, mock_init, mock_update, mock_exit);
    state_update(&s);
    update_return_value = 99;
    state_update(&s);
    update_return_value = 99;
    state_update(&s);
    TEST_ASSERT_EQUAL(2, init_call_count);
    TEST_ASSERT_EQUAL(3, update_call_count);
    TEST_ASSERT_EQUAL(2, exit_call_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_assign_state_sets_function_pointers);
    RUN_TEST(test_assign_state_resets_phase_and_step);
    RUN_TEST(test_state_update_phase_transitions_0_to_1);
    RUN_TEST(test_state_update_returns_zero_when_no_transition);
    RUN_TEST(test_state_update_transitions_on_nonzero_return);
    RUN_TEST(test_state_update_resets_phase_after_transition);
    RUN_TEST(test_state_update_step_increments_on_each_call);
    RUN_TEST(test_state_update_init_and_update_on_same_first_tick);
    RUN_TEST(test_state_update_does_not_call_init_on_second_call);
    RUN_TEST(test_state_update_with_null_init);
    RUN_TEST(test_state_update_with_null_update);
    RUN_TEST(test_state_update_with_null_exit_does_not_crash);
    RUN_TEST(test_state_update_resets_step_after_transition);
    RUN_TEST(test_state_update_reentrant_init);
    return UNITY_END();
}
