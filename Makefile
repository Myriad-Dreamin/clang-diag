
LLVM_PROJECT_ROOT ?= /mnt/sda2/mali-fuzz/llvm-project
MaliSourceList ?= build/source_list.txt

PHONY += build-diag
build-diag:
	cd cmake-build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j48 clang-diag

PHONY += llvm
llvm:
	mkdir -p ${LLVM_PROJECT_ROOT}/build
	cd ${LLVM_PROJECT_ROOT}/build && cmake -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra"  -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_GO_TESTS=OFF -A x64 -Thost=x64 ..\llvm
	cd ${LLVM_PROJECT_ROOT}/build && make all

# https://github.com/emscripten-core/emscripten-fastcomp/issues/188
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 6: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 8: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 28: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 29: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 40: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 42: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 54: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 65: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# : not foundali-fuzz/llvm-project/llvm/cmake/config.guess: 68: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess:
# /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess: 71: /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess: Syntax error: word unexpected (expecting "in")
# CMake Error at cmake/modules/GetHostTriple.cmake:36 (message):
#   Failed to execute /mnt/sda2/mali-fuzz/llvm-project/llvm/cmake/config.guess
# Call Stack (most recent call first):
#   cmake/config-ix.cmake:397 (get_host_triple)
#   CMakeLists.txt:671 (include)
PHONY += verify-bug-dos-config-guess
verify-bug-dos-config-guess:
	hexdump -C -n 4000 ${LLVM_PROJECT_ROOT}/llvm/cmake/config.guess | grep "0d 0a"

PHONY += fix-dos-config-guess
fix-dos-config-guess:
	dos2unix ${LLVM_PROJECT_ROOT}/llvm/cmake/config.guess

.PHONY: $(PHONY)
