CC ?= cc
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -I../src -Imocks -I../unity/src

STATE_SRC := ../src/state.c
ECS_SRC := ../src/ecs.c \
           ../src/systems/input_components.c \
           ../src/systems/render_components.c \
           ../src/systems/lighting_components.c \
           ../src/systems/camera_components.c
UNITY_SRC := ../unity/src/unity.c

test_state: test_state.c $(STATE_SRC) $(UNITY_SRC)
	$(CC) $(CFLAGS) -o test_state $^

test_ecs: test_ecs.c $(ECS_SRC) $(UNITY_SRC)
	$(CC) $(CFLAGS) -o test_ecs $^

test: test_state test_ecs
	@echo "=== State machine tests ==="
	./test_state
	@echo "=== ECS tests ==="
	./test_ecs

clean:
	rm -f test_state test_ecs

.PHONY: test clean
