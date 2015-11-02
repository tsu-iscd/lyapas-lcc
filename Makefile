BINARIES=lex_lyapas_im0
TEMP_FILES=lex_lyapas_im0.c

all: $(BINARIES)

lex_lyapas_im0: lex_lyapas_im0.c
	gcc -o $@ $< -lfl

lex_lyapas_im0.c: lex_lyapas_im0.l
	flex -o $@ $<


EXAMPLES_DIR=examples
AUX_DIR=aux

TEST_NUMBERS=1 2 3 4
TEST_EXAMPLES=$(foreach num, $(TEST_NUMBERS), test-ex$(num))
TEST_EXAMPLES_LYAPAS=$(foreach num, $(TEST_NUMBERS), test-ex$(num)-lyapas)
TEST_EXAMPLES_IM0=$(foreach num, $(TEST_NUMBERS), test-ex$(num)-im0)
TEST_EXAMPLES_IM1=$(foreach num, $(TEST_NUMBERS), test-ex$(num)-im1)
TEST_EXAMPLES_IM2=$(foreach num, $(TEST_NUMBERS), test-ex$(num)-im2)

test: test-ex

test-ex: $(TEST_EXAMPLES)

$(TEST_EXAMPLES): test-ex%: test-ex%-lyapas test-ex%-im0 test-ex%-im1 test-ex%-im2

$(TEST_EXAMPLES_LYAPAS): test-ex%-lyapas: $(EXAMPLES_DIR)/ex%.lyapas
	@true

$(TEST_EXAMPLES_IM0): test-ex%-im0: $(EXAMPLES_DIR)/ex%.im0 $(AUX_DIR)/im0.schema.json
	python -m jsonschema -i $< $(AUX_DIR)/im0.schema.json

$(TEST_EXAMPLES_IM1): test-ex%-im1: $(EXAMPLES_DIR)/ex%.im1 $(AUX_DIR)/im1.schema.json
	python -m jsonschema -i $< $(AUX_DIR)/im1.schema.json

$(TEST_EXAMPLES_IM2): test-ex%-im2: $(EXAMPLES_DIR)/ex%.im2 $(AUX_DIR)/im2.schema.json
	python -m jsonschema -i $< $(AUX_DIR)/im2.schema.json


clean:
	rm -f $(BINARIES) $(TEMP_FILES)
