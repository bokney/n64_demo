CC ?= cc
CFLAGS := -std=gnu17 -Wall -Wextra -I../src -Imocks -I../unity/src

STATE_SRC := ../src/state.c
ECS_SRC := ../src/ecs.c \
           ../src/systems/input_components.c \
           ../src/systems/render_components.c \
           ../src/systems/lighting_components.c \
           ../src/systems/camera_components.c
UNITY_SRC := ../unity/src/unity.c

test_state: test_state.c $(STATE_SRC) $(UNITY_SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/test_state $^

test_ecs: test_ecs.c $(ECS_SRC) $(UNITY_SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/test_ecs $^

test: test_state test_ecs
	@echo "=== State machine tests ==="
	./bin/test_state
	@echo "=== ECS tests ==="
	./bin/test_ecs

clean:
	rm -rf bin

.PHONY: test clean
