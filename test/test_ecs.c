#include "unity.h"
#include "ecs.h"
#include <string.h>

static bool mock_up;
static bool mock_down;
static bool mock_left;
static bool mock_right;

static bool mock_input_action_held(InputAction action) {
    switch (action) {
        case ACTION_UP:    return mock_up;
        case ACTION_DOWN:  return mock_down;
        case ACTION_LEFT:  return mock_left;
        case ACTION_RIGHT: return mock_right;
        default:           return false;
    }
}

void setUp(void) {
    memset(positions, 0, sizeof(positions));
    memset(has_position, 0, sizeof(has_position));
    memset(rotations, 0, sizeof(rotations));
    memset(has_rotation, 0, sizeof(has_rotation));
    memset(scales, 0, sizeof(scales));
    memset(has_scale, 0, sizeof(has_scale));
    memset(entity_alive, 0, sizeof(entity_alive));
    memset(input_movers, 0, sizeof(input_movers));
    memset(has_input_mover, 0, sizeof(has_input_mover));
    memset(sprites, 0, sizeof(sprites));
    memset(has_sprite, 0, sizeof(has_sprite));
    memset(texts, 0, sizeof(texts));
    memset(has_text, 0, sizeof(has_text));
    memset(triangles, 0, sizeof(triangles));
    memset(has_triangle, 0, sizeof(has_triangle));
    memset(meshes, 0, sizeof(meshes));
    memset(has_mesh, 0, sizeof(has_mesh));
    memset(lightings, 0, sizeof(lightings));
    memset(has_lighting, 0, sizeof(has_lighting));
    memset(cameras, 0, sizeof(cameras));
    memset(has_camera, 0, sizeof(has_camera));
    memset(camera_behaviors, 0, sizeof(camera_behaviors));
    memset(has_camera_behavior, 0, sizeof(has_camera_behavior));
    mock_up = false;
    mock_down = false;
    mock_left = false;
    mock_right = false;
}

void tearDown(void) {
}

void test_ecs_create_entity_returns_zero(void) {
    entity_t e = ecs_create_entity();
    TEST_ASSERT_EQUAL_UINT16(0, e);
}

void test_ecs_create_entity_returns_sequential_ids(void) {
    entity_t e0 = ecs_create_entity();
    entity_t e1 = ecs_create_entity();
    entity_t e2 = ecs_create_entity();
    TEST_ASSERT_EQUAL_UINT16(0, e0);
    TEST_ASSERT_EQUAL_UINT16(1, e1);
    TEST_ASSERT_EQUAL_UINT16(2, e2);
}

void test_ecs_destroy_marks_entity_dead(void) {
    entity_t e = ecs_create_entity();
    TEST_ASSERT_TRUE(entity_alive[e]);
    ecs_destroy_entity(e);
    TEST_ASSERT_FALSE(entity_alive[e]);
}

void test_ecs_create_reuses_destroyed_slot(void) {
    entity_t e1 = ecs_create_entity();
    ecs_destroy_entity(e1);
    entity_t e2 = ecs_create_entity();
    TEST_ASSERT_EQUAL_UINT16(e1, e2);
}

void test_ecs_create_returns_max_when_full(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        ecs_create_entity();
    }
    entity_t e = ecs_create_entity();
    TEST_ASSERT_EQUAL_UINT16(MAX_ENTITIES, e);
}

void test_ecs_create_resets_all_component_flags(void) {
    has_position[0] = true;
    has_rotation[0] = true;
    has_scale[0] = true;
    has_camera[0] = true;
    has_camera_behavior[0] = true;
    has_sprite[0] = true;
    has_text[0] = true;
    has_triangle[0] = true;
    has_input_mover[0] = true;
    has_mesh[0] = true;
    has_lighting[0] = true;

    entity_t e = ecs_create_entity();
    TEST_ASSERT_FALSE(ecs_has_position(e));
    TEST_ASSERT_FALSE(ecs_has_rotation(e));
    TEST_ASSERT_FALSE(ecs_has_scale(e));
    TEST_ASSERT_FALSE(ecs_has_camera(e));
    TEST_ASSERT_FALSE(ecs_has_camera_behavior(e));
    TEST_ASSERT_FALSE(ecs_has_sprite(e));
    TEST_ASSERT_FALSE(ecs_has_text(e));
    TEST_ASSERT_FALSE(ecs_has_triangle(e));
    TEST_ASSERT_FALSE(ecs_has_input_mover(e));
    TEST_ASSERT_FALSE(ecs_has_mesh(e));
    TEST_ASSERT_FALSE(ecs_has_lighting(e));
}

void test_ecs_position_add_has_get_remove(void) {
    entity_t e = ecs_create_entity();
    Position p = { .x = 1.0f, .y = 2.0f, .z = 3.0f };
    ecs_add_position(e, p);
    TEST_ASSERT_TRUE(ecs_has_position(e));
    Position *got = ecs_get_position(e);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, got->x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, got->y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, got->z);
    ecs_remove_position(e);
    TEST_ASSERT_FALSE(ecs_has_position(e));
}

void test_ecs_rotation_add_has_get_remove(void) {
    entity_t e = ecs_create_entity();
    Rotation r = { .pitch = 10.0f, .yaw = 20.0f, .roll = 30.0f };
    ecs_add_rotation(e, r);
    TEST_ASSERT_TRUE(ecs_has_rotation(e));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, ecs_get_rotation(e)->pitch);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 20.0f, ecs_get_rotation(e)->yaw);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, ecs_get_rotation(e)->roll);
    ecs_remove_rotation(e);
    TEST_ASSERT_FALSE(ecs_has_rotation(e));
}

void test_ecs_scale_add_has_get_remove(void) {
    entity_t e = ecs_create_entity();
    Scale s = { .x = 1.0f, .y = 2.0f, .z = 3.0f };
    ecs_add_scale(e, s);
    TEST_ASSERT_TRUE(ecs_has_scale(e));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, ecs_get_scale(e)->x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, ecs_get_scale(e)->y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, ecs_get_scale(e)->z);
    ecs_remove_scale(e);
    TEST_ASSERT_FALSE(ecs_has_scale(e));
}

void test_ecs_has_position_false_for_new_entity(void) {
    entity_t e = ecs_create_entity();
    TEST_ASSERT_FALSE(ecs_has_position(e));
    TEST_ASSERT_FALSE(ecs_has_rotation(e));
    TEST_ASSERT_FALSE(ecs_has_scale(e));
}

void test_ecs_destroy_then_create_has_no_position(void) {
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){1, 2, 3});
    ecs_destroy_entity(e);
    entity_t e2 = ecs_create_entity();
    TEST_ASSERT_FALSE(ecs_has_position(e2));
}

void test_ecs_tick_logic_moves_up(void) {
    mock_up = true;
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){0, 10.0f, 0});
    ecs_add_input_mover(e, (InputMover){.speed = 5, .move_up = ACTION_UP, .move_down = ACTION_DOWN, .move_left = ACTION_LEFT, .move_right = ACTION_RIGHT});
    Position *p = ecs_get_position(e);
    ecs_tick_logic(mock_input_action_held);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.0f, p->y);
}

void test_ecs_tick_logic_moves_down(void) {
    mock_down = true;
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){0, 0.0f, 0});
    ecs_add_input_mover(e, (InputMover){.speed = 3});
    Position *p = ecs_get_position(e);
    ecs_tick_logic(mock_input_action_held);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, p->y);
}

void test_ecs_tick_logic_moves_left(void) {
    mock_left = true;
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){0, 0.0f, 0});
    ecs_add_input_mover(e, (InputMover){.speed = 7});
    Position *p = ecs_get_position(e);
    ecs_tick_logic(mock_input_action_held);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -7.0f, p->x);
}

void test_ecs_tick_logic_moves_right(void) {
    mock_right = true;
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){0, 0.0f, 0});
    ecs_add_input_mover(e, (InputMover){.speed = 4});
    Position *p = ecs_get_position(e);
    ecs_tick_logic(mock_input_action_held);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 4.0f, p->x);
}

void test_ecs_tick_logic_no_mover_no_movement(void) {
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){5.0f, 5.0f, 0});
    mock_up = true;
    ecs_tick_logic(mock_input_action_held);
    Position *p = ecs_get_position(e);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.0f, p->x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.0f, p->y);
}

void test_ecs_tick_logic_dead_entity_skipped(void) {
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){0, 0.0f, 0});
    ecs_add_input_mover(e, (InputMover){.speed = 10});
    ecs_destroy_entity(e);
    mock_up = true;
    ecs_tick_logic(mock_input_action_held);
    TEST_ASSERT_FALSE(ecs_has_position(e));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ecs_get_position(e)->y);
}

void test_ecs_tick_logic_moves_diagonal_up_left(void) {
    mock_up = true;
    mock_left = true;
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){10, 10, 0});
    ecs_add_input_mover(e, (InputMover){.speed = 3});
    ecs_tick_logic(mock_input_action_held);
    Position *p = ecs_get_position(e);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, p->x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, p->y);
}

void test_ecs_destroy_entity_idempotent(void) {
    entity_t e = ecs_create_entity();
    ecs_add_position(e, (Position){1, 2, 3});
    ecs_destroy_entity(e);
    ecs_destroy_entity(e);
    TEST_ASSERT_FALSE(entity_alive[e]);
    TEST_ASSERT_FALSE(ecs_has_position(e));
}

void test_ecs_accessor_sentinel_returns_safe_values(void) {
    TEST_ASSERT_FALSE(ecs_has_position(MAX_ENTITIES));
    TEST_ASSERT_FALSE(ecs_has_rotation(MAX_ENTITIES));
    TEST_ASSERT_FALSE(ecs_has_scale(MAX_ENTITIES));
    TEST_ASSERT_FALSE(ecs_has_camera(MAX_ENTITIES));
    TEST_ASSERT_FALSE(ecs_has_sprite(MAX_ENTITIES));
    TEST_ASSERT_FALSE(ecs_has_mesh(MAX_ENTITIES));
    TEST_ASSERT_FALSE(ecs_has_lighting(MAX_ENTITIES));
    TEST_ASSERT_FALSE(ecs_has_input_mover(MAX_ENTITIES));
    TEST_ASSERT_NULL(ecs_get_position(MAX_ENTITIES));
    TEST_ASSERT_NULL(ecs_get_rotation(MAX_ENTITIES));
    TEST_ASSERT_NULL(ecs_get_scale(MAX_ENTITIES));
    TEST_ASSERT_NULL(ecs_get_mesh(MAX_ENTITIES));
    TEST_ASSERT_NULL(ecs_get_lighting(MAX_ENTITIES));
    TEST_ASSERT_NULL(ecs_get_input_mover(MAX_ENTITIES));
}

void test_ecs_tick_logic_multiple_entities(void) {
    mock_up = true;
    entity_t e1 = ecs_create_entity();
    ecs_add_position(e1, (Position){0, 0.0f, 0});
    ecs_add_input_mover(e1, (InputMover){.speed = 2});

    entity_t e2 = ecs_create_entity();
    ecs_add_position(e2, (Position){0, 0.0f, 0});
    ecs_add_input_mover(e2, (InputMover){.speed = 5});

    ecs_tick_logic(mock_input_action_held);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, -2.0f, ecs_get_position(e1)->y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -5.0f, ecs_get_position(e2)->y);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ecs_create_entity_returns_zero);
    RUN_TEST(test_ecs_create_entity_returns_sequential_ids);
    RUN_TEST(test_ecs_destroy_marks_entity_dead);
    RUN_TEST(test_ecs_create_reuses_destroyed_slot);
    RUN_TEST(test_ecs_create_returns_max_when_full);
    RUN_TEST(test_ecs_create_resets_all_component_flags);
    RUN_TEST(test_ecs_position_add_has_get_remove);
    RUN_TEST(test_ecs_rotation_add_has_get_remove);
    RUN_TEST(test_ecs_scale_add_has_get_remove);
    RUN_TEST(test_ecs_has_position_false_for_new_entity);
    RUN_TEST(test_ecs_destroy_then_create_has_no_position);
    RUN_TEST(test_ecs_tick_logic_moves_up);
    RUN_TEST(test_ecs_tick_logic_moves_down);
    RUN_TEST(test_ecs_tick_logic_moves_left);
    RUN_TEST(test_ecs_tick_logic_moves_right);
    RUN_TEST(test_ecs_tick_logic_no_mover_no_movement);
    RUN_TEST(test_ecs_tick_logic_dead_entity_skipped);
    RUN_TEST(test_ecs_tick_logic_multiple_entities);
    RUN_TEST(test_ecs_tick_logic_moves_diagonal_up_left);
    RUN_TEST(test_ecs_destroy_entity_idempotent);
    RUN_TEST(test_ecs_accessor_sentinel_returns_safe_values);
    return UNITY_END();
}
