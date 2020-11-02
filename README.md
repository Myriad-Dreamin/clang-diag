
## clang-diag

format clang error

### Compilation

#### Step1

Tou can configure project by `config.cmake`. Then you should check or set the value of `KERNEL_FUZZER_LLVM_PROJECT_ROOT` to the path to local llvm-project. please download [llvm-project](https://github.com/llvm/llvm-project) if it is not exists.

```shell script
# donwload from GitHub
git clone https://github.com/llvm/llvm-project vendor/llvm-project

# vi config.default.cmake
# set llvm project root directory
# # set(KERNEL_FUZZER_LLVM_PROJECT_ROOT path/to/llvm-project)
cp config.default.cmake config.cmake
```

#### Step2

run `make` in default configuration:

```shell script
make build-diag # config Debug mode && make -j48
```

or you can compile `clang-diag` in Release mode:

```shell script
cd cmake-build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j<cpu-core-count> clang-diag
```

#### Step3

Run the command to verify that it works well:
```shell script
cmake-build-${CMAKE_BUILD_TYPE} --version
```
it must outputs:
```plain
LLVM (http://llvm.org/):
  LLVM version 12.0.0git
# debug mode >>>
  DEBUG build with assertions. 
# release mode >>
  Optimized build.
# <<<
  Default target: x86_64-pc-windows-msvc
  Host CPU: skylake

clang-diag: https://github.com/Myriad-Dreamin/clang-diag
  clang-diag version 1.0.0
```

###  Usage

#### get info from clang

Append clang option `--serialize-diagnostics <diag-file>` to get clang serialized diagnostics info.

```shell script
clang ${compile-options} --serialize-diagnostics ./diagnostics
```

#### display available options

```shell script
clang-diag --help
```

```plain
USAGE: clang-diag [options] <input file>

OPTIONS:

Color Options:

  --color           - Use colors in output (default=autodetect)

Generic Options:

  --help            - Display available options (--help-hidden for more)
  --help-list       - Display list of available options (--help-list-hidden for more)
  --version         - Display the version of this program

clang-diag options:

  --format=<value>  - format diag to format
    =json           -   in json format
    =yaml           -   in yaml format
    =plugin         -   use format from that in dynamic plugin
  --output=<string> - the output path, default 'stdout'
  --raw             - simply convert serialized file to target format instead of changing the restructured info of diagnostics file at the same time
  --verbose         - see verbose info

clang diagnostics tool
```

#### convert file format to different style

The default value of argument `--raw` is `false`, `clang-diag` will not change the structure of file data if it is set.

The struct of raw diagnostics file can be described as follow:

```haskell
type FileID/CategoryID/FlagID/Severity/Line/Column/Offset = unsigned
data Location = Location FileID Line Column Offset
data RawDiagnosticsRecord =
    CategoryRecord { id :: CategoryID, name :: String } |
    DiagFlagRecord { id :: FlagID    , name :: String } |
    FilenameRecord { id :: FileID    , name :: String, size :: Size, timestamp :: Timestamp } |
    DiagnosticRecord { 
        severity :: Severity  ,
        category :: CategoryID,
        flag     :: FlagID    ,
        location :: Location  ,
        message  :: String } |
    SourceRangeRecord { beginLoc :: Location, endLoc :: Location }
data RawDiagnosticsFile = [ RawDiagnosticsRecord ]
```

A json example of `RawDiagnosticsRecord`s corresponding to the `ExtendedDiagnosticsRecord` in the next example:

```json
[
  {
    "category": 0,
    "flag": 0,
    "location": {
      "col": 10,
      "file_id": 1,
      "line": 3,
      "offset": 84
    },
    "msg": "in file included from test_main.c:3:",
    "severity": 1,
    "type": 3
  },
  {
    "category": 2,
    "flag": 0,
    "location": {
      "col": 40,
      "file_id": 6,
      "line": 433,
      "offset": 14610
    },
    "msg": "unknown type name '__gnuc_va_list'",
    "severity": 3,
    "type": 3
  },
  {
    "category": 0,
    "flag": 0,
    "location": {
      "col": 20,
      "file_id": 3,
      "line": 51,
      "offset": 1160
    },
    "msg": "expanded from macro '_G_va_list'",
    "severity": 1,
    "type": 3
  }
]
```

The struct of restructured diagnostics file can be described as follow:

```haskell
data ExtendedDiagnosticsRecord = ExtendedDiagnosticsRecord {
    base :: RawDiagnosticsRecord,
    diagStack :: [ Location ],
    expansionNote :: [ RawDiagnosticsRecord ]
}

data ClangDiagnostics = ClangDiagnostics {
    categoryMap :: Map CategoryID String,
    fileMap     :: Map FileID     String,
    flagMap     :: Map FlagID     String,
    records     :: [ ExtendedDiagnosticsRecord ]
}
```

A json example of `ExtendedDiagnosticsRecord`:

```json
{
  "category": 2,
  "expansion_note": [
    {
      "category": 0,
      "flag": 0,
      "location": {
        "col": 20,
        "file_id": 3,
        "line": 51,
        "offset": 1160
      },
      "msg": "expanded from macro '_G_va_list'",
      "severity": 1
    }
  ],
  "flag": 0,
  "location": {
    "col": 9,
    "file_id": 6,
    "line": 46,
    "offset": 1343
  },
  "msg": "unknown type name '__gnuc_va_list'",
  "severity": 3,
  "stack": [
    {
      "col": 10,
      "file_id": 1,
      "line": 3,
      "offset": 84
    }
  ]
}
```

#### get verbose execution info

The default value of verbose argument `--verbose` is `false`.

### License

Apache License v2.0 with LLVM Exceptions
